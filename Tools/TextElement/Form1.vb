Imports System
Imports System.Globalization
Imports Microsoft.VisualBasic


Public Class Form1
    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
       
        groupunicode()
        

    End Sub



    Private Sub groupunicode()
        Dim unigroup As New List(Of Tuple(Of List(Of Integer), String))

       
        Dim str = My.Computer.FileSystem.ReadAllText("E:\CFL\Tools\TextElement\unicodeGroup.txt")
        Dim ls = str.Split(vbCrLf)




        For Each l In ls
            Dim line = l.Trim()

            If line.Length > 0 Then
                Dim sp = line.Split(" ")

                Dim range = sp(0).Split("-")

                Dim st = Convert.ToInt32(range(0), 16)
                Dim ed = Convert.ToInt32(range(1), 16)

                '0x00d800 ~ 0x00dfff

                If ed < &HD800 Or st > &HDFFF Then

                    Dim group As New List(Of Integer)

                    For index = st To ed
                        group.Add(index)
                    Next

                    unigroup.Add(New Tuple(Of List(Of Integer), String)(group, sp(sp.Length - 1)))
                End If

            End If


        Next

        Dim ccc As New List(Of Tuple(Of String, Integer(), String))

        For Each g In unigroup
            Dim base As New List(Of Integer)
            Dim ext As New List(Of Integer)

            For Each c In g.Item1

                Dim ch As String = Char.ConvertFromUtf32(c)


                Dim catage = Char.GetUnicodeCategory(ch)



                '32位不需要考虑Surrogate
                'If catage = UnicodeCategory.Surrogate _
                If _
                     catage = UnicodeCategory.EnclosingMark _
                    Or catage = UnicodeCategory.NonSpacingMark _
                    Or catage = UnicodeCategory.SpacingCombiningMark Then

                    ext.Add(c)
                ElseIf catage <> UnicodeCategory.OtherNotAssigned And catage <> UnicodeCategory.Format _
                    And catage <> UnicodeCategory.Control And catage <> UnicodeCategory.DashPunctuation Then
                    base.Add(c)
                End If


            Next

            
            If ext.Count > 0 Then

                '**两个
                For Each i In base
                    For Each j In ext

                        Dim combstr = Char.ConvertFromUtf32(i) & Char.ConvertFromUtf32(j)

                        Dim strinfo = New StringInfo(combstr)

                        If strinfo.LengthInTextElements = 1 Then

                            Dim catage = Char.GetUnicodeCategory(Char.ConvertFromUtf32(j))
                           

                            Dim l1 = testStringWidth(Char.ConvertFromUtf32(i))
                            Dim l2 = testStringWidth(Char.ConvertFromUtf32(j))
                            Dim l3 = testStringWidth(combstr)

                            If l1.Item1 > 0 And l2.Item1 > 0 And l3.Item1 > 0 And l3.Item1 < l1.Item1 * 1.5 Then

                                If l1.Item2 > 0 And l2.Item2 > 0 And l3.Item2 < l1.Item2 + l2.Item2 And l3.Item2 <> l1.Item2 Then
                                    ccc.Add(New Tuple(Of String, Integer(), String)(combstr, New Integer() {i, j}, g.Item2))
                                End If

                            End If
                        Else
                            Dim catage1 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(i))
                            Dim catage2 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(j))

                            Debug.Print(combstr & " " & catage1.ToString() & " " & catage2.ToString())
                            
                        End If

                    Next
                Next

                If g.Item2 = "泰文" Or g.Item2 = "老撾文；寮文" Then


                    '**三个**
                    For Each i In base
                        For Each j In ext
                            For Each k In ext
                                Dim combstr = Char.ConvertFromUtf32(i) & _
                                    Char.ConvertFromUtf32(j) & Char.ConvertFromUtf32(k)

                                Dim strinfo = New StringInfo(combstr)

                                If strinfo.LengthInTextElements = 1 Then

                                    Dim l1 = testStringWidth(Char.ConvertFromUtf32(i))
                                    Dim l2 = testStringWidth(Char.ConvertFromUtf32(j))
                                    Dim l3 = testStringWidth(Char.ConvertFromUtf32(k))
                                    Dim l4 = testStringWidth(combstr)

                                    If l1.Item1 > 0 And l2.Item1 > 0 And l3.Item1 > 0 And l4.Item1 > 0 And l4.Item1 < l1.Item1 * 1.5 Then

                                        If l1.Item2 > 0 And l2.Item2 > 0 And l3.Item2 > 0 And l4.Item2 < l1.Item2 + l2.Item2 + l3.Item2 And l4.Item2 <> l1.Item2 Then
                                            ccc.Add(New Tuple(Of String, Integer(), String)(combstr, New Integer() {i, j, k}, g.Item2))
                                        End If

                                    End If
                                Else
                                    'Dim catage1 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(i))
                                    'Dim catage2 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(j))
                                    'Dim catage3 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(k))

                                    'Debug.Print(combstr & " " & catage1.ToString() & " " & catage2.ToString() & catage3.ToString())


                                End If


                            Next


                        Next
                    Next
                End If

                If g.Item2 = "藏文" And False Then
                    '**四个**
                    For Each i In base
                        For Each j In ext
                            For Each k In ext
                                For Each l In ext

                                    Dim combstr = Char.ConvertFromUtf32(i) & _
                                        Char.ConvertFromUtf32(j) & Char.ConvertFromUtf32(k) & Char.ConvertFromUtf32(l)

                                    Dim strinfo = New StringInfo(combstr)

                                    If strinfo.LengthInTextElements = 1 Then

                                        Dim l1 = testStringWidth(Char.ConvertFromUtf32(i))
                                        Dim l2 = testStringWidth(Char.ConvertFromUtf32(j))
                                        Dim l3 = testStringWidth(Char.ConvertFromUtf32(k))
                                        Dim l4 = testStringWidth(Char.ConvertFromUtf32(l))
                                        Dim l5 = testStringWidth(combstr)

                                        If l1.Item1 > 0 And l2.Item1 > 0 And l3.Item1 > 0 And l4.Item1 > 0 And l5.Item1 > 0 And l5.Item1 < l1.Item1 * 1.5 Then
                                            ccc.Add(New Tuple(Of String, Integer(), String)(combstr, New Integer() {i, j, k, l}, g.Item2))
                                        End If
                                    Else

                                    End If

                                Next

                            Next


                        Next
                    Next

                End If

            End If
            If base.Count = 0 Then
                Dim all As New List(Of Integer)
                For index = 0 To 3
                    all.AddRange(unigroup(index).Item1)
                Next

                For Each i In all
                    For Each j In ext

                        Dim combstr = Char.ConvertFromUtf32(i) & Char.ConvertFromUtf32(j)

                        Dim strinfo = New StringInfo(combstr)

                        If strinfo.LengthInTextElements = 1 Then

                            Dim l1 = testStringWidth(Char.ConvertFromUtf32(i))
                            Dim l2 = testStringWidth(Char.ConvertFromUtf32(j))
                            Dim l3 = testStringWidth(combstr)

                            If l1.Item1 > 0 And l2.Item1 > 0 And l3.Item1 > 0 And l3.Item1 < l1.Item1 + l2.Item1 * 0.2 Then

                                If l1.Item2 > 0 And l2.Item2 > 0 And l3.Item2 < l1.Item2 + l2.Item2 And l3.Item2 <> l1.Item2 Then
                                    ccc.Add(New Tuple(Of String, Integer(), String)(combstr, New Integer() {i, j}, g.Item2))
                                End If

                            End If
                        Else
                            Dim catage1 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(i))
                            Dim catage2 = Char.GetUnicodeCategory(Char.ConvertFromUtf32(j))

                            Debug.Print(combstr & " " & catage1.ToString() & " " & catage2.ToString())

                        End If

                    Next
                Next

            End If


        Next

        Dim fs As New System.IO.FileStream("E:\CFL\Tools\TextElement\output.txt", IO.FileMode.Create)
        Dim fw As New System.IO.StreamWriter(fs, System.Text.Encoding.UTF8)


        For Each cc In ccc
            fw.Write(cc.Item1)
            fw.Write(vbTab)


            fw.Write(String.Format("{0} {1:X4}", Char.ConvertFromUtf32(cc.Item2(0)), cc.Item2(0)))
            For index = 1 To cc.Item2.Length - 1
                fw.Write(",")
                fw.Write(String.Format("{0} {1:X4}", Char.ConvertFromUtf32(cc.Item2(index)), cc.Item2(index)))
            Next

            fw.Write(vbTab)
            fw.Write(cc.Item3)
            fw.WriteLine()

        Next

        fw.Close()
        fs.Close()

    End Sub




    Private Sub testTEE()
        Dim unicodes As New List(Of Integer)
        For index = 0 To &HFFFF
            If index < &HD800 Or index > &HDFFF Then

                unicodes.Add(index)
            End If
        Next
        For index = &H100000 To &H10FFFF
            unicodes.Add(index)
        Next

        Dim ccc As New List(Of Tuple(Of String, Integer()))


        'Dim expect As New HashSet(Of Integer)
        'For index = &HF600 To &HF6FF
        '    expect.Add(index)

        'Next


       
        
        For i = 0 To unicodes.Count - 1
            For j = 0 To unicodes.Count - 1

                'If expect.Contains(j) Then
                '    Continue For
                'End If

                Dim str = Char.ConvertFromUtf32(unicodes(i)) & Char.ConvertFromUtf32(unicodes(j))

                Dim strinfo = New StringInfo(str)

                If strinfo.LengthInTextElements = 1 Then

                    Dim l1 = testStringWidth(Char.ConvertFromUtf32(unicodes(i)))
                    Dim l2 = testStringWidth(Char.ConvertFromUtf32(unicodes(j)))
                    Dim l3 = testStringWidth(str)

                    If l1.Item1 > 0 And l2.Item1 > 0 And l3.Item1 > 0 And l3.Item1 < l1.Item1 + l2.Item1 * 0.5 Then

                        If l1.Item2 > 0 And l2.Item2 > 0 And l3.Item2 < l1.Item2 + l2.Item2 And l3.Item2 <> l1.Item2 Then
                            ccc.Add(New Tuple(Of String, Integer())(str, New Integer() {unicodes(i), unicodes(j)}))
                        End If

                    End If

                    'ccc.Add(New Tuple(Of String, Integer())(str, New Integer() {unicodes(i), unicodes(j)}))
                    'Try
                    '    Dim l1 = g.MeasureString(Char.ConvertFromUtf32(unicodes(i)), Me.Font)
                    '    Dim l2 = g.MeasureString(Char.ConvertFromUtf32(unicodes(j)), Me.Font)

                    '    Dim l3 = g.MeasureString(str, Me.Font)

                    '    If l3.Width < l1.Width + l2.Width * 0.6 Then


                    '    End If
                    'Catch ex As Exception

                    'End Try









                End If

            Next

            'If i = 128 Then
            '    Dim dd As New Dictionary(Of Integer, Integer)

            '    For Each k In ccc
            '        If Not dd.ContainsKey(k.Item2(1)) Then
            '            dd.Add(k.Item2(1), 1)

            '        Else
            '            dd(k.Item2(1)) += 1
            '        End If

            '    Next

            '    For Each v In dd
            '        If v.Value > 80 Then
            '            expect.Add(v.Key)

            '            For index = 0 To ccc.Count - 1
            '                If ccc(index).Item2(1) = v.Key Then
            '                    ccc.RemoveAt(index)
            '                    index -= 1
            '                End If

            '                If index = ccc.Count - 1 Then
            '                    Exit For
            '                End If

            '            Next



            '        End If


            '    Next


            'End If


        Next

       



    End Sub

    Private Function testStringWidth(c As String) As Tuple(Of Integer, Integer)

        Try
            Using img As New Bitmap(32 * 6, 32)


                Using g = Graphics.FromImage(img)
                    g.Clear(Color.Transparent)
                    g.DrawString(c, TextBox1.Font, Brushes.Black, New PointF(0, 0))
                End Using

                Dim map = img.LockBits(New Rectangle(0, 0, 32 * 6, 32), Imaging.ImageLockMode.ReadWrite,
                                       Imaging.PixelFormat.Format32bppArgb)

                Dim ptr As IntPtr = map.Scan0

                Dim stride = Math.Abs(map.Stride)

                ' Declare an array to hold the bytes of the bitmap.
                ' This code is specific to a bitmap with 24 bits per pixels.
                Dim bytes As Integer = Math.Abs(map.Stride) * img.Height
                Dim rgbValues(bytes - 1) As Byte

                ' Copy the RGB values into the array.
                System.Runtime.InteropServices.Marshal.Copy(ptr, rgbValues, 0, bytes)

                img.UnlockBits(map)


                Dim min = 128
                Dim max = 0


                Dim count As Integer

                For h = 0 To img.Height - 1
                    For cc = 0 To stride - 1 Step 4

                        Dim a = rgbValues(h * stride + cc + 3)

                        If a > 0 Then

                            count += 1

                            min = Math.Min(min, cc / 4)
                            max = Math.Max(max, cc / 4)
                        End If

                    Next
                Next

                Return New Tuple(Of Integer, Integer)(Math.Max(0, max - min), count)

            End Using
        Catch ex As Exception
            Return New Tuple(Of Integer, Integer)(0, 0)
        End Try







    End Function


    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click

        'Dim strCombining As String = ChrW(&H627) & ChrW(&H655) + ChrW(&H650) &
        '       ChrW(&H64A) & ChrW(&H647) & ChrW(&H64E) & ChrW(&H627) &
        '       ChrW(&H628) & ChrW(&H64C)

        'Dim strCombining = ChrW(&H627) & ChrW(&H655) & ChrW(&H650) &
        '      ChrW(&H64A) & ChrW(&H647) & ChrW(&H64E) & ChrW(&H627) &
        '      ChrW(&H628) & ChrW(&H64C)

        Dim strCombining = ChrW(&HE2D) & ChrW(&HE31) & ChrW(&HE32) & ChrW(&HE30) '& ChrW(&HF62) & ChrW(&HF0B)

        'Dim c = Char.GetUnicodeCategory(ChrW(&HD31))


        ' Creates and initializes a TextElementEnumerator for myString.
        Dim myTEE As TextElementEnumerator = StringInfo.GetTextElementEnumerator(strCombining)


        Dim sb As New System.Text.StringBuilder

        ' Displays the values returned by ElementIndex, Current and GetTextElement.
        ' Current and GetTextElement return a string containing the entire text element. 
        Console.WriteLine("Index" + ControlChars.Tab + "Current" + ControlChars.Tab + "GetTextElement")
        myTEE.Reset()
        While myTEE.MoveNext()
            sb.AppendFormat("[{0}]:" + ControlChars.Tab + "{1} hex:{2}", myTEE.ElementIndex, myTEE.Current, ShowHexValues(myTEE.Current))
            sb.AppendLine()
        End While
        sb.Append(strCombining)
        TextBox1.Text = sb.ToString()

    End Sub


    Private Function ShowHexValues(s As String) As String
        Dim hexString As String = ""
        For Each ch In s
            hexString += String.Format("{0:X4} ", Convert.ToUInt16(ch))
        Next
        Return hexString
    End Function


    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        '建立unicode数据库
        Dim ucd = My.Computer.FileSystem.ReadAllText("E:\CFL\Tools\TextElement\UCD\UnicodeData.txt")

        Dim lines = ucd.Split(vbLf)

        Dim cinfo As New List(Of Tuple(Of String, String, Integer, Integer))(&HFFFF)
        For index = 1 To (&HFFFF)
            cinfo.Add(Nothing)
        Next

        For Each l In lines
            Dim cols = l.Split(";")
            If cols.Length > 1 Then

                Dim code = Convert.ToInt32(cols(0), 16)
                If Not code <= 65535 Then
                    Continue For

                End If

                Dim combing = cols(3)
                Dim bidi = cols(4)

                If code >= &HD800 And code <= &HDFFF Then
                    Continue For
                End If


                Dim str = Char.ConvertFromUtf32(code)

                Dim uppercode = AscW(Char.ToUpper(str))
                Dim lowwercode = AscW(Char.ToLower(str))


                Dim idx = code
               
                cinfo(idx) = New Tuple(Of String, String, Integer, Integer)(bidi, combing, uppercode, lowwercode)

            End If
        Next

        Dim csb As New Text.StringBuilder


        Dim sb As New Text.StringBuilder
        For index = 0 To cinfo.Count - 1

            Dim info = cinfo(index)

            If Not info Is Nothing Then

                Dim str = Char.ConvertFromUtf32(index)
                Dim cat = Char.GetUnicodeCategory(str)

                csb.AppendFormat("static const unsigned int uc{0}[5]= {{unicodeCategory::{1}, bidiClass::{2} , {3},{4},{5}}};", _
                                index, cat.ToString(), info.Item1, info.Item2, info.Item3, info.Item4)
                csb.AppendLine()

                sb.AppendFormat("uc{0},", index)
                sb.AppendLine()
            Else
                sb.AppendLine("blank,")
            End If

            

        Next


        Dim sb2 As New Text.StringBuilder

        csb.Clear()
        '***超过65535部分
        For Each l In lines
            Dim cols = l.Split(";")
            If cols.Length > 1 Then

                Dim code = Convert.ToInt32(cols(0), 16)
                If code < 65535 Then
                    Continue For

                End If

                Dim combing = cols(3)
                Dim bidi = cols(4)

                If code >= &HD800 And code <= &HDFFF Then
                    Continue For
                End If


                Dim str = Char.ConvertFromUtf32(code)

                Dim uppercode = AscW(Char.ToUpper(str))
                Dim lowwercode = AscW(Char.ToLower(str))


                Dim idx = code

                'cinfo(idx) = New Tuple(Of String, String, Integer, Integer)(bidi, combing, uppercode, lowwercode)

                Dim cat = Char.GetUnicodeCategory(str)

                csb.AppendFormat("static const unsigned int uc{0}[5]= {{unicodeCategory::{1}, bidiClass::{2} , {3},{4},{5}}};", _
                                code, cat.ToString(), bidi, combing, code, code)
                csb.AppendLine()

                sb2.AppendFormat("{{{0},uc{1}}},", _
                                code, code)
                sb2.AppendLine()

            End If
        Next






    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click

        Dim str = "hahaha我擦你妹eeee真见鬼了蛋疼无比汗"

        Dim bytes = System.Text.Encoding.Default.GetBytes(str)

        Dim huffman As New HuffmanWapperLib.Huffman()

        Dim c = huffman.huffmanCompress(bytes)

        Dim up = huffman.unHuffmanCompress(c.data)

        Dim de = System.Text.Encoding.Default.GetString(up)


        Dim priority = huffman.getPriority(bytes)

        Dim c2 = huffman.huffmanCompress(bytes, priority, True)

        
        Dim decodeinfo = huffman.loadDecodeInfo(c.data)

        Dim u2 = huffman.unHuffmanCompress(c2.data, decodeinfo)
        Dim de2 = System.Text.Encoding.Default.GetString(u2)



    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        Dim input = "ฬุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุุ็็็็็็็็็็็็็็็็็็็็" '"∆˚¬˚åˆøπœ¬¬ππππ´øøøøøøππø¬µµµµ˜˜˜µµµµµ††††††\"

        Dim bytes = System.Text.Encoding.UTF32.GetBytes(input)

        Dim ms As New System.IO.MemoryStream(bytes)
        Dim br As New System.IO.BinaryReader(ms)

        Dim outstr As String = ""

        While ms.Position < ms.Length
            Dim code = br.ReadInt32()

            outstr = outstr & "+" & String.Format("CFLString(0x{0:X4})", code)

        End While

        TextBox1.Text = outstr





    End Sub
End Class
