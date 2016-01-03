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
        Dim glpyh As cfl.tools.freetypewapper.GlyphWapper
        SyncLock freetype
            glpyh = freetype.getGlyph(128, item.code)
        End SyncLock

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


    Public Function MakeAnSDF_old(code As ULong) As SDFImage

        Dim div As Integer = 4


        Dim glpyh As cfl.tools.freetypewapper.GlyphWapper
        SyncLock freetype
            glpyh = freetype.getGlyph(4096 / div, code)
        End SyncLock



        'Dim data(glpyh.imageheight - 1)() As Byte
        'For i = 0 To glpyh.imageheight - 1
        '    data(i) = New Byte(glpyh.imagewidth) {}
        'Next

        Dim outline As New List(Of Point)

        Dim outlinehashset As New HashSet(Of Point)

        '***先查找轮廓***
        '横向
        For j = 0 To glpyh.imageheight - 1
            Dim lastcolor As Byte = 0
            For i = 0 To glpyh.imagewidth - 1
                Dim oc = glpyh.bitmap(j * glpyh.imagewidth + i)

                If oc <> lastcolor Then
                    outlinehashset.Add(New Point(i, j))
                    lastcolor = oc

                    'data(j)(i) = 255
                End If
            Next
        Next
        '纵向
        For j = 0 To glpyh.imagewidth - 1
            Dim lastcolor As Byte = 0
            For i = 0 To glpyh.imageheight - 1
                Dim oc = glpyh.bitmap(i * glpyh.imagewidth + j)

                If oc <> lastcolor Then
                    outlinehashset.Add(New Point(j, i))
                    lastcolor = oc

                    'data(i)(j) = 255
                End If
            Next
        Next
        outline = outlinehashset.ToList()



        Dim signeddata(31, 31) As Double


        Dim col = 0
        Dim row = 0

        For py = 64 / div To 4096 / div Step 128 / div
            col = 0

            For px = 64 / div To 4096 / div Step 128 / div


                Dim fx = px + 0
                Dim fy = py + 0
                Dim c = glpyh.bitmap(fy * glpyh.imagewidth + fx)

                Dim mindis As Double = Integer.MaxValue
                For Each p In outline

                    Dim len = ((p.X - fx) * (p.X - fx) + (p.Y - fy) * (p.Y - fy))
                    If len < mindis Then
                        mindis = len
                    End If
                Next

                mindis = Math.Sqrt(mindis)
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

        'If min < -max * 8 Then
        '    min = -max * 8
        '    For i = 0 To 32 - 1
        '        For j = 0 To 32 - 1
        '            If signeddata(j, i) < min Then
        '                signeddata(j, i) = min
        '            End If
        '        Next
        '    Next
        'End If


        '实际存储的时候我是求了一下signed distance field中的最大值max和最小值min，
        '然后通过(signedDis-min)/(max-min)将signedDis映射到[0,1]，
        '并且将轮廓阈值0映射为(0-min)/(max-min)，
        '即得到了一个取值在[0,1]间的signed distance field,其轮廓阈值为(0-min)/(max-min)。


        'Dim signedoutlin As Byte = Math.Min(255, Math.Max(0, (0 - min) / (max - min) * 255))
        'Dim out As New Bitmap(32, 32)

        'For i = 0 To 32 - 1
        '    For j = 0 To 32 - 1

        '        If glpyh.bitmap.Length > 0 Then
        '            Dim signed As Byte = Math.Max(0, Math.Min(255, (signeddata(j, i) - min) / (max - min) * 255))
        '            out.SetPixel(i, j, Drawing.Color.FromArgb(255, signed, signed, signed))
        '        Else
        '            out.SetPixel(i, j, Drawing.Color.FromArgb(255, 0, 0, 0))
        '        End If


        '    Next
        'Next

        Dim resut As New SDFImage
        resut.maxDis = max
        resut.minDis = min
        resut.outlineSigned = (0 - min) / (max - min)
        resut.glyph = glpyh
        resut.code = code

        glpyh.bitmap = Nothing

        For i = 0 To 32 - 1
            For j = 0 To 32 - 1

                If max > 0 Then
                    'Dim signed As Byte = Math.Max(0, Math.Min(255, (signeddata(j, i) - min) / (max - min) * 255))


                    Dim signed As Byte = Math.Max(0, Math.Min(255, (signeddata(j, i) - min) / (max - min) * 255))
                    signed = Math.Min(255, signed)
                    resut.data(i, j) = signed
                Else
                    resut.data(i, j) = 0
                    resut.maxDis = 1024
                    resut.minDis = -1024
                    resut.outlineSigned = 0
                End If


            Next
        Next




        Return resut

        'Me.PictureBox2.Image = out


        'Dim outsize As Short = 16
        ''***还原测试
        'Dim rrr As New Bitmap(outsize, outsize)

        'For r = 0 To rrr.Height - 1
        '    For c = 0 To rrr.Width - 1
        '        Dim u As Single = r * 1.0 / (rrr.Width - 1)
        '        Dim v As Single = c * 1.0 / (rrr.Height - 1)


        '        Dim dist = sampler(u, v, out).R / 255.0 'out.GetPixel(tx, ty).R / 255.0



        '        Dim a As Double = smoothstep((0 - min - (4096.0 / 2 / outsize)) / (max - min), (0 - min + (4096.0 / 2 / outsize)) / (max - min), dist)

        '        a = Math.Pow(a, 1.0 / 2)
        '        'a = a * 1.414

        '        Dim alpha As Byte = Math.Max(0, Math.Min(255, a * 255))

        '        Dim fontcolor = Color.Black

        '        'Dim outlinedis = (0 - min) / (max - min)
        '        'If (dist - outlinedis) * 4096 > -256 * 8 And dist - outlinedis < 0 Then
        '        '    alpha = (1 - (dist - outlinedis) * 4096 / -256 / 8) * 255
        '        '    fontcolor = Drawing.Color.FromArgb(alpha, 255, 0, 0)
        '        'End If


        '        rrr.SetPixel(r, c, Drawing.Color.FromArgb(alpha, fontcolor))


        '    Next
        'Next


        'PictureBox3.Image = rrr



    End Function




    Private Function sampler(u As Double, v As Double, tex As Byte(,)) As Color

        '//double uu = tex.Width  * u;
        '// double vv = tex.Height * v;

        u = Math.Min(Math.Max(0, u), 1)
        v = Math.Min(Math.Max(0, v), 1)


        u *= 32 - 1
        v *= 32 - 1
        Dim x As Integer = Math.Floor(u)
        Dim y As Integer = Math.Floor(v)
        Dim u_ratio = u - x
        Dim v_ratio = v - y
        Dim u_opposite = 1 - u_ratio
        Dim v_opposite = 1 - v_ratio

        Dim c = tex(x, y)

        Dim cx1 = c
        If (x + 1 < 32) Then

            cx1 = tex(x + 1, y)

        End If

        Dim cy1 = c
        If (y + 1 < 32) Then

            cy1 = tex(x, y + 1)

        End If


        Dim cx1y1 = c
        If (x + 1 < 32 And y + 1 < 32) Then

            cx1y1 = tex(x + 1, y + 1)

        End If

        '//double r = (tex[x][y] * u_opposite + tex[x + 1][y] * u_ratio) * v_opposite +
        '//                (tex[x][y + 1] * u_opposite + tex[x + 1][y + 1] * u_ratio) * v_ratio;

        Dim r = (c * u_opposite + cx1 * u_ratio) * v_opposite +
                        (cy1 * u_opposite + cx1y1 * u_ratio) * v_ratio
        Dim g = (c * u_opposite + cx1 * u_ratio) * v_opposite +
                        (cy1 * u_opposite + cx1y1 * u_ratio) * v_ratio
        Dim b = (c * u_opposite + cx1 * u_ratio) * v_opposite +
                        (cy1 * u_opposite + cx1y1 * u_ratio) * v_ratio
        Dim a = (c * u_opposite + cx1 * u_ratio) * v_opposite +
                        (cy1 * u_opposite + cx1y1 * u_ratio) * v_ratio

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


    ''' <summary>
    ''' 等距图形信息
    ''' </summary>
    ''' <remarks></remarks>
    Class SDFImage
        Public glyph As cfl.tools.freetypewapper.GlyphWapper

        Public code As UInteger

        Public maxDis As Double
        Public minDis As Double
        Public outlineSigned As Double

        Public data(31, 31) As Byte

    End Class





    Private Sub btnExport_Click(sender As Object, e As EventArgs) Handles btnExport.Click
        ExportAll()
        Return
       
    End Sub

    Private Sub ExportAll()
        If lstCharCodes.Items.Count > 0 Then
            Dim trd As New System.Threading.Thread(AddressOf _doExport)

            SaveFileDialog1.Filter = "*.ff|*.ff"
            If SaveFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then


                trd.IsBackground = True

                trd.Start(SaveFileDialog1.FileName)

                Me.btnExport.Enabled = False
                Me.btnOpenFont.Enabled = False

                Me.ProgressBar1.Value = 0


            End If

        End If
    End Sub

    Private Sub updatePorgress(v As Integer)

        If InvokeRequired Then
            Me.BeginInvoke(New Action(Of Integer)(AddressOf updatePorgress), v)
        Else
            Me.ProgressBar1.Value = v
        End If

    End Sub

    Private Sub _exportEnd()

        If InvokeRequired Then
            Me.BeginInvoke(New Action(AddressOf _exportEnd))
        Else

            Me.btnExport.Enabled = True
            Me.btnOpenFont.Enabled = True


        End If


    End Sub

    Private Sub _doExport(fn As String)
        Dim info = freetype.getInfo()
        Dim codes = freetype.getAllCharCodes()

        Dim sdflist As New List(Of SDFImage)
        Dim count = 0

        For Each c In codes
            Dim sdf = MakeAnSDF(c)
            sdflist.Add(sdf)
            updatePorgress(count * 100.0 / codes.Count)
            count += 1

        Next



        Dim sdffontdatalist As New List(Of Byte())
        For index = 0 To sdflist.Count - 1

            Dim fstm As New System.IO.MemoryStream()
            Dim fbw As New System.IO.BinaryWriter(fstm)

            Dim sdf = sdflist(index)

            fbw.Write(sdf.glyph.advance_x)
            fbw.Write(sdf.glyph.advance_y)
            fbw.Write(sdf.glyph.padleft)
            fbw.Write(sdf.glyph.padtop)
            fbw.Write(sdf.glyph.pen_x)
            fbw.Write(sdf.glyph.pen_y)
            fbw.Write(sdf.glyph.clipwidth)
            fbw.Write(sdf.glyph.clipheight)
            fbw.Write(sdf.glyph.imagewidth)
            fbw.Write(sdf.glyph.imageheight)

            fbw.Write(CType(sdf.minDis, Single))
            fbw.Write(CType(sdf.maxDis, Single))

            For row = 0 To 31
                For col = 0 To 31
                    fbw.Write(sdf.data(col, row))
                Next
            Next

            Dim sdfbyte = fstm.ToArray()

            fbw.Dispose()
            fstm.Dispose()

            sdffontdatalist.Add(sdfbyte)
        Next

        Dim kernlist As New List(Of Tuple(Of UInteger, UInteger, Integer, Integer))
        If info.use_kerning Then
            For l = 0 To codes.Count - 1
                For r = 0 To codes.Count - 1
                    Dim kern = freetype.queryKerning(codes(l), codes(r))
                    If kern.det_x <> 0 Or kern.det_y <> 0 Then
                        kernlist.Add(New Tuple(Of UInteger, UInteger, Integer, Integer)(codes(l), codes(r), kern.det_x, kern.det_y))
                    End If

                Next
            Next


        End If

        Dim fontdata As New System.IO.MemoryStream()
        Dim bw As New System.IO.BinaryWriter(fontdata)
        bw.Write(info.ascender)
        bw.Write(info.descender)
        bw.Write(info.bbox_xMax)
        bw.Write(info.bbox_xMin)
        bw.Write(info.bbox_yMax)
        bw.Write(info.bbox_yMin)
        bw.Write(info.height)
        bw.Write(info.units_per_EM)


        bw.Write(sdflist.Count)

        '**写入kern表
        bw.Write(kernlist.Count)
        For index = 0 To kernlist.Count - 1
            bw.Write(kernlist(index).Item1)
            bw.Write(kernlist(index).Item2)
            bw.Write(kernlist(index).Item3)
            bw.Write(kernlist(index).Item4)
        Next



        '**写入每个字符的位置***
        Dim poslist As New List(Of Integer)
        Dim pos As Integer = fontdata.Position + sdffontdatalist.Count * 4 + sdffontdatalist.Count * 4 '每条记录占8个字节,前int是charcode,后int是位置
        For index = 0 To sdffontdatalist.Count - 1
            poslist.Add(pos)
            pos += sdffontdatalist(index).Length
        Next

        For index = 0 To poslist.Count - 1
            bw.Write(sdflist(index).code)
            bw.Write(poslist(index))
        Next

        For index = 0 To sdffontdatalist.Count - 1
            bw.Write(sdffontdatalist(index))
        Next

        Dim bytes = fontdata.ToArray()
        fontdata.Dispose()

        My.Computer.FileSystem.WriteAllBytes(fn, bytes, False)

        _exportEnd()
    End Sub



    Private Sub btnPerView_Click(sender As Object, e As EventArgs) Handles btnPerView.Click
        Dim sdfwidth As Double = 1024.0

        If lstCharCodes.SelectedIndex >= 0 Then
            Dim signed = MakeAnSDF(CType(lstCharCodes.SelectedItem, item).code)

            Dim sdfimage As New Bitmap(32, 32)
            For j = 0 To 31
                For i = 0 To 31
                    Dim c = signed.data(i, j)
                    sdfimage.SetPixel(i, j, Color.FromArgb(255, c, c, c))
                Next
            Next
            PictureBox2.Image = sdfimage




            Dim outsize As Short = Me.TrackBar1.Value
            '***还原测试
            Dim rrr As New Bitmap(outsize, outsize)

            For r = 0 To rrr.Height - 1
                For c = 0 To rrr.Width - 1
                    Dim u As Single = r * 1.0 / (rrr.Width - 1)
                    Dim v As Single = c * 1.0 / (rrr.Height - 1)


                    Dim dist = sampler(u, v, signed.data).R / 255.0 'out.GetPixel(tx, ty).R / 255.0



                    Dim a As Double = smoothstep((0 - signed.minDis - (sdfwidth / 2 / outsize)) / (signed.maxDis - signed.minDis), (0 - signed.minDis + (sdfwidth / 2 / outsize)) / (signed.maxDis - signed.minDis), dist)

                    a = Math.Pow(a, 1.0 / 1.5)
                    'a = a * 1.414

                    a = smoothstep(0.1, 0.9, a)


                    Dim alpha As Byte = Math.Max(0, Math.Min(255, a * 255))

                    Dim fontcolor = Color.Black

                    'Dim outlinedis = (0 - signed.minDis) / (signed.maxDis - signed.minDis)
                    'If (dist - outlinedis) * 1024 > -16 And dist - outlinedis < 0 Then
                    '    alpha = (1 - (dist - outlinedis) * 1024 / -16) * 255
                    '    fontcolor = Drawing.Color.FromArgb(alpha, 255, 0, 0)
                    'End If


                    rrr.SetPixel(r, c, Drawing.Color.FromArgb(alpha, fontcolor))


                Next
            Next


            PictureBox3.Image = rrr



        End If
    End Sub




    Public Function MakeAnSDF(code As ULong) As SDFImage

        Dim div As Integer = 4


        Dim glpyh As cfl.tools.freetypewapper.GlyphWapper
        SyncLock freetype
            glpyh = freetype.getGlyph(4096 / div, code)
        End SyncLock



        'Dim data(glpyh.imageheight - 1)() As Byte
        'For i = 0 To glpyh.imageheight - 1
        '    data(i) = New Byte(glpyh.imagewidth) {}
        'Next

        Dim outline As New List(Of Point)

        Dim outlinehashset As New HashSet(Of Point)

        Dim innerpoints As New List(Of Point)

        '***先查找轮廓***
        '横向
        For j = 0 To glpyh.imageheight - 1
            Dim lastcolor As Byte = 0
            For i = 0 To glpyh.imagewidth - 1
                Dim oc = glpyh.bitmap(j * glpyh.imagewidth + i)
                If oc > 128 Then
                    oc = 255
                Else
                    oc = 0
                End If

                If oc <> lastcolor Then
                    outlinehashset.Add(New Point(i, j))
                    lastcolor = oc

                    'data(j)(i) = 255
                End If

                If oc > 0 Then
                    innerpoints.Add(New Point(i, j))
                End If

            Next
        Next
        '纵向
        For j = 0 To glpyh.imagewidth - 1
            Dim lastcolor As Byte = 0
            For i = 0 To glpyh.imageheight - 1
                Dim oc = glpyh.bitmap(i * glpyh.imagewidth + j)

                If oc <> lastcolor Then
                    outlinehashset.Add(New Point(j, i))
                    lastcolor = oc

                    'data(i)(j) = 255
                End If
            Next
        Next
        outline = outlinehashset.ToList()



        Dim signeddata(31, 31) As Double


        Dim col = 0
        Dim row = 0

        For py = 64 / div To 4096 / div Step 128 / div
            col = 0

            For px = 64 / div To 4096 / div Step 128 / div


                Dim fx = px + 0
                Dim fy = py + 0
                Dim c = glpyh.bitmap(fy * glpyh.imagewidth + fx)

                Dim mindis As Double = Integer.MaxValue
                For Each p In outline

                    Dim len = ((p.X - fx) * (p.X - fx) + (p.Y - fy) * (p.Y - fy))
                    If len < mindis Then
                        mindis = len
                    End If
                Next

                mindis = Math.Sqrt(mindis)
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


        '**更新max值**
        For i = 0 To innerpoints.Count - 1 Step 1000
            Dim inn = innerpoints(i)
            Dim fx = inn.X
            Dim fy = inn.Y
            Dim c = glpyh.bitmap(fy * glpyh.imagewidth + fx)

            Dim mindis As Double = Integer.MaxValue
            For Each p In outline

                Dim len = ((p.X - fx) * (p.X - fx) + (p.Y - fy) * (p.Y - fy))
                If len < mindis Then
                    mindis = len
                End If
            Next

            mindis = Math.Sqrt(mindis)
            If mindis > max Then
                max = mindis
            End If

        Next





        'If min < -max * 8 Then
        '    min = -max * 8
        '    For i = 0 To 32 - 1
        '        For j = 0 To 32 - 1
        '            If signeddata(j, i) < min Then
        '                signeddata(j, i) = min
        '            End If
        '        Next
        '    Next
        'End If


        '实际存储的时候我是求了一下signed distance field中的最大值max和最小值min，
        '然后通过(signedDis-min)/(max-min)将signedDis映射到[0,1]，
        '并且将轮廓阈值0映射为(0-min)/(max-min)，
        '即得到了一个取值在[0,1]间的signed distance field,其轮廓阈值为(0-min)/(max-min)。


        'Dim signedoutlin As Byte = Math.Min(255, Math.Max(0, (0 - min) / (max - min) * 255))
        'Dim out As New Bitmap(32, 32)

        'For i = 0 To 32 - 1
        '    For j = 0 To 32 - 1

        '        If glpyh.bitmap.Length > 0 Then
        '            Dim signed As Byte = Math.Max(0, Math.Min(255, (signeddata(j, i) - min) / (max - min) * 255))
        '            out.SetPixel(i, j, Drawing.Color.FromArgb(255, signed, signed, signed))
        '        Else
        '            out.SetPixel(i, j, Drawing.Color.FromArgb(255, 0, 0, 0))
        '        End If


        '    Next
        'Next

        Dim resut As New SDFImage
        resut.maxDis = max
        resut.minDis = min
        resut.outlineSigned = (0 - min) / (max - min)
        resut.glyph = glpyh
        resut.code = code

        glpyh.bitmap = Nothing

        For i = 0 To 32 - 1
            For j = 0 To 32 - 1

                If max > 0 Then
                    'Dim signed As Byte = Math.Max(0, Math.Min(255, (signeddata(j, i) - min) / (max - min) * 255))


                    Dim signed As Byte = Math.Max(0, Math.Min(255, (signeddata(j, i) - min) / (max - min) * 255))
                    signed = Math.Min(255, signed * 1)
                    resut.data(i, j) = signed
                Else
                    resut.data(i, j) = 0
                    resut.maxDis = 1024
                    resut.minDis = -1024
                    resut.outlineSigned = 0
                End If


            Next
        Next




        Return resut




    End Function



End Class
