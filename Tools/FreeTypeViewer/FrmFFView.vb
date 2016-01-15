Public Class FrmFFView
    Class glyph
        Inherits cfl.tools.freetypewapper.GlyphWapper

        'Public mindis As Single
        'Public maxdis As Single
        Public bytesize As Short

        Public fontdata(,) As Byte

        Public bytes() As Byte

        Public isfullhuffmandata As Boolean

    End Class


    Friend fontdir As String

    Private sdfimagesize As Integer

    Private kerninglist As New List(Of cfl.tools.freetypewapper.KerningWapper)

    Private glyphDict As New Dictionary(Of UInt32, glyph)

    Private quality As Double
    Private dis As Double
    Dim info As New cfl.tools.freetypewapper.FontInfoWapper()

    Dim decoderinfo As HuffmanWapperLib.DecodeInfoWapper


    Private Sub FrmFFView_Load(sender As Object, e As EventArgs) Handles Me.Load

        Dim bytes = My.Computer.FileSystem.ReadAllBytes(fontdir)


        Using stream As New System.IO.MemoryStream(bytes)
            Using br As New System.IO.BinaryReader(stream)
                info.ascender = br.ReadInt16()
                info.descender = br.ReadInt16()
                info.bbox_xMax = br.ReadInt32()
                info.bbox_xMin = br.ReadInt32()
                info.bbox_yMax = br.ReadInt32()
                info.bbox_yMin = br.ReadInt32()
                info.height = br.ReadInt32()
                info.units_per_EM = br.ReadInt32()
                sdfimagesize = br.ReadInt32()

                quality = br.ReadDouble()
                dis = br.ReadDouble()

                Dim glpyhs As Integer = br.ReadInt32()
                Dim kernings As Integer = br.ReadInt32()

                If kernings > 0 Then
                    info.use_kerning = True
                    For index = 0 To kernings - 1
                        br.ReadUInt32()
                        br.ReadUInt32()
                        br.ReadInt32()
                        br.ReadInt32()
                    Next
                End If

                Dim codes As New List(Of Integer)

                For i = 0 To glpyhs - 1
                    Dim glyph As New glyph

                    Dim code As UInt32 = br.ReadUInt32()
                    glyph.advance_x = br.ReadInt32()
                    glyph.advance_y = br.ReadInt32()
                    glyph.padleft = br.ReadInt32()
                    glyph.padtop = br.ReadInt32()
                    glyph.pen_x = br.ReadInt32()
                    glyph.pen_y = br.ReadInt32()
                    glyph.clipwidth = br.ReadInt32()
                    glyph.clipheight = br.ReadInt32()
                    glyph.imagewidth = br.ReadInt32()
                    glyph.imageheight = br.ReadInt32()
                    'glyph.mindis = br.ReadSingle()
                    'glyph.maxdis = br.ReadSingle()
                    glyph.bytesize = br.ReadInt16()

                    glyphDict.Add(code, glyph)
                    codes.Add(code)
                Next

               
                For i = 0 To glpyhs - 1
                    Dim glyph = glyphDict(codes(i))
                    Dim data = br.ReadBytes(glyph.bytesize)

                    glyph.bytes = data

                    If i = 0 Then
                        Dim huffman As New HuffmanWapperLib.Huffman()

                        Using fontstm As New System.IO.MemoryStream(glyph.bytes)
                            Using fontbr As New System.IO.BinaryReader(fontstm)
                                Dim huffmanlen = fontbr.ReadInt16()
                                Dim zeroandgrouplen = fontbr.ReadInt16()
                                Dim huffmandata = fontbr.ReadBytes(huffmanlen)
                                decoderinfo = huffman.loadDecodeInfo(huffmandata)

                            End Using
                        End Using

                        glyph.isfullhuffmandata = True
                    End If

                Next

                Me.lstCharCodes.Items.Clear()
                For Each c In codes
                    Me.lstCharCodes.Items.Add(New FrmMain.item With {.code = c, .display = "0x" & Convert.ToString(CType(c, Long), 16)})

                Next

                Me.lstCharCodes.DisplayMember = "display"
                Me.lstCharCodes.ValueMember = "code"


            End Using

        End Using



    End Sub

    Private Sub lstCharCodes_SelectedIndexChanged(sender As Object, e As EventArgs) Handles lstCharCodes.SelectedIndexChanged
        Dim item As FrmMain.item = Me.lstCharCodes.SelectedItem
        Dim glpyh = glyphDict(item.code)

        Dim huffman As New HuffmanWapperLib.Huffman()
        
        Using fontstm As New System.IO.MemoryStream(glpyh.bytes)
            Using fontbr As New System.IO.BinaryReader(fontstm)
                Dim huffmanlen = fontbr.ReadInt16()
                Dim zeroandgrouplen = fontbr.ReadInt16()

                Dim huffmandata = fontbr.ReadBytes(huffmanlen)

                Dim zeroandgroup() As Byte

                If glpyh.isfullhuffmandata Then

                    zeroandgroup = huffman.unHuffmanCompress(huffmandata)
                Else
                    decoderinfo.uncompressedlen = zeroandgrouplen
                    zeroandgroup = huffman.unHuffmanCompress(huffmandata, decoderinfo)
                End If


                Dim vlicode = fontbr.ReadBytes(glpyh.bytes.Length - huffmanlen - 4)

                Dim dctresult As New DCT.DCTResult()
                dctresult.VLICode = vlicode
                dctresult.ZeroAndGroup = zeroandgroup


                glpyh.fontdata = DCT.UnDct(dctresult, quality, dis)

            End Using
        End Using



        Dim outsize As Short = 128
        '***还原测试
        Dim rrr As New Bitmap(outsize, outsize)

        For r = 0 To rrr.Height - 1
            For c = 0 To rrr.Width - 1
                Dim u As Single = r * 1.0 / (rrr.Width - 1)
                Dim v As Single = c * 1.0 / (rrr.Height - 1)


                Dim dist = FrmMain.sampler(u, v, glpyh.fontdata).R / 255.0 'out.GetPixel(tx, ty).R / 255.0



                'Dim a As Double = smoothstep((0 - signed.minDis - (sdfwidth / 2 / outsize)) / (signed.maxDis - signed.minDis), (0 - signed.minDis + (sdfwidth / 2 / outsize)) / (signed.maxDis - signed.minDis), dist)

                Dim a As Double = FrmMain.smoothstep((0.5 - (4 / outsize)),
                                             (0.5 + (4 / outsize)), dist)


                'a = Math.Pow(a, 1.0 / 1.5)
                'a = a * 1.414

                'a = smoothstep(0.1, 0.9, a)


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


        




        Dim g As Graphics = Graphics.FromHwnd(Me.PictureBox1.Handle)
        g.Clear(Color.Silver)

        Dim bitmap As New Bitmap(glpyh.imageheight \ 8, glpyh.imagewidth \ 8)

        g.TranslateTransform((256 - bitmap.Width) / 2, (256 - bitmap.Height) / 2)

        g.DrawImage(rrr, New Point(0, 0))

        g.DrawRectangle(Pens.Yellow, New Rectangle(0, 0, Bitmap.Width, Bitmap.Height))

        g.DrawImage(Bitmap, New Point(0, 0))

        g.FillEllipse(Brushes.Red, New RectangleF(glpyh.pen_x \ 8 - 1.5, glpyh.pen_y \ 8 - 1.5, 3, 3))


        g.Dispose()
        Bitmap.Dispose()
    End Sub
End Class