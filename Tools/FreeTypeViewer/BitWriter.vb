Public Class BitWriter

    Private cache As New System.IO.MemoryStream()


    Private temp As Byte
    Private currentbits As Byte



    Public Sub writeBit(bits As Byte, value As UShort)

        For bit = bits - 1 To 0 Step -1

            Dim bitvalue As Byte = (value >> bit) And &H1

            temp = temp << 1

            temp = temp Or bitvalue


            currentbits += 1

            If currentbits = 8 Then
                cache.WriteByte(temp)
                currentbits = 0
                temp = 0
            End If

        Next


    End Sub



    Public Function ToArray() As Byte()

        If currentbits > 0 Then

            temp = temp << (8 - currentbits)

            cache.WriteByte(temp)
        End If

        Return cache.ToArray()
    End Function


    Public Sub Close()
        cache.Dispose()
    End Sub


End Class
