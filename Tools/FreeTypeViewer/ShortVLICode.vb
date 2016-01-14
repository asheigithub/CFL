Public Class ShortVLICode

    Public Structure VLI
        ''' <summary>
        ''' 占用位数
        ''' </summary>
        ''' <remarks></remarks>
        Public bits As Byte
        ''' <summary>
        ''' 编码值
        ''' </summary>
        ''' <remarks></remarks>
        Public value As Short

    End Structure


    Public Class VLITableRow
        Public rowno As Integer
        Public low1 As Short
        Public high1 As Short

        Public low2 As Short
        Public high2 As Short

    End Class

    Public Class VLItable
        Public rows(15) As VLITableRow

        Public Sub New()
            For index = 0 To 15
                rows(index) = New VLITableRow()
                rows(index).rowno = index
                If index = 0 Then
                    rows(0).low1 = 0
                    rows(0).high1 = 0
                    rows(0).low2 = 0
                    rows(0).high2 = 0
                End If
            Next

            Dim nums = 1

            For i = 1 To 15
                Dim last = rows(i - 1)
                Dim row = rows(i)

                row.high1 = last.low1 - 1
                row.low1 = last.low1 - nums
                row.low2 = last.high2 + 1
                row.high2 = last.high2 + nums

                nums = nums * 2
            Next




        End Sub


    End Class

    Shared table As New VLItable()

    Public Shared Function getValue(row As Integer, idx As Integer) As Short
        If row = 0 Then
            Return 0
        End If

        Dim r = table.rows(row)

        If idx <= r.high1 - r.low1 Then
            Return r.low1 + idx
        Else
            Return r.low2 + idx - 1 - (r.high1 - r.low1)
        End If


    End Function

    Public Shared Function query(value As Short) As VLI
        For Each r In table.rows

            If value >= r.low1 And value <= r.high1 Then
                Dim result As New VLI
                result.bits = r.rowno
                result.value = value - r.low1
                Return result
            ElseIf value >= r.low2 And value <= r.high2 Then
                Dim result As New VLI
                result.bits = r.rowno
                result.value = value - r.low2 + (r.high1 - r.low1 + 1)
                Return result

            End If


        Next
        Throw New IndexOutOfRangeException()

    End Function


End Class
