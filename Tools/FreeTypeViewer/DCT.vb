Public Class DCT

    Public Shared Sub dctcovt(data(,) As Byte)
        Dim qt(,) As Byte = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
}



        Using ms As New System.IO.MemoryStream()

            For j = 0 To 32 - 1 Step 8
                For i = 0 To 32 - 1 Step 8
                    Dim block(7, 7) As Byte
                    For col = 0 To 7
                        For row = 0 To 7
                            block(col, row) = data(i + col, j + row)
                        Next
                    Next

                    Dim dctbyte = dct(block, 8)

                    For c = 0 To 7
                        For r = 0 To 7
                            dctbyte(c, r) /= qt(c, r)
                        Next
                    Next

                    For c = 0 To 7
                        For r = 0 To 7
                            dctbyte(c, r) *= qt(c, r)
                        Next
                    Next

                    Dim idctbyte = idct(dctbyte, 8)

                    Debug.Print("----idct----")

                    For c = 0 To 8 - 1
                        Dim line As String = ""
                        For r = 0 To 8 - 1
                            line = line & idctbyte(c, r) & vbTab
                        Next

                        Debug.Print(line)
                    Next

                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")
                    Debug.Print("")

                    For col = 0 To 7
                        For row = 0 To 7
                            data(i + col, j + row) = idctbyte(col, row)
                        Next
                    Next


                Next
            Next

        End Using

    End Sub



    Public Shared Function dct(data(,) As Byte, NUM As Integer) As Short(,)
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

        For i = 0 To NUM - 1

            Dim line As String = ""
            For j = 0 To NUM - 1
                line = line & data(i, j) & vbTab
            Next

            Debug.Print(line)

        Next

        Debug.Print("----dct----")

        For i = 0 To NUM - 1
            Dim line As String = ""
            For j = 0 To NUM - 1
                line = line & output(i, j) & vbTab
            Next

            Debug.Print(line)
        Next

        

        Debug.Print("===")

        Debug.Print("")
        Debug.Print("")


        Return output

    End Function

    Public Shared Function idct(data(,) As Short, NUM As Integer) As Byte(,)
        Dim output(NUM - 1, NUM - 1) As Byte
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

                output(i, j) = Math.Max(0, Math.Min(255, Math.Round(tmp)))

            Next

        Next


        Return output




    End Function

End Class
