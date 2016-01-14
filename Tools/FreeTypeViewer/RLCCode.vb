Public Class RLCCode

    Public Class RLCItem
        Public zeroCount As Byte
        Public Value As ShortVLICode.VLI

    End Class

    Public Shared Function loadfromDctResult(dct As DCT.DCTResult) As List(Of Short())
        Dim ret As New List(Of Short())


        Dim p1idx As Integer = 0
        Dim p2idx As Integer = 0
        For toread = 0 To 15

            Dim result As New List(Of Short)

            Dim stbit = p1idx

            Dim templist As New List(Of RLCItem)

            Dim items As Integer = 0

            While True
                Dim zeros = HuffmanWapperLib.Huffman.readBits(dct.ZeroAndGroup, 4, stbit)
                stbit += 4
                Dim valuebits = HuffmanWapperLib.Huffman.readBits(dct.ZeroAndGroup, 4, stbit)
                stbit += 4

                items += zeros
                If valuebits > 0 Then
                    items += 1
                ElseIf zeros = 15 Then
                    items += 1
                End If

                Dim rlcitem As New RLCItem()
                rlcitem.zeroCount = zeros
                rlcitem.Value = New ShortVLICode.VLI()
                rlcitem.Value.bits = valuebits

                templist.Add(rlcitem)

                If items >= 64 Then
                    Exit While
                End If

            End While

            p1idx = stbit


            stbit = p2idx

            Dim ridx As Integer
            For Each r In templist
                For index = 0 To r.zeroCount - 1
                    result.Add(0)
                Next
                If r.Value.bits > 0 Then
                    Dim value = HuffmanWapperLib.Huffman.readBits(dct.VLICode, r.Value.bits, stbit)
                    stbit += r.Value.bits
                    result.Add(ShortVLICode.getValue(r.Value.bits, value))
                ElseIf r.zeroCount = 15 And result.Count < 64 Then
                    result.Add(0)
                End If

                ridx += 1
            Next

            p2idx = IIf(stbit Mod 8 = 0, stbit, stbit + (8 - (stbit Mod 8)))


            ret.Add(result.ToArray())
        Next


        Return ret

    End Function



    'Public Shared Function loadfrombytes(data() As Byte) As Short()

    '    Dim result As New List(Of Short)

    '    Dim stbit = 0

    '    Dim templist As New List(Of RLCItem)

    '    Dim items As Integer = 0

    '    While True
    '        Dim zeros = HuffmanWapperLib.Huffman.readBits(data, 4, stbit)
    '        stbit += 4
    '        Dim valuebits = HuffmanWapperLib.Huffman.readBits(data, 4, stbit)
    '        stbit += 4

    '        items += zeros
    '        If valuebits > 0 Then
    '            items += 1
    '        ElseIf zeros = 15 Then
    '            items += 1
    '        End If

    '        Dim rlcitem As New RLCItem()
    '        rlcitem.zeroCount = zeros
    '        rlcitem.Value = New ShortVLICode.VLI()
    '        rlcitem.Value.bits = valuebits

    '        templist.Add(rlcitem)

    '        If items >= 64 Then
    '            Exit While
    '        End If

    '    End While

    '    Dim ridx As Integer
    '    For Each r In templist
    '        For index = 0 To r.zeroCount - 1
    '            result.Add(0)
    '        Next
    '        If r.Value.bits > 0 Then
    '            Dim value = HuffmanWapperLib.Huffman.readBits(data, r.Value.bits, stbit)
    '            stbit += r.Value.bits
    '            result.Add(ShortVLICode.getValue(r.Value.bits, value))
    '        ElseIf r.zeroCount = 15 And result.Count < 64 Then
    '            result.Add(0)
    '        End If

    '        ridx += 1
    '    Next



    '    Return result.ToArray()
    'End Function


    'Public Shared Function writetobytes(rlclist As List(Of RLCItem)) As Byte()
    '    Dim bitwriter As New BitWriter()


    '    For Each rlc In rlclist

    '        bitwriter.writeBit(4, rlc.zeroCount)
    '        bitwriter.writeBit(4, rlc.Value.bits)
    '    Next
    '    Dim ridx As Integer
    '    For Each rlc In rlclist
    '        bitwriter.writeBit(rlc.Value.bits, rlc.Value.value)
    '        ridx = ridx + 1
    '    Next


    '    Dim result = bitwriter.ToArray()

    '    bitwriter.Close()

    '    Return result

    'End Function

    Public Shared Function WritePart2(rlclist As List(Of RLCItem)) As Byte()
        Dim bitwriter As New BitWriter()

        For Each rlc In rlclist
            bitwriter.writeBit(rlc.Value.bits, rlc.Value.value)
        Next

        Dim result = bitwriter.ToArray()
        bitwriter.Close()
        Return result

    End Function
    Public Shared Function WritePart1(rlclist As List(Of RLCItem)) As Byte()
        Dim bitwriter As New BitWriter()

        For Each rlc In rlclist

            bitwriter.writeBit(4, rlc.zeroCount)
            bitwriter.writeBit(4, rlc.Value.bits)
        Next
        
        Dim result = bitwriter.ToArray()
        bitwriter.Close()
        Return result

    End Function



    '从第一个开始编码
    Public Shared Function RLCEncode(data() As Short) As List(Of RLCItem)
        Dim result As New List(Of RLCItem)

        Dim zerocount = 0
        For i = 0 To data.Length - 1
            If data(i) = 0 Then
                zerocount += 1

                '如果有16个连续0，则用一个{15,0}表示16个连续的0
                If zerocount = 16 Then

                    Dim rlc As New RLCItem()
                    rlc.zeroCount = 15
                    rlc.Value = ShortVLICode.query(0)
                    zerocount = 0

                    result.Add(rlc)

                End If

            Else
                Dim rlc As New RLCItem()
                rlc.zeroCount = zerocount
                rlc.Value = ShortVLICode.query(data(i))
                zerocount = 0

                result.Add(rlc)

            End If


        Next

        If zerocount > 0 Then
            Dim rlc As New RLCItem()
            rlc.zeroCount = zerocount
            rlc.Value = ShortVLICode.query(0)
            zerocount = 0

            result.Add(rlc)


        End If


        Return result
    End Function


End Class
