<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class BBM_SideSlip
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(BBM_SideSlip))
        Me.BackgroundWorker1 = New System.ComponentModel.BackgroundWorker()
        Me.Tick = New System.Windows.Forms.Timer(Me.components)
        Me.Bgrn_ComPort = New System.IO.Ports.SerialPort(Me.components)
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.MenuToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.CalibrationToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.SettingToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.SaveToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.PrintToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.ExitToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Btn_Print = New System.Windows.Forms.Button()
        Me.Btn_Connect = New System.Windows.Forms.Button()
        Me.ComBox_ComPort = New System.Windows.Forms.ComboBox()
        Me.Btn_Zero = New System.Windows.Forms.Button()
        Me.Txt_Axle = New System.Windows.Forms.TextBox()
        Me.GrpBox_Cal = New System.Windows.Forms.GroupBox()
        Me.Cal_Btn_Exit = New System.Windows.Forms.Button()
        Me.Cal_Btn_Set = New System.Windows.Forms.Button()
        Me.Label16 = New System.Windows.Forms.Label()
        Me.Label14 = New System.Windows.Forms.Label()
        Me.Cal_Label_Real_Value = New System.Windows.Forms.Label()
        Me.Cal_Label_ADC = New System.Windows.Forms.Label()
        Me.Label12 = New System.Windows.Forms.Label()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.RadioBtn_H = New System.Windows.Forms.RadioButton()
        Me.RadioBtn_0 = New System.Windows.Forms.RadioButton()
        Me.ProgressBar1 = New System.Windows.Forms.ProgressBar()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Txt_Value = New System.Windows.Forms.TextBox()
        Me.Btn_Save = New System.Windows.Forms.Button()
        Me.Label15 = New System.Windows.Forms.Label()
        Me.Label17 = New System.Windows.Forms.Label()
        Me.GrpBox_Number = New System.Windows.Forms.GroupBox()
        Me.Rx_Indicator = New System.Windows.Forms.Button()
        Me.Vehicle_Number = New System.Windows.Forms.TextBox()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.GrpBox_Set = New System.Windows.Forms.GroupBox()
        Me.Setting_Btn_Exit = New System.Windows.Forms.Button()
        Me.Setting_Btn_Save = New System.Windows.Forms.Button()
        Me.Txt_Wait_Time_Measure = New System.Windows.Forms.TextBox()
        Me.Txt_Noise_Zero_Tolerance = New System.Windows.Forms.TextBox()
        Me.Label20 = New System.Windows.Forms.Label()
        Me.Label18 = New System.Windows.Forms.Label()
        Me.Label19 = New System.Windows.Forms.Label()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.MenuStrip1.SuspendLayout()
        Me.GrpBox_Cal.SuspendLayout()
        Me.GrpBox_Number.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GrpBox_Set.SuspendLayout()
        Me.SuspendLayout()
        '
        'BackgroundWorker1
        '
        Me.BackgroundWorker1.WorkerReportsProgress = True
        Me.BackgroundWorker1.WorkerSupportsCancellation = True
        '
        'Tick
        '
        Me.Tick.Interval = 70
        '
        'Bgrn_ComPort
        '
        Me.Bgrn_ComPort.ReadTimeout = 1000
        Me.Bgrn_ComPort.RtsEnable = True
        Me.Bgrn_ComPort.WriteTimeout = 1000
        '
        'MenuStrip1
        '
        Me.MenuStrip1.BackColor = System.Drawing.Color.White
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.MenuToolStripMenuItem})
        resources.ApplyResources(Me.MenuStrip1, "MenuStrip1")
        Me.MenuStrip1.Name = "MenuStrip1"
        '
        'MenuToolStripMenuItem
        '
        Me.MenuToolStripMenuItem.BackColor = System.Drawing.Color.White
        Me.MenuToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.CalibrationToolStripMenuItem, Me.SettingToolStripMenuItem, Me.SaveToolStripMenuItem, Me.PrintToolStripMenuItem, Me.ExitToolStripMenuItem})
        Me.MenuToolStripMenuItem.Name = "MenuToolStripMenuItem"
        resources.ApplyResources(Me.MenuToolStripMenuItem, "MenuToolStripMenuItem")
        '
        'CalibrationToolStripMenuItem
        '
        resources.ApplyResources(Me.CalibrationToolStripMenuItem, "CalibrationToolStripMenuItem")
        Me.CalibrationToolStripMenuItem.Name = "CalibrationToolStripMenuItem"
        '
        'SettingToolStripMenuItem
        '
        resources.ApplyResources(Me.SettingToolStripMenuItem, "SettingToolStripMenuItem")
        Me.SettingToolStripMenuItem.Name = "SettingToolStripMenuItem"
        '
        'SaveToolStripMenuItem
        '
        Me.SaveToolStripMenuItem.Name = "SaveToolStripMenuItem"
        resources.ApplyResources(Me.SaveToolStripMenuItem, "SaveToolStripMenuItem")
        '
        'PrintToolStripMenuItem
        '
        Me.PrintToolStripMenuItem.Name = "PrintToolStripMenuItem"
        resources.ApplyResources(Me.PrintToolStripMenuItem, "PrintToolStripMenuItem")
        '
        'ExitToolStripMenuItem
        '
        Me.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem"
        resources.ApplyResources(Me.ExitToolStripMenuItem, "ExitToolStripMenuItem")
        '
        'Btn_Print
        '
        resources.ApplyResources(Me.Btn_Print, "Btn_Print")
        Me.Btn_Print.Name = "Btn_Print"
        Me.Btn_Print.Tag = ""
        Me.Btn_Print.UseVisualStyleBackColor = True
        '
        'Btn_Connect
        '
        resources.ApplyResources(Me.Btn_Connect, "Btn_Connect")
        Me.Btn_Connect.ForeColor = System.Drawing.Color.Black
        Me.Btn_Connect.Name = "Btn_Connect"
        Me.Btn_Connect.UseVisualStyleBackColor = True
        '
        'ComBox_ComPort
        '
        resources.ApplyResources(Me.ComBox_ComPort, "ComBox_ComPort")
        Me.ComBox_ComPort.FormattingEnabled = True
        Me.ComBox_ComPort.Name = "ComBox_ComPort"
        '
        'Btn_Zero
        '
        resources.ApplyResources(Me.Btn_Zero, "Btn_Zero")
        Me.Btn_Zero.BackColor = System.Drawing.Color.Maroon
        Me.Btn_Zero.ForeColor = System.Drawing.Color.White
        Me.Btn_Zero.Name = "Btn_Zero"
        Me.Btn_Zero.Tag = "(M)"
        Me.Btn_Zero.UseVisualStyleBackColor = False
        '
        'Txt_Axle
        '
        resources.ApplyResources(Me.Txt_Axle, "Txt_Axle")
        Me.Txt_Axle.BackColor = System.Drawing.Color.FromArgb(CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer))
        Me.Txt_Axle.CausesValidation = False
        Me.Txt_Axle.Cursor = System.Windows.Forms.Cursors.Default
        Me.Txt_Axle.ForeColor = System.Drawing.Color.White
        Me.Txt_Axle.Name = "Txt_Axle"
        Me.Txt_Axle.ReadOnly = True
        Me.Txt_Axle.ShortcutsEnabled = False
        Me.Txt_Axle.TabStop = False
        Me.Txt_Axle.Tag = "M"
        '
        'GrpBox_Cal
        '
        resources.ApplyResources(Me.GrpBox_Cal, "GrpBox_Cal")
        Me.GrpBox_Cal.BackColor = System.Drawing.Color.Black
        Me.GrpBox_Cal.Controls.Add(Me.Cal_Btn_Exit)
        Me.GrpBox_Cal.Controls.Add(Me.Cal_Btn_Set)
        Me.GrpBox_Cal.Controls.Add(Me.Label16)
        Me.GrpBox_Cal.Controls.Add(Me.Label14)
        Me.GrpBox_Cal.Controls.Add(Me.Cal_Label_Real_Value)
        Me.GrpBox_Cal.Controls.Add(Me.Cal_Label_ADC)
        Me.GrpBox_Cal.Controls.Add(Me.Label12)
        Me.GrpBox_Cal.Controls.Add(Me.Label11)
        Me.GrpBox_Cal.Controls.Add(Me.RadioBtn_H)
        Me.GrpBox_Cal.Controls.Add(Me.RadioBtn_0)
        Me.GrpBox_Cal.Controls.Add(Me.ProgressBar1)
        Me.GrpBox_Cal.ForeColor = System.Drawing.Color.White
        Me.GrpBox_Cal.Name = "GrpBox_Cal"
        Me.GrpBox_Cal.TabStop = False
        '
        'Cal_Btn_Exit
        '
        Me.Cal_Btn_Exit.ForeColor = System.Drawing.Color.Black
        resources.ApplyResources(Me.Cal_Btn_Exit, "Cal_Btn_Exit")
        Me.Cal_Btn_Exit.Name = "Cal_Btn_Exit"
        Me.Cal_Btn_Exit.UseVisualStyleBackColor = True
        '
        'Cal_Btn_Set
        '
        Me.Cal_Btn_Set.ForeColor = System.Drawing.Color.Black
        resources.ApplyResources(Me.Cal_Btn_Set, "Cal_Btn_Set")
        Me.Cal_Btn_Set.Name = "Cal_Btn_Set"
        Me.Cal_Btn_Set.UseVisualStyleBackColor = True
        '
        'Label16
        '
        resources.ApplyResources(Me.Label16, "Label16")
        Me.Label16.BackColor = System.Drawing.Color.Black
        Me.Label16.ForeColor = System.Drawing.Color.White
        Me.Label16.Name = "Label16"
        '
        'Label14
        '
        resources.ApplyResources(Me.Label14, "Label14")
        Me.Label14.BackColor = System.Drawing.Color.Black
        Me.Label14.ForeColor = System.Drawing.Color.White
        Me.Label14.Name = "Label14"
        '
        'Cal_Label_Real_Value
        '
        Me.Cal_Label_Real_Value.BackColor = System.Drawing.Color.Black
        Me.Cal_Label_Real_Value.ForeColor = System.Drawing.Color.White
        resources.ApplyResources(Me.Cal_Label_Real_Value, "Cal_Label_Real_Value")
        Me.Cal_Label_Real_Value.Name = "Cal_Label_Real_Value"
        '
        'Cal_Label_ADC
        '
        Me.Cal_Label_ADC.BackColor = System.Drawing.Color.Black
        Me.Cal_Label_ADC.ForeColor = System.Drawing.Color.White
        resources.ApplyResources(Me.Cal_Label_ADC, "Cal_Label_ADC")
        Me.Cal_Label_ADC.Name = "Cal_Label_ADC"
        '
        'Label12
        '
        Me.Label12.BackColor = System.Drawing.Color.Black
        Me.Label12.ForeColor = System.Drawing.Color.White
        resources.ApplyResources(Me.Label12, "Label12")
        Me.Label12.Name = "Label12"
        '
        'Label11
        '
        Me.Label11.BackColor = System.Drawing.Color.Black
        Me.Label11.ForeColor = System.Drawing.Color.White
        resources.ApplyResources(Me.Label11, "Label11")
        Me.Label11.Name = "Label11"
        '
        'RadioBtn_H
        '
        resources.ApplyResources(Me.RadioBtn_H, "RadioBtn_H")
        Me.RadioBtn_H.BackColor = System.Drawing.Color.Black
        Me.RadioBtn_H.ForeColor = System.Drawing.Color.White
        Me.RadioBtn_H.Name = "RadioBtn_H"
        Me.RadioBtn_H.UseVisualStyleBackColor = False
        '
        'RadioBtn_0
        '
        resources.ApplyResources(Me.RadioBtn_0, "RadioBtn_0")
        Me.RadioBtn_0.BackColor = System.Drawing.Color.Black
        Me.RadioBtn_0.Checked = True
        Me.RadioBtn_0.ForeColor = System.Drawing.Color.White
        Me.RadioBtn_0.Name = "RadioBtn_0"
        Me.RadioBtn_0.TabStop = True
        Me.RadioBtn_0.UseVisualStyleBackColor = False
        '
        'ProgressBar1
        '
        resources.ApplyResources(Me.ProgressBar1, "ProgressBar1")
        Me.ProgressBar1.MarqueeAnimationSpeed = 1
        Me.ProgressBar1.Maximum = 1000
        Me.ProgressBar1.Name = "ProgressBar1"
        Me.ProgressBar1.Step = 1
        '
        'Label1
        '
        resources.ApplyResources(Me.Label1, "Label1")
        Me.Label1.BackColor = System.Drawing.Color.Transparent
        Me.Label1.ForeColor = System.Drawing.Color.White
        Me.Label1.Name = "Label1"
        Me.Label1.Tag = "M"
        '
        'Txt_Value
        '
        Me.Txt_Value.AccessibleRole = System.Windows.Forms.AccessibleRole.None
        resources.ApplyResources(Me.Txt_Value, "Txt_Value")
        Me.Txt_Value.BackColor = System.Drawing.Color.FromArgb(CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer))
        Me.Txt_Value.Cursor = System.Windows.Forms.Cursors.Default
        Me.Txt_Value.ForeColor = System.Drawing.Color.White
        Me.Txt_Value.Name = "Txt_Value"
        Me.Txt_Value.ReadOnly = True
        Me.Txt_Value.Tag = "M"
        '
        'Btn_Save
        '
        resources.ApplyResources(Me.Btn_Save, "Btn_Save")
        Me.Btn_Save.Name = "Btn_Save"
        Me.Btn_Save.Tag = ""
        Me.Btn_Save.UseVisualStyleBackColor = True
        '
        'Label15
        '
        resources.ApplyResources(Me.Label15, "Label15")
        Me.Label15.BackColor = System.Drawing.Color.Transparent
        Me.Label15.ForeColor = System.Drawing.Color.White
        Me.Label15.Name = "Label15"
        '
        'Label17
        '
        resources.ApplyResources(Me.Label17, "Label17")
        Me.Label17.BackColor = System.Drawing.Color.Transparent
        Me.Label17.ForeColor = System.Drawing.Color.White
        Me.Label17.Name = "Label17"
        Me.Label17.Tag = ""
        '
        'GrpBox_Number
        '
        Me.GrpBox_Number.BackColor = System.Drawing.Color.Black
        Me.GrpBox_Number.Controls.Add(Me.ComBox_ComPort)
        Me.GrpBox_Number.Controls.Add(Me.Rx_Indicator)
        Me.GrpBox_Number.Controls.Add(Me.Btn_Connect)
        Me.GrpBox_Number.Controls.Add(Me.Vehicle_Number)
        Me.GrpBox_Number.ForeColor = System.Drawing.Color.White
        resources.ApplyResources(Me.GrpBox_Number, "GrpBox_Number")
        Me.GrpBox_Number.Name = "GrpBox_Number"
        Me.GrpBox_Number.TabStop = False
        '
        'Rx_Indicator
        '
        resources.ApplyResources(Me.Rx_Indicator, "Rx_Indicator")
        Me.Rx_Indicator.BackColor = System.Drawing.Color.White
        Me.Rx_Indicator.ForeColor = System.Drawing.Color.Transparent
        Me.Rx_Indicator.Name = "Rx_Indicator"
        Me.Rx_Indicator.UseVisualStyleBackColor = False
        '
        'Vehicle_Number
        '
        resources.ApplyResources(Me.Vehicle_Number, "Vehicle_Number")
        Me.Vehicle_Number.Name = "Vehicle_Number"
        '
        'PictureBox1
        '
        Me.PictureBox1.BackColor = System.Drawing.Color.Transparent
        resources.ApplyResources(Me.PictureBox1, "PictureBox1")
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.TabStop = False
        '
        'GrpBox_Set
        '
        Me.GrpBox_Set.BackColor = System.Drawing.Color.Black
        Me.GrpBox_Set.Controls.Add(Me.Setting_Btn_Exit)
        Me.GrpBox_Set.Controls.Add(Me.Setting_Btn_Save)
        Me.GrpBox_Set.Controls.Add(Me.Txt_Wait_Time_Measure)
        Me.GrpBox_Set.Controls.Add(Me.Txt_Noise_Zero_Tolerance)
        Me.GrpBox_Set.Controls.Add(Me.Label20)
        Me.GrpBox_Set.Controls.Add(Me.Label18)
        Me.GrpBox_Set.Controls.Add(Me.Label19)
        Me.GrpBox_Set.Controls.Add(Me.Label13)
        Me.GrpBox_Set.ForeColor = System.Drawing.Color.White
        resources.ApplyResources(Me.GrpBox_Set, "GrpBox_Set")
        Me.GrpBox_Set.Name = "GrpBox_Set"
        Me.GrpBox_Set.TabStop = False
        '
        'Setting_Btn_Exit
        '
        Me.Setting_Btn_Exit.ForeColor = System.Drawing.Color.Black
        resources.ApplyResources(Me.Setting_Btn_Exit, "Setting_Btn_Exit")
        Me.Setting_Btn_Exit.Name = "Setting_Btn_Exit"
        Me.Setting_Btn_Exit.UseVisualStyleBackColor = True
        '
        'Setting_Btn_Save
        '
        Me.Setting_Btn_Save.ForeColor = System.Drawing.Color.Black
        resources.ApplyResources(Me.Setting_Btn_Save, "Setting_Btn_Save")
        Me.Setting_Btn_Save.Name = "Setting_Btn_Save"
        Me.Setting_Btn_Save.UseVisualStyleBackColor = True
        '
        'Txt_Wait_Time_Measure
        '
        resources.ApplyResources(Me.Txt_Wait_Time_Measure, "Txt_Wait_Time_Measure")
        Me.Txt_Wait_Time_Measure.Name = "Txt_Wait_Time_Measure"
        '
        'Txt_Noise_Zero_Tolerance
        '
        resources.ApplyResources(Me.Txt_Noise_Zero_Tolerance, "Txt_Noise_Zero_Tolerance")
        Me.Txt_Noise_Zero_Tolerance.Name = "Txt_Noise_Zero_Tolerance"
        '
        'Label20
        '
        resources.ApplyResources(Me.Label20, "Label20")
        Me.Label20.BackColor = System.Drawing.Color.Black
        Me.Label20.ForeColor = System.Drawing.Color.White
        Me.Label20.Name = "Label20"
        '
        'Label18
        '
        resources.ApplyResources(Me.Label18, "Label18")
        Me.Label18.BackColor = System.Drawing.Color.Black
        Me.Label18.ForeColor = System.Drawing.Color.White
        Me.Label18.Name = "Label18"
        '
        'Label19
        '
        resources.ApplyResources(Me.Label19, "Label19")
        Me.Label19.BackColor = System.Drawing.Color.Black
        Me.Label19.ForeColor = System.Drawing.Color.White
        Me.Label19.Name = "Label19"
        '
        'Label13
        '
        resources.ApplyResources(Me.Label13, "Label13")
        Me.Label13.BackColor = System.Drawing.Color.Black
        Me.Label13.ForeColor = System.Drawing.Color.White
        Me.Label13.Name = "Label13"
        '
        'BBM_SideSlip
        '
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.Color.Black
        Me.BackgroundImage = Global.UjiSideSlip.My.Resources.Resources.Mobil
        Me.Controls.Add(Me.Btn_Zero)
        Me.Controls.Add(Me.Txt_Axle)
        Me.Controls.Add(Me.Txt_Value)
        Me.Controls.Add(Me.GrpBox_Number)
        Me.Controls.Add(Me.GrpBox_Set)
        Me.Controls.Add(Me.GrpBox_Cal)
        Me.Controls.Add(Me.Label15)
        Me.Controls.Add(Me.Label17)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.Btn_Save)
        Me.Controls.Add(Me.Btn_Print)
        Me.Controls.Add(Me.MenuStrip1)
        Me.Controls.Add(Me.PictureBox1)
        Me.ForeColor = System.Drawing.Color.Black
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.MainMenuStrip = Me.MenuStrip1
        Me.MinimizeBox = False
        Me.Name = "BBM_SideSlip"
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.GrpBox_Cal.ResumeLayout(False)
        Me.GrpBox_Cal.PerformLayout()
        Me.GrpBox_Number.ResumeLayout(False)
        Me.GrpBox_Number.PerformLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GrpBox_Set.ResumeLayout(False)
        Me.GrpBox_Set.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents BackgroundWorker1 As System.ComponentModel.BackgroundWorker
    Friend WithEvents Tick As Timer
    Friend WithEvents Bgrn_ComPort As IO.Ports.SerialPort
    Friend WithEvents MenuStrip1 As MenuStrip
    Friend WithEvents MenuToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents CalibrationToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents Btn_Print As Button
    Friend WithEvents Btn_Connect As Button
    Friend WithEvents ComBox_ComPort As ComboBox
    Friend WithEvents Btn_Zero As Button
    Friend WithEvents Txt_Axle As TextBox
    Friend WithEvents GrpBox_Cal As GroupBox
    Friend WithEvents Cal_Btn_Exit As Button
    Friend WithEvents Cal_Btn_Set As Button
    Friend WithEvents Label14 As Label
    Friend WithEvents Cal_Label_ADC As Label
    Friend WithEvents Label12 As Label
    Friend WithEvents Label11 As Label
    Friend WithEvents RadioBtn_H As RadioButton
    Friend WithEvents RadioBtn_0 As RadioButton
    Friend WithEvents ProgressBar1 As ProgressBar
    Friend WithEvents Label10 As Label
    Friend WithEvents Label7 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents Label6 As Label
    Friend WithEvents Label8 As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents Txt_Value As TextBox
    Friend WithEvents SaveToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents Label16 As Label
    Friend WithEvents Cal_Label_Real_Value As Label
    Friend WithEvents Btn_Save As Button
    Friend WithEvents ExitToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents PrintToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents Label15 As Label
    Friend WithEvents Label17 As Label
    Friend WithEvents GrpBox_Number As GroupBox
    Friend WithEvents PictureBox1 As PictureBox
    Friend WithEvents SettingToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents GrpBox_Set As GroupBox
    Friend WithEvents Setting_Btn_Exit As Button
    Friend WithEvents Setting_Btn_Save As Button
    Public WithEvents Txt_Noise_Zero_Tolerance As TextBox
    Friend WithEvents Label13 As Label
    Public WithEvents Txt_Wait_Time_Measure As TextBox
    Friend WithEvents Label20 As Label
    Friend WithEvents Label18 As Label
    Friend WithEvents Label19 As Label
    Friend WithEvents Rx_Indicator As Button
    Public WithEvents Vehicle_Number As TextBox
End Class
