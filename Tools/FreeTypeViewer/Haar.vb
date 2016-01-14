Imports FreeTypeViewer.DCT

''' <summary>
''' Haar小波变换
''' </summary>
''' <remarks></remarks>
Public Class Haar

    Shared qt(,) As Single = {
    {1, 1, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5},
    {1, 1, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5},
    {1.5, 1.5, 2.5, 2.5, 1.5, 1.5, 4, 4},
    {1.5, 1.5, 2.5, 2.5, 1.5, 1.5, 4, 4},
    {1.5, 1.5, 1.5, 1.5, 8, 8, 8, 8},
    {1.5, 1.5, 1.5, 1.5, 8, 8, 8, 8},
    {1.5, 1.5, 4, 4, 8, 8, 8, 8},
    {1.5, 1.5, 4, 4, 8, 8, 8, 8}
}

    Public Shared Function UnHaarData(dct As DCTResult, quality As Double) As Byte(,)
        Dim result(31, 31) As Byte

        'For index = 0 To 7
        '    For j = 0 To 7
        '        qt(index, j) = 1 + (index + 1 + j + 1 - 1) * quality * 2
        '    Next
        'Next

        Dim zigs = RLCCode.loadfromDctResult(dct)

        For k = 0 To 15
            Dim data = unZigzag88(zigs(k))

            Dim block(7, 7) As Single
            For col = 0 To 7
                For row = 0 To 7
                    block(col, row) = data(col, row)

                Next
            Next

            For col = 0 To 7
                For row = 0 To 7
                    block(col, row) = block(col, row) * qt(col, row)
                Next
            Next

            For ii = 0 To 1
                For jj = 0 To 1


                    Dim h2(3, 3) As Single
                    For i = 0 To 3
                        For j = 0 To 3
                            h2(i, j) = block(ii * 4 + i, jj * 4 + j)
                        Next
                    Next
                    Dim unh2 = unhaar(h2, 4)
                    For i = 0 To 3
                        For j = 0 To 3
                            block(ii * 4 + i, jj * 4 + j) = unh2(i, j)
                        Next
                    Next

                Next
            Next


            block = unhaar(block, 8)

            Dim bx As Integer = (k Mod 4) * 8
            Dim by As Integer = (k \ 4) * 8

            For col = 0 To 7
                For row = 0 To 7
                    result(bx + col, by + row) = Math.Min(255, Math.Max(0, block(col, row) + 128))
                Next
            Next

        Next

        Return result


    End Function



    Private Shared Function unZigzag88(zig() As Short) As Short(,)

        Dim Zigzagtable(,) As Short = {
            {0, 1, 5, 6, 14, 15, 27, 28},
            {2, 4, 7, 13, 16, 26, 29, 42},
            {3, 8, 12, 17, 25, 30, 41, 43},
            {9, 11, 18, 24, 31, 40, 44, 53},
            {10, 19, 23, 32, 39, 45, 52, 54},
            {20, 22, 33, 38, 46, 51, 55, 60},
            {21, 34, 37, 47, 50, 56, 59, 61},
            {35, 36, 48, 49, 57, 58, 62, 63}
}
        Dim ret(7, 7) As Short

        For i = 0 To 7
            For j = 0 To 7
                ret(i, j) = zig(Zigzagtable(i, j))
            Next
        Next

        Return ret

    End Function



    Private Shared Function Zigzag88(data(,) As Short) As Short()
        Dim Zigzagtable(,) As Short = {
            {0, 1, 5, 6, 14, 15, 27, 28},
            {2, 4, 7, 13, 16, 26, 29, 42},
            {3, 8, 12, 17, 25, 30, 41, 43},
            {9, 11, 18, 24, 31, 40, 44, 53},
            {10, 19, 23, 32, 39, 45, 52, 54},
            {20, 22, 33, 38, 46, 51, 55, 60},
            {21, 34, 37, 47, 50, 56, 59, 61},
            {35, 36, 48, 49, 57, 58, 62, 63}
}


        Dim out(63) As Short

        For i = 0 To 7
            For j = 0 To 7

                out(Zigzagtable(i, j)) = data(i, j)

            Next

        Next



        Return out
    End Function


    Public Shared Function Haar(data(,) As Byte, quality As Double) As DCTResult
        'Dim result(31, 31) As Short

        'For index = 0 To 7
        '    For j = 0 To 7
        '        qt(index, j) = 1 + (index + 1 + j + 1 - 1) * quality * 2
        '    Next
        'Next

        Dim rlcLists As New List(Of List(Of RLCCode.RLCItem))


        For j = 0 To 32 - 1 Step 8
            For i = 0 To 32 - 1 Step 8
                Dim block(7, 7) As Single
                For col = 0 To 7
                    For row = 0 To 7
                        block(col, row) = data(i + col, j + row) - 128
                    Next
                Next

                Dim hblock = dohaar(block, 8)

                For ii = 0 To 1
                    For jj = 0 To 1

                        Dim h2data(4, 4) As Single

                        For h2i = 0 To 3
                            For h2j = 0 To 3
                                h2data(h2i, h2j) = hblock(ii * 4 + h2i, jj * 4 + h2j)
                            Next
                        Next
                        Dim h2block = dohaar(h2data, 4)
                        For h2i = 0 To 3
                            For h2j = 0 To 3
                                hblock(ii * 4 + h2i, jj * 4 + h2j) = h2block(h2i, h2j)
                            Next
                        Next


                    Next
                Next



                Debug.Print("====")
                displayBlock(hblock, 8)

                For col = 0 To 7
                    For row = 0 To 7
                        hblock(col, row) = hblock(col, row) / qt(col, row)
                    Next
                Next

                Debug.Print("== Quantization ==")
                displayBlock(hblock, 8)
                Debug.Print("")
                Debug.Print("")

                Dim result(7, 7) As Short
                For col = 0 To 7
                    For row = 0 To 7
                        result(col, row) = hblock(col, row)

                        'If col >= 4 And row >= 4 Then
                        '    result(i + col, j + row) = 128
                        'End If

                    Next
                Next

                Dim zig = Zigzag88(result)

                Dim rlclist = RLCCode.RLCEncode(zig)

                rlcLists.Add(rlclist)


            Next
        Next


        Dim res As New DCTResult

        Using ms1 As New System.IO.MemoryStream
            For i = 0 To rlcLists.Count - 1
                Dim p1 = RLCCode.WritePart1(rlcLists(i))
                'bw.Write(p1, 0, p1.Length)
                ms1.Write(p1, 0, p1.Length)
            Next
            res.ZeroAndGroup = ms1.ToArray()

        End Using

        Using ms1 As New System.IO.MemoryStream
            For i = 0 To rlcLists.Count - 1
                Dim p2 = RLCCode.WritePart2(rlcLists(i))
                'bw.Write(p2, 0, p2.Length)
                ms1.Write(p2, 0, p2.Length)
            Next
            res.VLICode = ms1.ToArray()
        End Using

        Return res


        'Return result

    End Function



    Private Shared Function dohaar(data As Single(,), size As Short) As Single(,)
        Dim tempData(size - 1) As Single
        Dim result(size - 1, size - 1) As Single

        For i = 0 To size - 1
            For j = 0 To size - 1
                result(i, j) = data(i, j)
            Next

        Next


        For i = 0 To size - 1
            For j = 0 To size / 2 - 1
                Dim temp1 = result(i, 2 * j)
                Dim temp2 = result(i, 2 * j + 1)
                tempData(j) = (temp1 + temp2) / 2
                tempData(j + size / 2) = (temp1 - temp2) / 2
            Next
            For j = 0 To size - 1
                result(i, j) = tempData(j)
            Next
        Next

        For i = 0 To size - 1
            For j = 0 To size / 2 - 1
                Dim temp1 = result(2 * j, i)
                Dim temp2 = result(2 * j + 1, i)
                tempData(j) = (temp1 + temp2) / 2
                tempData(j + size / 2) = (temp1 - temp2) / 2
            Next
            For j = 0 To size - 1
                result(j, i) = tempData(j)
            Next
        Next
        Return result
    End Function

    Private Shared Function unhaar(data As Single(,), size As Short) As Single(,)
        Dim result(size - 1, size - 1) As Single

        For i = 0 To size - 1
            For j = 0 To size - 1
                result(i, j) = data(i, j)
            Next

        Next

        Dim tempData(size - 1) As Single
        For i = 0 To size - 1
            For j = 0 To size / 2 - 1
                Dim temp1 = result(j, i)
                Dim temp2 = result(j + size / 2, i)
                tempData(2 * j) = temp1 + temp2
                tempData(2 * j + 1) = temp1 - temp2
            Next
            For j = 0 To size - 1
                result(j, i) = tempData(j)
            Next
        Next

        For i = 0 To size - 1
            For j = 0 To size / 2 - 1
                Dim temp1 = result(i, j)
                Dim temp2 = result(i, j + size / 2)
                tempData(2 * j) = temp1 + temp2
                tempData(2 * j + 1) = temp1 - temp2
            Next
            For j = 0 To size - 1
                result(i, j) = tempData(j)
            Next
        Next

        Return result
    End Function

    Private Shared Sub displayBlock(data(,) As Single, NUM As Integer)

        For i = 0 To NUM - 1

            Dim line As String = ""
            For j = 0 To NUM - 1
                line = line & CType(data(i, j), Short) & vbTab
            Next

            Debug.Print(line)

        Next

    End Sub
End Class
