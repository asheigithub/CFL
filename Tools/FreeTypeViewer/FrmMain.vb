Public Class FrmMain

    Dim freetype As cfl.tools.freetypewapper.FreeTypeWapperLib
    Private Sub btnOpenFont_Click(sender As Object, e As EventArgs) Handles btnOpenFont.Click
        'Me.OpenFileDialog1.InitialDirectory = System.Environment.GetEnvironmentVariable("windir") + "\fonts"
        Me.OpenFileDialog1.FileName = ""
        Me.OpenFileDialog1.Filter = "*.*|*.*"
        If Me.OpenFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
            'MsgBox(Me.OpenFileDialog1.FileName)

            freetype = New cfl.tools.freetypewapper.FreeTypeWapperLib()

            Dim open = freetype.init(My.Computer.FileSystem.ReadAllBytes(Me.OpenFileDialog1.FileName), 0)

            If Not open Then

                freetype = Nothing


                MsgBox("字体打开错误", MsgBoxStyle.Critical)

                Return

            End If


            Dim codes = freetype.getAllCharCodes()
            Me.lstCharCodes.Items.Clear()
            For Each c In codes
                Me.lstCharCodes.Items.Add(New item With {.code = c, .display = "0x" & Convert.ToString(CType(c, Long), 16)})

            Next

            Me.lstCharCodes.DisplayMember = "display"
            Me.lstCharCodes.ValueMember = "code"

            Me.StatusStrip1.Items.Clear()
            Me.StatusStrip1.Items.Add(String.Format("共有字形{0}个", codes.Count))
        End If

    End Sub

    Private Sub lstCharCodes_SelectedIndexChanged(sender As Object, e As EventArgs) Handles lstCharCodes.SelectedIndexChanged
        Dim item As item = Me.lstCharCodes.SelectedItem


        Dim glpyh = freetype.getGlyph(128, item.code)

        Dim bitmap As New Bitmap(glpyh.imageheight, glpyh.imagewidth)

        For j = 0 To bitmap.Height - 1
            For i = 0 To bitmap.Width - 1

                Dim color = glpyh.bitmap(j * bitmap.Width + i)

                bitmap.SetPixel(i, j, System.Drawing.Color.FromArgb(color, 0, 0, 0))

            Next


        Next

        Dim g As Graphics = Graphics.FromHwnd(Me.PictureBox1.Handle)
        g.Clear(Color.Silver)

        Dim info = freetype.getInfo()


        g.TranslateTransform((256 - bitmap.Width) / 2, (256 - bitmap.Height) / 2)
        g.DrawRectangle(Pens.Yellow, New Rectangle(0, 0, bitmap.Width, bitmap.Height))

        g.DrawImage(bitmap, New Point(0, 0))

        g.FillEllipse(Brushes.Red, New RectangleF(glpyh.pen_x - 1.5, glpyh.pen_y - 1.5, 3, 3))


        g.Dispose()
        bitmap.Dispose()
    End Sub

    Class item
        Public code As ULong
        Public display As String
        Public Overrides Function ToString() As String
            Return display
        End Function
    End Class


    Public Sub MakeAnSDF(code As ULong)

        Dim glpyh = freetype.getGlyph(4096, code)

        Dim data(glpyh.imageheight - 1)() As Byte
        For i = 0 To glpyh.imageheight - 1
            data(i) = New Byte(glpyh.imagewidth) {}
        Next

        Dim outline As New List(Of Point)

        '***先查找轮廓***
        For j = 0 To glpyh.imageheight - 1
            For i = 0 To glpyh.imagewidth - 1
                Dim oc = glpyh.bitmap(j * glpyh.imagewidth + i)

                If oc > 0 Then
                    For x = -1 To 1
                        For y = -1 To 1
                            Dim ii = i + x
                            Dim jj = j + y

                            If ii >= 0 And ii < glpyh.imagewidth And jj >= 0 And jj < glpyh.imageheight Then

                                Dim nc = glpyh.bitmap(jj * glpyh.imagewidth + ii)

                                If nc = 0 Then

                                    data(j)(i) = 255

                                    outline.Add(New Point(i, j))

                                    GoTo lblexitfor
                                End If

                            End If

                        Next
                    Next
lblexitfor:


                Else
                    data(j)(i) = 0
                End If



            Next
        Next

        '查看轮廓
        'Dim bitmap = New Bitmap(glpyh.imagewidth, glpyh.imageheight)
        'For j = 0 To glpyh.imageheight - 1
        '    For i = 0 To glpyh.imagewidth - 1
        '        If data(j)(i) > 0 Then

        '            bitmap.SetPixel(i, j, Color.Red)

        '        End If


        '    Next


        'Next

        'bitmap.Save("e:\sdf.png")

        Dim signeddata(32, 32) As Double


        Dim col = 0
        Dim row = 0

        For py = 64 To 4096 Step 128
            col = 0

            For px = 64 To 4096 Step 128


                Dim c = glpyh.bitmap(py * glpyh.imagewidth + px)

                Dim mindis As Double = 4096 * Math.Sqrt(2)
                For Each p In outline
                    Dim len = Math.Sqrt((p.X - px) * (p.X - px) + (p.Y - py) * (p.Y - py))
                    If len < mindis Then
                        mindis = len
                    End If
                Next


                If c > 0 Then
                    '内部

                Else
                    '外部
                    mindis = -mindis
                End If

                signeddata(row, col) = mindis

                col += 1

            Next
            row += 1
        Next


        Dim min As Double = 65535
        Dim max As Double = -65535

        For i = 0 To 32 - 1
            For j = 0 To 32 - 1
                min = Math.Min(min, signeddata(j, i))
                max = Math.Max(max, signeddata(j, i))
            Next
        Next

        '实际存储的时候我是求了一下signed distance field中的最大值max和最小值min，
        '然后通过(signedDis-min)/(max-min)将signedDis映射到[0,1]，
        '并且将轮廓阈值0映射为(0-min)/(max-min)，
        '即得到了一个取值在[0,1]间的signed distance field,其轮廓阈值为(0-min)/(max-min)。


        Dim signedoutlin As Byte = (0 - min) / (max - min) * 255
        Dim out As New Bitmap(32, 32)

        For i = 0 To 32 - 1
            For j = 0 To 32 - 1
                Dim signed As Byte = (signeddata(j, i) - min) / (max - min) * 255
                out.SetPixel(i, j, Drawing.Color.FromArgb(255, signed, signed, signed))
            Next
        Next
        Me.PictureBox2.Image = out



        '***还原测试
        Dim rrr As New Bitmap(17, 17)

        For r = 0 To rrr.Height - 1
            For c = 0 To rrr.Width - 1
                Dim u As Single = r * 1.0 / (rrr.Width - 1)
                Dim v As Single = c * 1.0 / (rrr.Height - 1)

                Dim tx As Integer = u * 31
                Dim ty As Integer = v * 31

                Dim dist = sampler(u, v, out).R / 255.0 'out.GetPixel(tx, ty).R / 255.0



                Dim alpha As Byte = smoothstep((0 - min - (4096.0 / 2 / 17)) / (max - min), (0 - min + (4096.0 / 2 / 17)) / (max - min), dist) * 255

                'alpha = 0
                'If dist > (-100 - min) / (max - min) Then
                '    alpha = 255
                'End If

                Dim fontcolor = Color.Black

                'Dim outlinedis = (0 - min) / (max - min)
                'If Math.Abs(dist - outlinedis) * 256 < 5 Then
                '    fontcolor = Drawing.Color.FromArgb(alpha, 255, 0, 0)
                'End If


                rrr.SetPixel(r, c, Drawing.Color.FromArgb(alpha, fontcolor))


            Next
        Next


        PictureBox3.Image = rrr



    End Sub




    Private Function sampler(u As Double, v As Double, tex As Bitmap) As Color

        '//double uu = tex.Width  * u;
        '// double vv = tex.Height * v;

        u = Math.Min(Math.Max(0, u), 1)
        v = Math.Min(Math.Max(0, v), 1)


        u *= tex.Width - 1
        v *= tex.Height - 1
        Dim x As Integer = Math.Floor(u)
        Dim y As Integer = Math.Floor(v)
        Dim u_ratio = u - x
        Dim v_ratio = v - y
        Dim u_opposite = 1 - u_ratio
        Dim v_opposite = 1 - v_ratio

        Dim c = tex.GetPixel(x, y)

        Dim cx1 = c
        If (x + 1 < tex.Width) Then

            cx1 = tex.GetPixel(x + 1, y)

        End If

        Dim cy1 = c
        If (y + 1 < tex.Height) Then

            cy1 = tex.GetPixel(x, y + 1)

        End If


        Dim cx1y1 = c
        If (x + 1 < tex.Width And y + 1 < tex.Height) Then

            cx1y1 = tex.GetPixel(x + 1, y + 1)

        End If

        '//double r = (tex[x][y] * u_opposite + tex[x + 1][y] * u_ratio) * v_opposite +
        '//                (tex[x][y + 1] * u_opposite + tex[x + 1][y + 1] * u_ratio) * v_ratio;

        Dim r = (c.R * u_opposite + cx1.R * u_ratio) * v_opposite +
                        (cy1.R * u_opposite + cx1y1.R * u_ratio) * v_ratio
        Dim g = (c.R * u_opposite + cx1.R * u_ratio) * v_opposite +
                        (cy1.R * u_opposite + cx1y1.R * u_ratio) * v_ratio
        Dim b = (c.R * u_opposite + cx1.R * u_ratio) * v_opposite +
                        (cy1.R * u_opposite + cx1y1.R * u_ratio) * v_ratio
        Dim a = (c.R * u_opposite + cx1.R * u_ratio) * v_opposite +
                        (cy1.R * u_opposite + cx1y1.R * u_ratio) * v_ratio

        Return Color.FromArgb(a, r, g, b)


    End Function

    Private Function clamp(v As Double, min As Double, max As Double) As Double

        Return Math.Max(Math.Min(v, max), min)
    End Function

    Private Function smoothstep(edge0 As Double, edge1 As Double, x As Double) As Single
        Dim t As Double   '/* Or genDType t; */
        t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0)
        Return t * t * (3.0 - 2.0 * t)

    End Function


    Private Sub btnExport_Click(sender As Object, e As EventArgs) Handles btnExport.Click
        If lstCharCodes.SelectedIndex >= 0 Then
            MakeAnSDF(CType(lstCharCodes.SelectedItem, item).code)

        End If
    End Sub
End Class
