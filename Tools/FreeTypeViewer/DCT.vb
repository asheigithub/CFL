Public Class DCT



    Private Shared Function Zigzag22(data(,) As Short) As SByte()
        Dim Zigzagtable(,) As SByte = {
       {0, 1},
        {2, 3}
}


        Dim out(3) As SByte

        For i = 0 To 1
            For j = 0 To 1

                out(Zigzagtable(i, j)) = data(i, j)

            Next

        Next



        Return out
    End Function


    Public Shared Function dctcovt22(data(,) As Byte) As Byte()
       

        Dim qt(,) As Byte = {
    {4, 16},
    {16, 18}
}

        'For index = 0 To 1
        '    For j = 0 To 1
        '        qt(index, j) = 8
        '    Next

        'Next



        Using ms As New System.IO.MemoryStream()
            Dim bw As New System.IO.BinaryWriter(ms)
            For j = 0 To 32 - 1 Step 2
                For i = 0 To 32 - 1 Step 2
                    Dim block(1, 1) As Short
                    For col = 0 To 1
                        For row = 0 To 1
                            block(col, row) = data(i + col, j + row) - 128
                        Next
                    Next

                    Dim dctbyte = dct(block, 2)
                    
                    Debug.Print("--dct1-")
                    displayBlock(dctbyte, 2)

                    For c = 0 To 1
                        For r = 0 To 1
                            dctbyte(c, r) \= qt(c, r)
                        Next
                    Next

                    Dim zig = Zigzag22(dctbyte)
                    Dim zcount As Byte = 0
                    For index = zig.Length - 1 To 0 Step -1
                        If zig(index) <> 0 Then
                            Exit For
                        End If
                        zcount += 1
                    Next

                    For index = 0 To 4 - zcount - 1
                        bw.Write(zig(index))
                    Next


                    Debug.Print("--dctqd-")
                    displayBlock(dctbyte, 2)

                    For c = 0 To 1
                        For r = 0 To 1

                            'dctbyte(c, r) -= 64

                            dctbyte(c, r) *= qt(c, r)
                        Next
                    Next
                    'dctbyte(0, 0) = first
                    'dctbyte(0, 1) = second
                    'dctbyte(1, 0) = thrid

                    Dim idctbyte = idct(dctbyte, 2)



                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")

                    For col = 0 To 1
                        For row = 0 To 1
                            data(i + col, j + row) = Math.Min(255, Math.Max(0, idctbyte(col, row) + 128))
                        Next
                    Next


                Next
            Next
            Return ms.ToArray()
        End Using

    End Function








    Private Shared Function Zigzag(data(,) As Short) As SByte()
        Dim Zigzagtable(,) As SByte = {
        {0, 1, 5, 6},
        {2, 4, 7, 12},
        {3, 8, 11, 13},
        {9, 10, 14, 15}
}


        Dim out(15) As SByte

        For i = 0 To 3
            For j = 0 To 3

                out(Zigzagtable(i, j)) = data(i, j)

            Next

        Next



        Return out
    End Function

    Public Shared Function dctcovt(data(,) As Byte) As Byte()
        '        Dim qt(,) As Byte = {
        '    {16, 11, 10, 16, 24, 40, 51, 61},
        '    {12, 12, 14, 19, 26, 58, 60, 55},
        '    {14, 13, 16, 24, 40, 57, 69, 56},
        '    {14, 17, 22, 29, 51, 87, 80, 62},
        '    {18, 22, 37, 56, 68, 109, 103, 77},
        '    {24, 35, 55, 64, 81, 104, 113, 92},
        '    {49, 64, 78, 87, 103, 121, 120, 101},
        '    {72, 92, 95, 98, 112, 100, 103, 99}
        '}
        '        Dim qt(,) As Byte = {
        '    {5, 4, 4, 5},
        '    {4, 4, 5, 7},
        '    {4, 5, 7, 10},
        '    {5, 7, 10, 15}
        '}

       

        '        Dim qt(,) As Byte = {
        '    {4, 4, 8, 12},
        '    {4, 8, 8, 16},
        '    {8, 8, 16, 20},
        '    {12, 16, 20, 25}
        '}

        Dim qt(,) As Byte = {
    {4, 4, 4, 6},
    {4, 4, 8, 12},
    {4, 8, 12, 16},
    {6, 12, 16, 20}
}


        'For index = 0 To 3
        '    For j = 0 To 3
        '        qt(index, j) = 4
        '    Next

        'Next



        Using ms As New System.IO.MemoryStream()
            Dim bw As New System.IO.BinaryWriter(ms)
            For j = 0 To 32 - 1 Step 4
                For i = 0 To 32 - 1 Step 4
                    Dim block(3, 3) As Short
                    For col = 0 To 3
                        For row = 0 To 3
                            block(col, row) = data(i + col, j + row) - 128
                        Next
                    Next

                    Dim dctbyte = dct(block, 4)
                    'Dim first = dctbyte(0, 0)
                    'Dim second = dctbyte(0, 1)
                    'Dim thrid = dctbyte(1, 0)

                    Debug.Print("--dct1-")
                    displayBlock(dctbyte, 4)

                    For c = 0 To 3
                        For r = 0 To 3
                            dctbyte(c, r) \= qt(c, r)

                            'dctbyte(c, r) += 64

                            'If dctbyte(c, r) > 255 Then dctbyte(c, r) = 255
                            'If dctbyte(c, r) < 0 Then dctbyte(c, r) = 0
                        Next
                    Next

                    Dim zig = Zigzag(dctbyte)
                    Dim zcount As Byte = 0
                    For index = zig.Length - 1 To 0 Step -1
                        If zig(index) <> 0 Then
                            Exit For
                        End If
                        zcount += 1
                    Next


                    
                    For index = 0 To 16 - zcount - 1
                        bw.Write(zig(index))
                    Next


                    Debug.Print("--dctqd-")
                    displayBlock(dctbyte, 4)

                    For c = 0 To 3
                        For r = 0 To 3

                            'dctbyte(c, r) -= 64

                            dctbyte(c, r) *= qt(c, r)
                        Next
                    Next
                    'dctbyte(0, 0) = first
                    'dctbyte(0, 1) = second
                    'dctbyte(1, 0) = thrid

                    Dim idctbyte = idct(dctbyte, 4)



                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")

                    For col = 0 To 3
                        For row = 0 To 3
                            data(i + col, j + row) = Math.Min(255, Math.Max(0, idctbyte(col, row) + 128))
                        Next
                    Next


                Next
            Next
            Return ms.ToArray()
        End Using

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


    Public Shared Function dctcovt88(data(,) As Byte) As Byte()
        Dim qt(,) As Byte = {
    {2, 2, 2, 2, 2, 3, 4, 6},
    {2, 2, 2, 2, 3, 4, 6, 10},
    {2, 2, 2, 3, 4, 6, 10, 14},
    {2, 2, 3, 4, 6, 10, 14, 18},
    {2, 3, 4, 6, 10, 14, 18, 22},
    {3, 4, 6, 10, 14, 18, 22, 24},
    {4, 6, 10, 14, 18, 22, 24, 28},
    {6, 10, 14, 18, 22, 24, 28, 33}
}

        '        Dim qt(,) As Byte = {
        '    {16, 11, 10, 16, 24, 40, 51, 61},
        '    {12, 12, 14, 19, 26, 58, 60, 55},
        '    {14, 13, 16, 24, 40, 57, 69, 56},
        '    {14, 17, 22, 29, 51, 87, 80, 62},
        '    {18, 22, 37, 56, 68, 109, 103, 77},
        '    {24, 35, 55, 64, 81, 104, 113, 92},
        '    {49, 64, 78, 87, 103, 121, 120, 101},
        '    {72, 92, 95, 98, 112, 100, 103, 99}
        '}
        '        For index = 0 To 7
        '            For j = 0 To 7
        '                qt(index, j) \= 8
        '            Next
        '        Next


        For index = 0 To 7
            For j = 0 To 7
                qt(index, j) = 1 + (index + 1 + j + 1 - 1) * 0.7
            Next

        Next
       

        Using ms As New System.IO.MemoryStream()
            Dim bw As New System.IO.BinaryWriter(ms)
            For j = 0 To 32 - 1 Step 8
                For i = 0 To 32 - 1 Step 8
                    Dim block(7, 7) As Short
                    For col = 0 To 7
                        For row = 0 To 7
                            block(col, row) = data(i + col, j + row) - 128
                        Next
                    Next

                    Dim dctbyte = dct(block, 8)
                    'Dim first = dctbyte(0, 0)
                    'Dim second = dctbyte(0, 1)
                    'Dim thrid = dctbyte(1, 0)

                    Debug.Print("--dct1-")
                    displayBlock(dctbyte, 8)

                    For c = 0 To 7
                        For r = 0 To 7
                            dctbyte(c, r) \= qt(c, r)

                            'dctbyte(c, r) += 128

                            'If dctbyte(c, r) > 255 Then dctbyte(c, r) = 255
                            'If dctbyte(c, r) < 0 Then dctbyte(c, r) = 0
                        Next
                    Next

                    Dim zig = Zigzag88(dctbyte)
                    Dim zcount As Byte = 0
                    For index = zig.Length - 1 To 0 Step -1
                        If zig(index) <> 0 Then
                            Exit For
                        End If
                        zcount += 1
                    Next

                    For index = 0 To 64 - zcount - 1

                        Dim d = zig(index)

                        If d < -128 Or d > 127 Then
                            bw.Write(CType(d >> 8, SByte))

                            Dim m = d And &HFF

                            bw.Write(CType(m - 128, SByte))

                        Else
                            bw.Write(CType(d, SByte))
                        End If







                    Next


                    Debug.Print("--dctqd-")
                    displayBlock(dctbyte, 8)

                    For c = 0 To 7
                        For r = 0 To 7

                            'dctbyte(c, r) -= 128

                            dctbyte(c, r) *= qt(c, r)
                        Next
                    Next
                    'dctbyte(0, 0) = first
                    'dctbyte(0, 1) = second
                    'dctbyte(1, 0) = thrid

                    Dim idctbyte = idct(dctbyte, 8)



                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")

                    For col = 0 To 7
                        For row = 0 To 7
                            data(i + col, j + row) = Math.Min(Math.Max(0, idctbyte(col, row) + 128), 255)
                        Next
                    Next


                Next
            Next
            Return ms.ToArray()
        End Using

    End Function




    Public Shared Function dct(data(,) As Short, NUM As Integer) As Short(,)
        Dim output(NUM - 1, NUM - 1) As Short

        Dim ALPHA As Double
        Dim BETA As Double

        Dim u As Short = 0
        Dim v As Short = 0
        Dim i As Short = 0
        Dim j As Short = 0

        For u = 0 To NUM - 1
            For v = 0 To NUM - 1
                If u = 0 Then
                    ALPHA = Math.Sqrt(1.0 / NUM)
                Else
                    ALPHA = Math.Sqrt(2.0 / NUM)
                End If

                If v = 0 Then
                    BETA = Math.Sqrt(1.0 / NUM)
                Else
                    BETA = Math.Sqrt(2.0 / NUM)
                End If
                Dim tmp As Double = 0
                For i = 0 To NUM - 1
                    For j = 0 To NUM - 1

                        tmp += data(i, j) * Math.Cos((2 * i + 1) * u * Math.PI / (2.0 * NUM)) * Math.Cos((2 * j + 1) * v * Math.PI / (2.0 * NUM))


                    Next

                Next

                output(u, v) = Math.Round(ALPHA * BETA * tmp)

            Next
        Next


        Return output

    End Function

    Public Shared Function idct(data(,) As Short, NUM As Integer) As Short(,)
        Dim output(NUM - 1, NUM - 1) As Short
        Dim ALPHA As Double
        Dim BETA As Double
        Dim u As Short = 0
        Dim v As Short = 0
        Dim i As Short = 0
        Dim j As Short = 0

        For i = 0 To NUM - 1
            For j = 0 To NUM - 1

                Dim tmp As Double = 0
                For u = 0 To NUM - 1
                    For v = 0 To NUM - 1

                        If u = 0 Then
                            ALPHA = Math.Sqrt(1.0 / NUM)
                        Else
                            ALPHA = Math.Sqrt(2.0 / NUM)
                        End If
                        If v = 0 Then
                            BETA = Math.Sqrt(1.0 / NUM)
                        Else
                            BETA = Math.Sqrt(2.0 / NUM)
                        End If

                        tmp += ALPHA * BETA * data(u, v) * Math.Cos((2 * i + 1) * u * Math.PI / (2.0 * NUM)) * Math.Cos((2 * j + 1) * v * Math.PI / (2.0 * NUM))

                    Next

                Next

                output(i, j) = Math.Round(tmp)

            Next

        Next


        Return output




    End Function

    Private Shared Sub displayBlock(data(,) As Short, NUM As Integer)

        For i = 0 To NUM - 1

            Dim line As String = ""
            For j = 0 To NUM - 1
                line = line & data(i, j) & vbTab
            Next

            Debug.Print(line)

        Next

    End Sub
    Private Shared Sub displayBlock(data(,) As Byte, NUM As Integer)

        For i = 0 To NUM - 1

            Dim line As String = ""
            For j = 0 To NUM - 1
                line = line & data(i, j) & vbTab
            Next

            Debug.Print(line)

        Next

    End Sub

End Class
