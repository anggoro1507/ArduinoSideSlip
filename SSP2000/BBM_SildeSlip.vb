Imports System.IO
Imports System.Math
Imports System.ComponentModel
Imports System.IO.Ports
Imports System.Globalization.CultureInfo

Public Class BBM_SideSlip
    Dim myPort As Array
    Dim rs As New Resizer

    Public NamaPort As String
    Dim Inbuffer As String
    Dim LastData As String = "0000"
    Dim Code_DataRx As String
    Dim Str_ADC As String
    Dim Txt_koef_ADC As String
    Dim Txt_Settings As String
    Public Txt_Result As String
    Public Jam_Menit_Detik As String
    Public Tanggal As String
    Public Bulan As String
    Public Tahun As String
    Public File_Result As String
    Public Last_Vehicle_Number As String

    Dim t0 As Double
    Dim t1 As Double
    Dim t2 As Double
    Dim t_white As Double
    Dim t_lime As Double
    Dim Koef_ADC As Double
    Dim Cal_Real_Value As Double
    Dim Cal_Max_Real_Value As Double
    Public Value(255) As Double

    Dim j As Integer
    Dim k As Integer
    Dim n As Integer
    Dim Int_ADC As Integer
    Dim Zero_ADC As Integer
    Dim High_ADC As Integer
    Dim Low_ADC As Integer = 65535
    Dim Noise_Zero_Tolerance As Single
    Dim Last_ADC As Integer
    Dim Cal_Zero_ADC As Integer
    Dim Cal_High_ADC As Integer
    Dim Cal_Low_ADC As Integer
    Dim Cal_Real_Zero As Integer
    Dim Wait_Time_Measure As Integer
    Public Axle_Value As Integer

    Dim Get_Zero_ADC_Flag As Boolean
    Dim Zero_Potition_Flag As Boolean
    Dim Data_Recive_Flag As Boolean
    Dim High_ADC_flag As Boolean
    Dim Busy_Flag As Boolean
    Dim Show_Value_Flag As Boolean
    Dim Zero_Display_Flag As Boolean
    Dim Stable_Check_Flag As Boolean
    Dim Show_Result_Flag As Boolean
    Dim Calibration_Flag As Boolean
    Dim Delate_File_Flag As Boolean

    Const Get_ADC As String = "R"
    Const Stop_ADC As String = "C"
    Const File_Cal As String = "\SideSlipCalibration.txt"
    Const File_Set As String = "\SideSlipSettings.txt"

    Public Sub Enable_Mesure_Item()
        Dim ctrl As Control
        For Each ctrl In Me.Controls
            If ctrl.Tag = "M" Then
                ctrl.Enabled = True
                ctrl.Visible = True
            End If
        Next
        Vehicle_Number.ReadOnly = True
        Setting_Btn_Exit.PerformClick()
    End Sub

    Public Sub Disable_Mesure_Item()
        PictureBox1.Show()
        Dim ctrl As Control
        For Each ctrl In Me.Controls
            If ctrl.Tag = "M" Then
                ctrl.Enabled = False
                ctrl.Visible = False
            End If
        Next
        Label15.Hide()
        Label17.Hide()
        Cal_Btn_Exit.PerformClick()
        Vehicle_Number.ReadOnly = False
        PictureBox1.Hide()
    End Sub

    Public Sub BBM_SideSlip_Resize(sender As Object, e As EventArgs) Handles Me.Resize
        GrpBox_Set.Location = New Point(34, 249)
        rs.ResizeAllControls(Me)
    End Sub
    Public Sub BBM_SideSlip_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        Dim screen As Screen
        Try
            screen = Screen.AllScreens(1)
        Catch ex As Exception
            screen = Screen.AllScreens(0)
            MsgBox("Monitor 2 tidak terbaca. Hubungkan Monitor 2!")
        End Try
        Location = screen.Bounds.Location
        WindowState = FormWindowState.Maximized
        'panUjiKincupRoda.Visible = PengujianCartec.LToe.Visible

        rs.FindAllControls(Me)
        Me.KeyPreview = True
        Me.BackgroundImage = My.Resources.Mobil
        Disable_Mesure_Item()
        ComPort_List()
        CalibrationToolStripMenuItem.Enabled = False
        GrpBox_Cal.Visible = False
        Cal_Load()
        Set_Load()
    End Sub

    Public Sub ComBox_ComPort_Click(sender As Object, e As MouseEventArgs) Handles ComBox_ComPort.Click
        ComPort_List()
    End Sub
    Public Sub ComPort_List()
        j = 0
        Try
            myPort = SerialPort.GetPortNames
            ComBox_ComPort.Items.Clear()
            For i = 0 To UBound(myPort)
                If myPort(i).ToString = NamaPort Then j = i
                ComBox_ComPort.Items.Add(myPort(i))
            Next
            ComBox_ComPort.SelectedIndex = 0
        Catch ex As Exception
            MsgBox("Com Port Doesn't Exis", vbOKOnly, "Side Slip Tester")
            ComBox_ComPort.Items.Clear()
            ComBox_ComPort.Enabled = False

        End Try
    End Sub

    Public Sub BBM_Side_Slip_KeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles Me.KeyDown
        If e.Control And e.KeyCode = Keys.F1 Then
            CalibrationToolStripMenuItem.Enabled = True
        ElseIf e.Control And e.KeyCode = Keys.F2 Then
            SettingToolStripMenuItem.Enabled = True
        End If
    End Sub

    Public Sub Zero_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Btn_Zero.Click
        Get_Zero_ADC_Flag = True
        Cal_Real_Zero = LastData
        Cal_Max_Real_Value = 0
    End Sub

    Public Sub Btn_Print_Click(sender As Object, e As EventArgs) Handles Btn_Print.Click, PrintToolStripMenuItem.Click
        Me.Enabled = False
        Print_Form.Show()
    End Sub

    Public Sub ConnectCom()
        If Btn_Connect.Text = "CONNECT" Then
            Try
                If Bgrn_ComPort.IsOpen Then Bgrn_ComPort.Close()

                NamaPort = ComBox_ComPort.Text
                Bgrn_ComPort.PortName = NamaPort
                Bgrn_ComPort.BaudRate = 9600
                Bgrn_ComPort.Parity = IO.Ports.Parity.None
                Bgrn_ComPort.DataBits = 8
                Bgrn_ComPort.StopBits = IO.Ports.StopBits.One

                Bgrn_ComPort.Open()

                Btn_Connect.Text = "DISCONNECT"
                ComBox_ComPort.Enabled = False

                ' UI setup
                Me.BackgroundImage = My.Resources.SS0
                Txt_Value.BackColor = Color.FromArgb(64, 64, 64)
                Txt_Value.Text = "0.0"
                Txt_Axle.Text = "0"

                Enable_Mesure_Item()
                Btn_Print.Hide()
                Btn_Save.Hide()

                ' Flags dan background worker
                Get_Zero_ADC_Flag = True
                Data_Recive_Flag = False
                Busy_Flag = False
                Tick.Start()
                Tick.Enabled = True

                ' Jalankan pembacaan data
                If Not BackgroundWorker1.IsBusy Then
                    BackgroundWorker1.RunWorkerAsync()
                End If

            Catch ex As Exception
                MsgBox("Com Port tidak dapat dibuka: " & ex.Message, vbExclamation, "Side Slip Tester")
                ComBox_ComPort.Enabled = True
                If Bgrn_ComPort.IsOpen Then Bgrn_ComPort.Close()
                Hide()
                Pengujian.BKeluar.PerformClick()
            End Try
        Else
            'If Bgrn_ComPort.IsOpen = False Then Bgrn_ComPort.Open()
            'Bgrn_ComPort.DiscardInBuffer()
            'Bgrn_ComPort.DiscardOutBuffer()
            'BackgroundWorker1.CancelAsync()
            'Data_Recive_Flag = True
            'SendSerialData(Stop_ADC)
            'Waitms(200)
            'If Bgrn_ComPort.IsOpen Then Bgrn_ComPort.Close()
            'Btn_Connect.Text = "CONNECT"
            'ComBox_ComPort.Enabled = True
            'Tick.Enabled = False
            'Busy_Flag = False
            'Data_Recive_Flag = False
            'Disable_Mesure_Item()
            'Me.BackgroundImage = My.Resources.Mobil
            'Rx_Indicator.BackColor = Color.White

            Try
                ' Pastikan background worker berhenti dulu
                If BackgroundWorker1.IsBusy Then
                    BackgroundWorker1.CancelAsync()
                    Do While BackgroundWorker1.IsBusy
                        Application.DoEvents()
                        Threading.Thread.Sleep(50)
                    Loop
                End If

                ' Kirim perintah stop ke alat (jika perlu)
                If Bgrn_ComPort.IsOpen Then
                    SendSerialData(Stop_ADC)
                    Threading.Thread.Sleep(200)
                    Bgrn_ComPort.DiscardInBuffer()
                    Bgrn_ComPort.DiscardOutBuffer()
                    Bgrn_ComPort.Close()
                End If

                Btn_Connect.Text = "CONNECT"
                ComBox_ComPort.Enabled = True
                Tick.Enabled = False
                Busy_Flag = False
                Data_Recive_Flag = False
                Disable_Mesure_Item()
                Me.BackgroundImage = My.Resources.Mobil
                Rx_Indicator.BackColor = Color.White

            Catch ex As Exception
                MsgBox("Gagal memutus koneksi: " & ex.Message, vbExclamation, "Side Slip Tester")
            End Try
        End If
    End Sub

    Public Sub Connect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Btn_Connect.Click
        ConnectCom()
    End Sub

    Public Sub Cal_Btn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cal_Btn_Set.Click, Cal_Btn_Exit.Click
        Select Case CType(sender.name, String)
            Case "Cal_Btn_Set"
                If Cal_Btn_Set.Text = "SAVE" Then
                    If Cal_High_ADC - Cal_Zero_ADC = 0 Then
                        MsgBox("Wrong Data Calibration" & vbCrLf & "Please Calibration Again!", vbOKOnly, "Side Slip Tester")
                        Cal_Zero_ADC = 0
                        Cal_High_ADC = 0
                        RadioBtn_0.Checked = True
                    Else
                        Koef_ADC = Cal_High_ADC - Cal_Zero_ADC
                        Txt_koef_ADC = "*" & Koef_ADC & "Z" & Cal_Zero_ADC & "H" & Cal_High_ADC & "#"
                        Build_File(Txt_koef_ADC, File_Cal)
                    End If
                Else
                    If RadioBtn_0.Checked Then
                        Cal_Zero_ADC = Int_ADC
                        Label11.Text = Cal_Zero_ADC
                        RadioBtn_0.Checked = False
                        RadioBtn_H.Checked = True
                    ElseIf RadioBtn_H.Checked Then
                        Cal_High_ADC = Int_ADC
                        Label12.Text = Cal_High_ADC
                        RadioBtn_H.Checked = False
                        Cal_Btn_Set.Text = "SAVE"
                    End If
                End If
            Case "Cal_Btn_Exit"
                CalibrationToolStripMenuItem.Enabled = False
                Txt_Axle.Text = "0"
                Txt_Value.Text = "0.0"
                If Btn_Connect.Text = "DISCONNECT" Then
                    Btn_Connect.PerformClick()
                End If
                Txt_Value.Hide()
                Label1.Hide()
                Txt_Axle.Hide()
                Btn_Print.Hide()
                Btn_Save.Hide()
                Label15.Hide()
                Label17.Hide()
                Btn_Zero.PerformClick()
                Btn_Zero.Hide()
                GrpBox_Cal.Visible = False
                Calibration_Flag = False
        End Select
    End Sub

    Public Sub Cal_Chck(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioBtn_0.CheckedChanged, RadioBtn_H.CheckedChanged
        Select Case CType(sender.name, String)
            Case "RadioBtn_H"
                If Cal_Btn_Set.Text = "SAVE" And RadioBtn_H.Checked = True Then
                    Cal_Btn_Set.Text = "NEXT"
                End If
            Case Else
                Cal_Btn_Set.Text = "NEXT"
        End Select
    End Sub

    Public Sub Settings_Btn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Setting_Btn_Save.Click, Setting_Btn_Exit.Click
        Select Case CType(sender.name, String)
            Case "Setting_Btn_Save"
                Try
                    Noise_Zero_Tolerance = Convert.ToDecimal(Txt_Noise_Zero_Tolerance.Text.Replace(",", "."), GetCultureInfo("en-US"))
                    Wait_Time_Measure = Convert.ToDecimal(Txt_Wait_Time_Measure.Text.Replace(",", "."), GetCultureInfo("en-US"))
                    If Noise_Zero_Tolerance > 10 Or Noise_Zero_Tolerance <= 0 Or Wait_Time_Measure < 3 Then
                        Me.Show()
                        MsgBox("Wrong Data Settings" & vbCrLf & "Please Setting First!", vbOKOnly, "Side Slip Tester")
                        Noise_Zero_Tolerance = 0.3
                        Wait_Time_Measure = 3
                        SettingToolStripMenuItem.Enabled = True
                    Else
                        Txt_Settings = "*" & Noise_Zero_Tolerance.ToString("F1", GetCultureInfo("en-US")) & "T" & Wait_Time_Measure & "#"
                        Build_File(Txt_Settings, File_Set)
                    End If
                Catch ex As Exception
                    Me.Show()
                    MsgBox("Wrong Data Settings" & vbCrLf & "Please Setting First!", vbOKOnly, "Side Slip Tester")
                    Noise_Zero_Tolerance = 0.3
                    Wait_Time_Measure = 3
                    SettingToolStripMenuItem.Enabled = True
                End Try
            Case "Setting_Btn_Exit"
                SettingToolStripMenuItem.Enabled = False
                GrpBox_Set.Visible = False
        End Select
    End Sub

    Public Sub Tick_Tick(sender As Object, e As EventArgs) Handles Tick.Tick
        If Data_Recive_Flag = False Then SendSerialData(Get_ADC)
        If Busy_Flag = False And BackgroundWorker1.IsBusy = False Then BackgroundWorker1.RunWorkerAsync()
    End Sub

    Public Sub Waitms(ByVal Mili_s As Double)
        Dim t As Double = Microsoft.VisualBasic.Timer
        Do Until (Microsoft.VisualBasic.Timer - t) >= Mili_s / 1000
            Application.DoEvents()
        Loop
    End Sub

    Sub SendSerialData(ByVal data As String)
        Try
            If Bgrn_ComPort.IsOpen = False Then Bgrn_ComPort.Open()
            Bgrn_ComPort.Write(data)
            Bgrn_ComPort.WriteTimeout = 1500
        Catch ex As TimeoutException
            If Bgrn_ComPort.IsOpen = False Then Bgrn_ComPort.Open()
            Bgrn_ComPort.DiscardOutBuffer()
            Bgrn_ComPort.Close()
            BackgroundWorker1.CancelAsync()
            ComBox_ComPort.Enabled = True
            Tick.Enabled = False
            Data_Recive_Flag = True
            Busy_Flag = True
            Btn_Connect.Text = "CONNECT"
            MsgBox("Com Port Can't be Connect", vbOKOnly, "Side Slip Tester")
            Disable_Mesure_Item()
            Me.BackgroundImage = My.Resources.Mobil
            ComBox_ComPort.Enabled = True
            If Bgrn_ComPort.IsOpen Then Bgrn_ComPort.Close()
            Hide()
            Pengujian.BKeluar.PerformClick()
        End Try
    End Sub

    Public Sub BackgroundWorker1_DoWork(sender As Object, e As DoWorkEventArgs) Handles BackgroundWorker1.DoWork
        Busy_Flag = True
        Try
            Int_ADC = String2Int(ReceiveSerialData())
            If Calibration_Flag = False Then
                Measure(Int_ADC)
            End If
        Catch ex As Exception
        End Try
    End Sub

    Function ReceiveSerialData() As String
        Dim returnStr As String
        Try
            If Bgrn_ComPort.IsOpen = False Then Bgrn_ComPort.Open()
            Bgrn_ComPort.ReadTimeout = 1500
            If Rx_Indicator.BackColor = Color.Blue And Microsoft.VisualBasic.Timer - t_white > 0.5 Then
                t_lime = Microsoft.VisualBasic.Timer
                Rx_Indicator.BackColor = Color.White
            End If
            Do
                Dim Incoming As String = Bgrn_ComPort.ReadLine()
                If Incoming.LastIndexOf("*") - Incoming.IndexOf("#") = -5 Then
                    Bgrn_ComPort.DiscardInBuffer()
                    returnStr = Incoming.Substring(Incoming.LastIndexOf("*") + 1, 4)
                    LastData = returnStr
                    Data_Recive_Flag = True
                    If Rx_Indicator.BackColor = Color.White And Microsoft.VisualBasic.Timer - t_lime > 0.5 Then
                        t_white = Microsoft.VisualBasic.Timer
                        Rx_Indicator.BackColor = Color.Blue
                    End If
                    Exit Do
                Else
                    Incoming = ""
                End If
            Loop
        Catch ex As TimeoutException
            returnStr = LastData
        Finally
            If Bgrn_ComPort IsNot Nothing Then Bgrn_ComPort.Close()
        End Try
        Return returnStr
    End Function

    Function String2Int(data As String) As Integer
        Dim byteArray() As Byte
        Dim int_data As Integer
        Dim decNumbers As System.Text.StringBuilder = New System.Text.StringBuilder
        byteArray = System.Text.ASCIIEncoding.ASCII.GetBytes(data)
        For i As Integer = 0 To byteArray.Length - 1
            Select Case byteArray(i).ToString("D")
                Case 0 : decNumbers.Append("00")
                Case 1 : decNumbers.Append("01")
                Case 2 : decNumbers.Append("02")
                Case 3 : decNumbers.Append("03")
                Case 4 : decNumbers.Append("04")
                Case 5 : decNumbers.Append("05")
                Case 6 : decNumbers.Append("06")
                Case 7 : decNumbers.Append("07")
                Case 8 : decNumbers.Append("08")
                Case 9 : decNumbers.Append("09")
                Case Else
                    decNumbers.Append(byteArray(i).ToString("D"))
            End Select
        Next
        Code_DataRx = decNumbers.ToString()
        Try
            If (Code_DataRx.Length >= 8) Then
                Str_ADC = Chr(Int(Mid(Code_DataRx, 1, 2))) & Chr(Int(Mid(Code_DataRx, 3, 2))) & Chr(Int(Mid(Code_DataRx, 5, 2))) & Chr(Int(Mid(Code_DataRx, 7, 2)))
                int_data = Convert.ToInt32(Str_ADC)
            End If
        Catch ex As Exception
            int_data = Convert.ToInt32(LastData)
        End Try
        Return int_data
    End Function

    Sub Measure(Data As Integer)
        If Get_Zero_ADC_Flag Then
            For i = Axle_Value To 0 Step -1
                Value(i) = 0
            Next
            Zero_ADC = Data
            High_ADC = Data
            Low_ADC = Data
            Axle_Value = 0
            Zero_Display_Flag = True
            Zero_Potition_Flag = True
            Show_Result_Flag = False
            Show_Value_Flag = False
            Stable_Check_Flag = False
            Delate_File_Flag = False
            Get_Zero_ADC_Flag = False
        End If

        If Zero_Potition_Flag And (Noise_Zero_Tolerance * 1024 / 100) < Abs(Data - Zero_ADC) Then
            If Data > Zero_ADC Then
                High_ADC_flag = True
                Axle_Value = 1
            ElseIf Data < Zero_ADC Then
                High_ADC_flag = False
                Axle_Value = 1
            End If
            Zero_Potition_Flag = False
            Stable_Check_Flag = False
            t0 = Microsoft.VisualBasic.Timer
        End If

        If Zero_Potition_Flag = False And Stable_Check_Flag = False Then
            If High_ADC_flag Then
                If High_ADC < Data Then
                    High_ADC = Data
                    Show_Value_Flag = True
                    t0 = Microsoft.VisualBasic.Timer
                ElseIf Microsoft.VisualBasic.Timer - t0 > 2.5 Then
                    Stable_Check_Flag = True
                    t1 = Microsoft.VisualBasic.Timer
                End If
            Else
                If Low_ADC > Data Then
                    Low_ADC = Data
                    Show_Value_Flag = True
                    t0 = Microsoft.VisualBasic.Timer
                ElseIf Microsoft.VisualBasic.Timer - t0 > 2.5 Then
                    Stable_Check_Flag = True
                    t1 = Microsoft.VisualBasic.Timer
                End If
            End If
        End If

        If Microsoft.VisualBasic.Timer - t1 > Wait_Time_Measure - 3 Then
            If Stable_Check_Flag Then
                If (Noise_Zero_Tolerance * 1024 / 100) < Abs(Data - Last_ADC) Then
                    t2 = Microsoft.VisualBasic.Timer
                Else
                    If Microsoft.VisualBasic.Timer - t2 > 0.5 Then
                        Zero_ADC = Data
                        High_ADC = Data
                        Low_ADC = Data
                        Show_Result_Flag = True
                        Zero_Potition_Flag = True
                        Stable_Check_Flag = False
                    End If
                End If
            End If
        Else
            t2 = Microsoft.VisualBasic.Timer
        End If
        Last_ADC = Data
    End Sub

    Public Sub BackgroundWorker1_RunWorkerCompleted(sender As Object, e As RunWorkerCompletedEventArgs) Handles BackgroundWorker1.RunWorkerCompleted

        If Calibration_Flag Then
            Cal_Label_ADC.Text = Int_ADC
            ProgressBar1.Value = Int_ADC * 1000 / 1024
            Cal_Real_Value = (Int_ADC - Cal_Real_Zero) / Koef_ADC * 10
            If Abs(Cal_Max_Real_Value) + (Noise_Zero_Tolerance / Koef_ADC * 100) < Abs(Cal_Real_Value) Then
                If Abs(Cal_Real_Value) > Abs(Cal_Max_Real_Value) Then
                    Cal_Max_Real_Value = Cal_Real_Value
                End If
            End If
            Txt_Value.Text = Cal_Max_Real_Value.ToString("F1", GetCultureInfo("en-US"))
            Cal_Label_Real_Value.Text = Cal_Real_Value.ToString("F1", GetCultureInfo("en-US"))
            Txt_Axle.Text = Cal_Label_Real_Value.Text
        End If

        If Zero_Display_Flag Then
            Txt_Value.BackColor = Color.FromArgb(64, 64, 64)
            Txt_Value.Text = Nothing
            Txt_Axle.Text = Nothing
            Txt_Axle.Text = "0"
            Txt_Value.Text = "0.0"
            Zero_Display_Flag = False
        End If

        If Show_Result_Flag Then
            Txt_Axle.Text = Axle_Value
            Txt_Value.Text = Value(Axle_Value).ToString("F1", GetCultureInfo("en-US"))
            If Val(Txt_Value.Text) < 0 Then
                Me.BackgroundImage = My.Resources.SS_M
            ElseIf Val(Txt_Value.Text) >= 0 Then
                Me.BackgroundImage = My.Resources.SS_P
            End If
            Enable_Mesure_Item()
            Try
                If Val(Txt_Value.Text) < -5 Or Val(Txt_Value.Text) > 5 Then
                    Txt_Value.BackColor = Color.Red
                Else
                    Txt_Value.BackColor = Color.Green
                End If
            Catch ex As Exception
                Txt_Value.BackColor = Color.Red
            End Try
            Show_Result_Flag = False
            Pengujian.TSideSlip.Text = Txt_Value.Text.Replace(".", ",")
            Pengujian.CBHasilUjiSideSlip.Enabled = True
            Pengujian.TCatatan.Enabled = True
            Pengujian.BSimpan.Enabled = True
            Pengujian.BCetak.Enabled = True
        End If

        If Show_Value_Flag Then
            'Txt_Value.BackColor = Color.FromArgb(64, 64, 64)
            If High_ADC_flag Then
                Value(Axle_Value) = (High_ADC - Zero_ADC) / Koef_ADC * 10
            Else
                Value(Axle_Value) = (Low_ADC - Zero_ADC) / Koef_ADC * 10
            End If
            'Txt_Axle.Text = Axle_Value
            'Txt_Value.Text = Value(Axle_Value).ToString("F1", GetCultureInfo("en-US"))
            Show_Value_Flag = False
        End If

        Busy_Flag = False
    End Sub

    Public Sub SaveToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles SaveToolStripMenuItem.Click, Btn_Save.Click
        File_Result = "\Save\HasilUjiSideSlip" & ".txt" 'Now.ToString("_yyyyMMdd", GetCultureInfo("en-US")) & ".txt"
        Dim sLine As String = ""
        If FileIO.FileSystem.FileExists(My.Application.Info.DirectoryPath & File_Result) Then
            Dim objReader As New StreamReader(My.Application.Info.DirectoryPath & File_Result)
            Dim lines As Integer = IO.File.ReadAllLines(My.Application.Info.DirectoryPath & File_Result).Length
            sLine = objReader.ReadToEnd
            Txt_Result = ""
            objReader.Close()
        End If

        If Axle_Value > 0 Then
            If Delate_File_Flag Then
                Delete_File(File_Result)
            End If
            Delate_File_Flag = True
            If Btn_Connect.Text = "CONNECT" Then
                Txt_Result = "Vehicle Number: " & Last_Vehicle_Number.Replace(" ", "") & Chr(10)
            Else
                Last_Vehicle_Number = Vehicle_Number.Text
                Txt_Result &= "Vehicle Number: " & Vehicle_Number.Text.Replace(" ", "") & Chr(10)
            End If
            Txt_Result &= "Testing at: " & Now.ToString("dddd, dd/MM/yyyy HH:mm:ss", GetCultureInfo("en-US")) & Chr(10)
            For i = 1 To Axle_Value
                Txt_Result &= "Axel " & i & ": " & Value(i).ToString("F1", GetCultureInfo("en-US")) & "mm/m"
                If Val(Txt_Value.Text) < -5.0 Or Val(Txt_Value.Text) > 5.0 Then
                    Txt_Result &= " - FAIL"
                Else
                    Txt_Result &= " - PASS"
                End If
                Txt_Result &= Chr(10)
            Next
            Txt_Result &= "END" & Chr(10) & Chr(10)
            Tahun = Now.ToString("yyyy", GetCultureInfo("en-US"))
            Bulan = Now.ToString("MM", GetCultureInfo("en-US"))
            Tanggal = Now.ToString("dd", GetCultureInfo("en-US"))
            Jam_Menit_Detik = Now.ToString("HH.mm.ss", GetCultureInfo("en-US"))
            'File_Result = "\Save\" & Tahun & "\" & Bulan & "\" & Tanggal & "\"
            Directory.CreateDirectory(My.Application.Info.DirectoryPath & "\Save\")
            'If Btn_Connect.Text = "CONNECT" Then
            '    File_Result &= Last_Vehicle_Number.Replace(" ", "") & ".txt"
            'Else
            '    File_Result &= Vehicle_Number.Text.Replace(" ", "") & ".txt"
            'End If
            If Btn_Connect.Text = "DISCONNECT" Then Btn_Connect.PerformClick()
            Txt_Result &= sLine
            Build_File(Txt_Result, File_Result)
        Else
            'MsgBox("No Saved Data!", vbOKOnly, "Side Slip Tester")
        End If
    End Sub

    Public Sub CalibrationToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles CalibrationToolStripMenuItem.Click
        If Btn_Connect.Text = "DISCONNECT" Then
            Me.BackgroundImage = My.Resources.SS0
            Cal_Load()
            Txt_Value.BackColor = Color.FromArgb(64, 64, 64)
            GrpBox_Cal.Visible = True
            RadioBtn_0.Checked = True
            Label11.Text = Cal_Zero_ADC
            Label12.Text = Cal_High_ADC
            Cal_Real_Zero = LastData
            Calibration_Flag = True
            Enable_Mesure_Item()
            Btn_Zero.Show()
            Label15.Show()
            Label17.Show()
            Btn_Save.Hide()
            Btn_Print.Hide()
        Else
            MsgBox("No Com Port Connection", vbOKOnly, "Side Slip Tester")
        End If
    End Sub

    Public Sub SettingToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles SettingToolStripMenuItem.Click
        If Btn_Connect.Text = "CONNECT" Then
            GrpBox_Set.Location = New Point(34, 249)
            Txt_Noise_Zero_Tolerance.Text = Noise_Zero_Tolerance.ToString("F1", GetCultureInfo("en-US"))
            Txt_Wait_Time_Measure.Text = Wait_Time_Measure.ToString("F0", GetCultureInfo("en-US"))
            Set_Load()
            GrpBox_Set.Visible = True
        Else
            MsgBox("Please Disconnect Port Connection!", vbOKOnly, "Side Slip Tester")
        End If
    End Sub

    Public Sub Build_File(ByVal Hasil As String, ByVal NamaFile As String)
        Dim objStreamWriter As StreamWriter
        objStreamWriter = New StreamWriter(My.Application.Info.DirectoryPath & NamaFile)
        objStreamWriter.Write(Hasil)
        objStreamWriter.Close()
        'MsgBox("Data Saved!", vbOKOnly, "Side Slip Tester")
    End Sub

    Public Sub Delete_File(ByVal NamaFile As String)
        If FileIO.FileSystem.FileExists(My.Application.Info.DirectoryPath & NamaFile) Then
            File.Delete(My.Application.Info.DirectoryPath & NamaFile)
        End If
    End Sub

    Public Sub Cal_Load()
        Try
            If FileIO.FileSystem.FileExists(My.Application.Info.DirectoryPath & File_Cal) Then
                Dim objReader As New StreamReader(My.Application.Info.DirectoryPath & File_Cal)
                Dim sLine As String = ""
                sLine = sLine + objReader.ReadLine
                objReader.Close()
                Koef_ADC = Convert.ToDecimal(sLine.Substring(sLine.IndexOf("*") + 1, sLine.IndexOf("Z") - sLine.IndexOf("*") - 1), GetCultureInfo("en-US"))
                Cal_Zero_ADC = Convert.ToDecimal(sLine.Substring(sLine.IndexOf("Z") + 1, sLine.IndexOf("H") - sLine.IndexOf("Z") - 1), GetCultureInfo("en-US"))
                Cal_High_ADC = Convert.ToDecimal(sLine.Substring(sLine.IndexOf("H") + 1, sLine.IndexOf("#") - sLine.IndexOf("H") - 1), GetCultureInfo("en-US"))
            End If
            If Koef_ADC = 0 Then
                Me.Show()
                MsgBox("Wrong Data Calibration" & vbCrLf & "Please Calibration First!", vbOKOnly, "Side Slip Tester")
                Koef_ADC = 1024
                Cal_Zero_ADC = 0
                Cal_High_ADC = 0
                Cal_Low_ADC = 0
                CalibrationToolStripMenuItem.Enabled = True
            End If
        Catch ex As Exception
            Me.Show()
            MsgBox("Wrong Data Calibration" & vbCrLf & "Please Calibration First!", vbOKOnly, "Side Slip Tester")
            Koef_ADC = 1024
            Cal_Zero_ADC = 0
            Cal_High_ADC = 0
            Cal_Low_ADC = 0
            CalibrationToolStripMenuItem.Enabled = True
        End Try
    End Sub

    Public Sub Set_Load()
        Try
            If FileIO.FileSystem.FileExists(My.Application.Info.DirectoryPath & File_Set) Then
                Dim objReader As New StreamReader(My.Application.Info.DirectoryPath & File_Set)
                Dim sLine As String = ""
                sLine = sLine + objReader.ReadLine
                objReader.Close()
                Noise_Zero_Tolerance = Convert.ToDecimal(sLine.Substring(sLine.IndexOf("*") + 1, sLine.IndexOf("T") - sLine.IndexOf("*") - 1), GetCultureInfo("en-US"))
                Wait_Time_Measure = Convert.ToDecimal(sLine.Substring(sLine.IndexOf("T") + 1, sLine.IndexOf("#") - sLine.IndexOf("T") - 1), GetCultureInfo("en-US"))
            End If
            If Noise_Zero_Tolerance > 10 Or Noise_Zero_Tolerance <= 0 Or Wait_Time_Measure < 3 Then
                Me.Show()
                MsgBox("Wrong Data Settings" & vbCrLf & "Please Setting First!", vbOKOnly, "Side Slip Tester")
                Noise_Zero_Tolerance = 0.3
                Wait_Time_Measure = 3
                SettingToolStripMenuItem.Enabled = True
            End If
        Catch ex As Exception
            Me.Show()
            MsgBox("Wrong Data Settings" & vbCrLf & "Please Setting First!", vbOKOnly, "Side Slip Tester")
            Noise_Zero_Tolerance = 0.3
            Wait_Time_Measure = 3
            SettingToolStripMenuItem.Enabled = True
        End Try
    End Sub

    Public Sub ExitToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles ExitToolStripMenuItem.Click
        MyBase.Close()
    End Sub

    Public Sub BBM_SideSlip_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        If Btn_Connect.Text = "DISCONNECT" Then
            If MessageBox.Show("Side Slip Tester is Connected" & Environment.NewLine & "Do you want to close?", "Side Slip Tester", MessageBoxButtons.YesNo) = DialogResult.Yes Then
                Btn_Connect.PerformClick()
            Else
                e.Cancel = True
            End If
        Else
            End
        End If
    End Sub

End Class

Public Class Resizer

    '----------------------------------------------------------
    ' ControlInfo
    ' Structure of original state of all processed controls
    '----------------------------------------------------------
    Public Structure ControlInfo
        Public name As String
        Public parentName As String
        Public leftOffsetPercent As Double
        Public topOffsetPercent As Double
        Public heightPercent As Double
        Public originalHeight As Integer
        Public originalWidth As Integer
        Public widthPercent As Double
        Public originalFontSize As Single
    End Structure

    '-------------------------------------------------------------------------
    ' ctrlDict
    ' Dictionary of (control name, control info) for all processed controls
    '-------------------------------------------------------------------------
    Public ctrlDict As Dictionary(Of String, ControlInfo) = New Dictionary(Of String, ControlInfo)

    '----------------------------------------------------------------------------------------
    ' FindAllControls
    ' Recursive function to process all controls contained in the initially passed
    ' control container and store it in the Control dictionary
    '----------------------------------------------------------------------------------------
    Public Sub FindAllControls(thisCtrl As Control)

        '-- If the current control has a parent, store all original relative position
        '-- and size information in the dictionary.
        '-- Recursively call FindAllControls for each control contained in the
        '-- current Control
        For Each ctl As Control In thisCtrl.Controls
            Try
                If Not IsNothing(ctl.Parent) Then
                    Dim parentHeight = ctl.Parent.Height
                    Dim parentWidth = ctl.Parent.Width

                    Dim c As ControlInfo
                    c.name = ctl.Name
                    c.parentName = ctl.Parent.Name
                    c.topOffsetPercent = Convert.ToDouble(ctl.Top) / Convert.ToDouble(parentHeight)
                    c.leftOffsetPercent = Convert.ToDouble(ctl.Left) / Convert.ToDouble(parentWidth)
                    c.heightPercent = Convert.ToDouble(ctl.Height) / Convert.ToDouble(parentHeight)
                    c.widthPercent = Convert.ToDouble(ctl.Width) / Convert.ToDouble(parentWidth)
                    c.originalFontSize = ctl.Font.Size
                    c.originalHeight = ctl.Height
                    c.originalWidth = ctl.Width
                    ctrlDict.Add(c.name, c)
                End If

            Catch ex As Exception
                Debug.Print(ex.Message)
            End Try

            If ctl.Controls.Count > 0 Then
                FindAllControls(ctl)
            End If

        Next '-- For Each

    End Sub

    '----------------------------------------------------------------------------------------
    ' ResizeAllControls
    ' Recursive function to resize and reposition all controls contained in the Control
    ' dictionary
    '----------------------------------------------------------------------------------------
    Public Sub ResizeAllControls(thisCtrl As Control)

        Dim fontRatioW As Single
        Dim fontRatioH As Single
        Dim fontRatio As Single
        Dim f As Font

        '-- Resize and reposition all controls in the passed control
        For Each ctl As Control In thisCtrl.Controls
            Try
                If Not IsNothing(ctl.Parent) Then
                    Dim parentHeight = ctl.Parent.Height
                    Dim parentWidth = ctl.Parent.Width

                    Dim c As New ControlInfo

                    Dim ret As Boolean = False
                    Try
                        '-- Get the current control's info from the control info dictionary
                        ret = ctrlDict.TryGetValue(ctl.Name, c)

                        '-- If found, adjust the current control based on control relative
                        '-- size and position information stored in the dictionary
                        If (ret) Then
                            '-- Size
                            ctl.Width = Int(parentWidth * c.widthPercent)
                            ctl.Height = Int(parentHeight * c.heightPercent)

                            '-- Position
                            ctl.Top = Int(parentHeight * c.topOffsetPercent)
                            ctl.Left = Int(parentWidth * c.leftOffsetPercent)

                            '-- Font
                            f = ctl.Font
                            fontRatioW = ctl.Width / c.originalWidth
                            fontRatioH = ctl.Height / c.originalHeight
                            fontRatio = (fontRatioW +
                            fontRatioH) / 2 '-- average change in control Height and Width
                            ctl.Font = New Font(f.FontFamily,
                            c.originalFontSize * fontRatio, f.Style)

                        End If
                    Catch
                    End Try
                End If
            Catch ex As Exception
            End Try

            '-- Recursive call for controls contained in the current control
            If ctl.Controls.Count > 0 Then
                ResizeAllControls(ctl)
            End If

        Next '-- For Each
    End Sub

End Class