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

End Class
