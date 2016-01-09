Public Class Compress
    Public Shared Function dxt1compress(data(,) As Byte) As Byte()

        Using ms As New System.IO.MemoryStream()

            For j = 0 To 32 - 1 Step 4
                For i = 0 To 32 - 1 Step 4
                    Dim block(3, 3) As Byte
                    For col = 0 To 3
                        For row = 0 To 3
                            block(col, row) = data(i + col, j + row)
                        Next
                    Next
                    Dim minc As Byte
                    Dim maxc As Byte


                    Dim bs = compressblock(block, minc, maxc)

                    ms.WriteByte(minc)
                    ms.WriteByte(maxc)
                    ms.Write(bs, 0, bs.Length)


                Next
            Next

            Return ms.ToArray()

        End Using

    End Function
    Private Shared Function compressblock(ByRef block(,) As Byte, ByRef minc As Byte, ByRef maxc As Byte) As Byte()

        Dim compressed(3, 3) As Byte

        Dim min As Byte = 255
        Dim max As Byte = 0
        For i = 0 To 3
            For j = 0 To 3
                min = Math.Min(block(i, j), min)
                max = Math.Max(block(i, j), max)

            Next
        Next

        Const INSET_SHIFT As Integer = 4
        Dim inset = max - min >> INSET_SHIFT

        Dim mincolor As Integer = IIf(min + inset <= 255, min + inset, 255)
        Dim maxcolor As Integer = IIf(max > inset, max - inset, 0)

        minc = mincolor
        maxc = maxcolor

        Dim colors(3) As Integer
        colors(0) = maxcolor
        colors(1) = mincolor
        colors(2) = (2 * colors(0) + 1 * colors(1)) / 3
        colors(3) = (1 * colors(0) + 2 * colors(1)) / 3

        For i = 0 To 3
            For j = 0 To 3
                Dim c0 As Integer = block(i, j)

                Dim d0 = Math.Abs(colors(0) - c0)
                Dim d1 = Math.Abs(colors(1) - c0)
                Dim d2 = Math.Abs(colors(2) - c0)
                Dim d3 = Math.Abs(colors(3) - c0)

                If d0 < d1 And d0 < d2 And d0 < d3 Then
                    block(i, j) = colors(0)
                    compressed(i, j) = 0
                ElseIf d1 < d0 And d1 < d2 And d1 < d3 Then
                    block(i, j) = colors(1)

                    compressed(i, j) = 1

                ElseIf d2 < d0 And d2 < d1 And d2 < d3 Then
                    block(i, j) = colors(2)

                    compressed(i, j) = 2

                Else
                    block(i, j) = colors(3)
                    compressed(i, j) = 3
                End If


            Next

        Next

        Dim result(3) As Byte
        For index = 0 To 3
            Dim col As Byte = 0

            col = col Or compressed(index, 0)

            col = col << 2
            col = col Or compressed(index, 1)

            col = col << 2
            col = col Or compressed(index, 2)

            col = col << 2
            col = col Or compressed(index, 3)

            result(index) = col
        Next

        Return result
    End Function

    Public Shared Function uncompress(data As Byte()) As Byte(,)

        Dim result(31, 31) As Byte

        Using ms As New System.IO.MemoryStream(data)

            Using br As New System.IO.BinaryReader(ms)

                For blockrow = 0 To 7
                    For blockcol = 0 To 7
                        '解压一个块

                        Dim minc = br.ReadByte()
                        Dim maxc = br.ReadByte()
                        Dim colors(3) As Integer
                        colors(0) = maxc
                        colors(1) = minc
                        colors(2) = (2 * colors(0) + 1 * colors(1)) / 3
                        colors(3) = (1 * colors(0) + 2 * colors(1)) / 3

                        For i = 0 To 3
                            Dim bc = br.ReadByte()

                            Dim r1idx = bc >> 6 And 3
                            Dim r2idx = bc >> 4 And 3
                            Dim r3idx = bc >> 2 And 3
                            Dim r4idx = bc And 3

                            result(blockcol * 4 + i, blockrow * 4 + 0) = colors(r1idx)
                            result(blockcol * 4 + i, blockrow * 4 + 1) = colors(r2idx)
                            result(blockcol * 4 + i, blockrow * 4 + 2) = colors(r3idx)
                            result(blockcol * 4 + i, blockrow * 4 + 3) = colors(r4idx)

                        Next
                    Next
                Next
            End Using
        End Using
        Return result
    End Function


End Class
