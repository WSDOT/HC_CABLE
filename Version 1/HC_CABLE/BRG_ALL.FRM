VERSION 5.00
Begin VB.Form WSDOT_ALL 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   0  'None
   Caption         =   "WSDOT  -  Bridge Cable Tension   -   ver. 1.1"
   ClientHeight    =   8160
   ClientLeft      =   1050
   ClientTop       =   330
   ClientWidth     =   7185
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   8160
   ScaleWidth      =   7185
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton btn_ApplyManualTide 
      BackColor       =   &H000000FF&
      Caption         =   "Apply"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3000
      TabIndex        =   14
      Top             =   630
      Width           =   855
   End
   Begin VB.VScrollBar VS_ManualTideChange 
      Height          =   375
      Left            =   2730
      TabIndex        =   13
      Top             =   630
      Width           =   255
   End
   Begin VB.Timer OneSecTick 
      Interval        =   1000
      Left            =   6240
      Top             =   480
   End
   Begin VB.CommandButton btn_GoCablePair 
      Caption         =   "Select Pair"
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   4515
      TabIndex        =   11
      Top             =   7665
      Width           =   1095
   End
   Begin VB.CommandButton Command_Cables_EXIT 
      Caption         =   "Exit"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   5880
      TabIndex        =   1
      Top             =   7665
      Width           =   975
   End
   Begin VB.ListBox ListBox_Cables_1 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   5730
      ItemData        =   "BRG_ALL.frx":0000
      Left            =   240
      List            =   "BRG_ALL.frx":0002
      TabIndex        =   0
      Top             =   1050
      Width           =   6615
   End
   Begin VB.Label Label9 
      Alignment       =   1  'Right Justify
      Caption         =   "Tidal Elevation : "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   1575
      TabIndex        =   9
      Top             =   7665
      Width           =   1695
   End
   Begin VB.Label lbl_TideElev 
      Alignment       =   2  'Center
      Caption         =   "-1.3 ft."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   255
      Left            =   3255
      TabIndex        =   10
      Top             =   7665
      Width           =   1215
   End
   Begin VB.Image Image1 
      Height          =   600
      Left            =   6000
      Picture         =   "BRG_ALL.frx":0004
      Top             =   6960
      Width           =   645
   End
   Begin VB.Label Label6 
      Alignment       =   2  'Center
      BackColor       =   &H00FF0000&
      Caption         =   "HNTB"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Index           =   1
      Left            =   210
      TabIndex        =   12
      Top             =   7770
      Width           =   975
   End
   Begin VB.Label Label8 
      BackStyle       =   0  'Transparent
      Caption         =   "       (PSI)              (KIPS)                                             (KIPS)               (PSI)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   360
      TabIndex        =   8
      Top             =   720
      Width           =   8775
   End
   Begin VB.Line Line7 
      X1              =   240
      X2              =   240
      Y1              =   1080
      Y2              =   0
   End
   Begin VB.Line Line6 
      X1              =   3960
      X2              =   3960
      Y1              =   1080
      Y2              =   0
   End
   Begin VB.Line Line5 
      X1              =   2640
      X2              =   2640
      Y1              =   1080
      Y2              =   0
   End
   Begin VB.Line Line4 
      X1              =   6840
      X2              =   6840
      Y1              =   1080
      Y2              =   0
   End
   Begin VB.Line Line3 
      X1              =   240
      X2              =   6840
      Y1              =   360
      Y2              =   360
   End
   Begin VB.Line Line2 
      X1              =   5160
      X2              =   5160
      Y1              =   1080
      Y2              =   360
   End
   Begin VB.Line Line1 
      X1              =   1560
      X2              =   1560
      Y1              =   1080
      Y2              =   360
   End
   Begin VB.Label Label7 
      Caption         =   " Gauge      Tension         Cable         Tension       Gauge"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   480
      TabIndex        =   7
      Top             =   360
      Width           =   6255
   End
   Begin VB.Label Label6 
      Caption         =   "          North                                                  South"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   0
      Left            =   600
      TabIndex        =   6
      Top             =   0
      Width           =   6015
   End
   Begin VB.Shape Shape1 
      Height          =   750
      Left            =   240
      Top             =   6840
      Width           =   6615
   End
   Begin VB.Label lbl_CurrentTime 
      Caption         =   "(mm dd yy  hh:mm:ss)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2520
      TabIndex        =   5
      Top             =   6930
      Width           =   3255
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "Current Time : "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   630
      TabIndex        =   3
      Top             =   7245
      Width           =   1815
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "Calculation Time : "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   360
      TabIndex        =   2
      Top             =   6930
      Width           =   2055
   End
   Begin VB.Label lbl_CalculationTime 
      Caption         =   "(mm dd yy  hh:mm:ss)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   375
      Left            =   2520
      TabIndex        =   4
      Top             =   7245
      Width           =   3255
   End
End
Attribute VB_Name = "WSDOT_ALL"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ------------------------------------------------
'       Copyright(C) 1997 HNTB Corporation
' ------------------------------------------------
' WSDOT_ALL.FRM - Complete Hood Canal Bridge Cable list window
' ------------------------------------------------
' Revisions
' ------------------------------------------------
' rev   by   date       description
' ----  ---  ------     --------------------------
' 0.01  elr  02/28/97   original design
' 0.02  elr  06/01/97   enabled tension calc data access
' ------------------------------------------------

Private Sub Form_Load()
If PassWord = "47341" Then
  bManualTideAdjust = True
'  Open "c:\progra~1\wsdotbrg\outfile.txt" For Output As #1
  Open "c:\ARP\HC_CABLE\outfile.txt" For Output As #1
Else
  bManualTideAdjust = False
End If

If bManualTideAdjust = True Then
  VS_ManualTideChange.Visible = True
  VS_ManualTideChange.Enabled = True
  btn_ApplyManualTide.Visible = True
  btn_ApplyManualTide.Enabled = True
Else
  VS_ManualTideChange.Visible = False
  VS_ManualTideChange.Enabled = False
  btn_ApplyManualTide.Visible = False
  btn_ApplyManualTide.Enabled = False
End If

WSDOT_ALL.Move 0, 0
WSDOT_MAIN.Caption = "WSDOT  -  Hood Canal Bridge Cable List"
WSDOT_MAIN.Width = WSDOT_ALL.ScaleWidth * 1.01 + (2 * Borderw)
WSDOT_MAIN.Height = WSDOT_ALL.ScaleHeight * 1.06 + (Borderw + TitleH)
WSDOT_MAIN.Move (Screen.Width / 2) - WSDOT_MAIN.Width / 2, (Screen.Height / 2) - WSDOT_MAIN.Height / 1.92
lbl_CurrentTime.Caption = Format(Date, "dddd, mm/dd/yy") & " - " & Format(Time, "hh:mm:ss")

ListBox_Cables_1.AddItem "   ****   ***    N -A- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -B- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -C- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -D- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -E- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -F- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -G- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -H- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -J- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N  L1 S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N  L2 S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -M- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    NW N SW   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    NE N SE   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -O- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -P- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -Q- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -R- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -S- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -T- S   ***    ****"
ListBox_Cables_1.AddItem "   ****   ***    N -U- S   ***    ****"
UpdateCableList
GetNowTide
UpdateStamps

'DEV:VS_ManualTideChange.Value = (CrntTide * 10)
VS_ManualTideChange.SmallChange = 1
VS_ManualTideChange.LargeChange = 10
VS_ManualTideChange.Min = -200
VS_ManualTideChange.Max = 200


End Sub

Private Sub btn_ApplyManualTide_Click()
'bDebugPrint = True
'bDebugPrint2 = True
'bDebugPrint3 = True
'bDebugPrint4 = True
'bDebugPrint5 = True
  
  CalcTension
  UpdateCableList
  UpdateStamps

End Sub

Private Sub UpdateCableList()

Dim LineBfr As String
Dim WeeBfr As String
Dim i As Integer

For i = 0 To ((NumCables / 2) - 1)
'LineBfr = "   ----   ---    N -A- S   ---    ----"
 LineBfr = "                 N -A- S              "
s = 0
If CInt(TensionAry(i * 2)) < 100 Then s = 1
If CInt(TensionAry(i * 2)) > 9999 Then s = -1
If CInt(TensionAry(i * 2)) = -2 Then
  Mid(LineBfr, ofstTensionL + s, lenTension) = "ERR"
  Mid(LineBfr, ofstGaugeL, lenGauge) = "ERR"
Else
  If s = -1 Then
    Mid(LineBfr, ofstTensionL + s, lenTension + 1) = CInt(TensionAry(i * 2))
  End If
  Mid(LineBfr, ofstTensionL + s, lenTension) = CInt(TensionAry(i * 2))
  Mid(LineBfr, ofstGaugeL, lenGauge) = CInt(GaugeAry(i * 2))
End If
Mid(LineBfr, ofstPairNam, lenPairNam) = PairNam(i)
s = 0
If CInt(TensionAry((i * 2) + 1)) < 100 Then s = 1
If CInt(TensionAry((i * 2) + 1)) > 9999 Then s = -1
If CInt(TensionAry((i * 2) + 1)) = -2 Then
  Mid(LineBfr, ofstTensionR + s, lenTension) = "ERR"
  Mid(LineBfr, ofstGaugeR, lenGauge) = "ERR"
Else
  If s = -1 Then
    Mid(LineBfr, ofstTensionL + s, lenTension + 1) = CInt(TensionAry((i * 2) + 1))
  End If
  Mid(LineBfr, ofstTensionR + s, lenTension) = CInt(TensionAry((i * 2) + 1))
  Mid(LineBfr, ofstGaugeR, lenGauge) = CInt(GaugeAry((i * 2) + 1))
End If
ListBox_Cables_1.RemoveItem i
ListBox_Cables_1.AddItem LineBfr, i

Next i

End Sub

' ----------------------------------------
Private Sub btn_GoCablePair_Click()
  SelCable = ListBox_Cables_1.ListIndex
  Unload WSDOT_ALL
  Load WSDOT_CBL
End Sub

' ----------------------------------------
Private Sub Command_Cables_EXIT_Click()
End
End Sub

' ---------------------------------------
Private Sub ListBox_Cables_1_DblClick()
Unload WSDOT_ALL
Load WSDOT_CBL
End Sub
Private Sub ListBox_Cables_1_Click()
  btn_GoCablePair.Enabled = True
  SelCable = ListBox_Cables_1.ListIndex
End Sub

' -----------------------------------------------------
' Misc processing that must happen on each second tick.
' -----------------------------------------------------
Private Sub OneSecTick_Timer()

lbl_CurrentTime.Caption = Format(Date, "dddd, mm/dd/yy") & " - " & Format(Time, "hh:mm:ss")

'If Second(Time) Mod 30 = 0 Then
If (Minute(Time) Mod 10 = 0) Then
  If bTimesUp = False Then
    If bManualTideAdjust = False Then
      bTimesUp = True
      GetTideNow
      CalcTension
      UpdateCableList
      UpdateStamps
    End If
  End If
Else
  bTimesUp = False
End If

End Sub

' --------------------------------------------
' Stamp current date/time into 'Calculation Time' in window.
' --------------------------------------------
Private Sub UpdateStamps()

  lbl_CalculationTime.Caption = Str_CalcTime
  lbl_TideElev.Caption = STR_Tide
End Sub

' --------------------------------------------
Private Sub GetTideNow()
  TideElev = CrntTide
End Sub

Private Sub VS_ManualTideChange_Change()
  CrntTide = -(VS_ManualTideChange.Value / 10)
  StampCalcTime
  UpdateStamps
End Sub
