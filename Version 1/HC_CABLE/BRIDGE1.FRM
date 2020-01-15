VERSION 5.00
Begin VB.Form WSDOT_CBL 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   0  'None
   Caption         =   "WSDOT  -  Bridge Cable Tension   -   ver. 1.1"
   ClientHeight    =   4065
   ClientLeft      =   3555
   ClientTop       =   2100
   ClientWidth     =   6750
   Icon            =   "BRIDGE1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   4065
   ScaleWidth      =   6750
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton Command1 
      Caption         =   "Cable List"
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
      Left            =   4200
      TabIndex        =   9
      Top             =   3360
      Width           =   1095
   End
   Begin VB.Timer OneSecTick 
      Interval        =   1000
      Left            =   3150
      Top             =   420
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
      Left            =   5460
      TabIndex        =   1
      Top             =   3360
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
      Height          =   330
      ItemData        =   "BRIDGE1.frx":0442
      Left            =   315
      List            =   "BRIDGE1.frx":0449
      TabIndex        =   0
      Top             =   1575
      Width           =   6105
   End
   Begin VB.Line Line1 
      X1              =   315
      X2              =   6405
      Y1              =   840
      Y2              =   840
   End
   Begin VB.Image Image1 
      Height          =   600
      Left            =   5670
      Picture         =   "BRIDGE1.frx":0475
      Top             =   2310
      Width           =   645
   End
   Begin VB.Line Line11 
      X1              =   5145
      X2              =   5145
      Y1              =   1575
      Y2              =   840
   End
   Begin VB.Line Line4 
      X1              =   4095
      X2              =   4095
      Y1              =   1590
      Y2              =   420
   End
   Begin VB.Line Line10 
      X1              =   315
      X2              =   6405
      Y1              =   420
      Y2              =   420
   End
   Begin VB.Line Line2 
      X1              =   315
      X2              =   315
      Y1              =   1590
      Y2              =   420
   End
   Begin VB.Line Line12 
      X1              =   1575
      X2              =   1575
      Y1              =   1545
      Y2              =   840
   End
   Begin VB.Line Line8 
      X1              =   2730
      X2              =   2730
      Y1              =   1575
      Y2              =   420
   End
   Begin VB.Label Label10 
      Caption         =   "              North                                                  South"
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
      Left            =   420
      TabIndex        =   12
      Top             =   525
      Width           =   5370
   End
   Begin VB.Label Label5 
      Caption         =   "   Gauge       Tension                           Tension      Gauge "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Left            =   420
      TabIndex        =   11
      Top             =   840
      Width           =   5895
   End
   Begin VB.Line Line9 
      X1              =   6405
      X2              =   6405
      Y1              =   1590
      Y2              =   420
   End
   Begin VB.Label Label4 
      BackStyle       =   0  'Transparent
      Caption         =   "   (PSI)          (KIPS)          Cable         (KIPS)         (PSI)"
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
      Left            =   525
      TabIndex        =   10
      Top             =   1260
      Width           =   5685
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
      Left            =   315
      TabIndex        =   8
      Top             =   3465
      Width           =   975
   End
   Begin VB.Label lbl_TideElev 
      Caption         =   "-1.3 ft."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   255
      Left            =   3255
      TabIndex        =   7
      Top             =   3465
      Width           =   960
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
      Left            =   1470
      TabIndex        =   6
      Top             =   3465
      Width           =   1695
   End
   Begin VB.Shape Shape1 
      Height          =   1065
      Left            =   315
      Top             =   2100
      Width           =   6090
   End
   Begin VB.Label lbl_CurrentTime 
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
      Left            =   2310
      TabIndex        =   5
      Top             =   2625
      Width           =   3270
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
      Left            =   420
      TabIndex        =   3
      Top             =   2625
      Width           =   1800
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
      Height          =   285
      Left            =   315
      TabIndex        =   2
      Top             =   2310
      Width           =   1905
   End
   Begin VB.Label lbl_CalculationTime 
      Caption         =   "* * * * * * * * * * * * * * * * * * * *"
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
      Left            =   2310
      TabIndex        =   4
      Top             =   2310
      Width           =   3270
   End
End
Attribute VB_Name = "WSDOT_CBL"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ------------------------------------------------
'       Copyright(C) 1997 HNTB Corporation
' ------------------------------------------------
' WSDOT_CBL.FRM - Hood Canal Bridge Cable Tension window
' ------------------------------------------------
' Revisions
' ------------------------------------------------
' rev   by   date       description
' ----  ---  ------     --------------------------
' 0.01  elr  02/28/97   original design
' 0.02  elr  03/06/97   center window on form load, add prototype for
'                       config file load (gauge factors, etc.), add
'                       diag fn to pull tide values, diag fn to calc
'                       tension, diag fn to calc gauge reading, fn to
'                       load cable list box.
' -------------------------------------------------------------------

' ----------------------------------------------
' Processing that happens as the window is loaded.
' ----------------------------------------------
Private Sub Form_Load()
WSDOT_CBL.Move 0, 0
WSDOT_MAIN.Caption = "WSDOT  -  Hood Canal Bridge Cable Pair"
WSDOT_MAIN.Width = WSDOT_CBL.ScaleWidth * 1.01 + (2 * Borderw)
WSDOT_MAIN.Height = WSDOT_CBL.ScaleHeight * 1.08 + (Borderw + TitleH)
WSDOT_MAIN.Move (Screen.Width / 2) - WSDOT_MAIN.Width / 2, (Screen.Height / 2) - WSDOT_MAIN.Height / 1.92

InitListBox
UpdateCableList
ListBox_Cables_1.ListIndex = SelCable 'ListBox_Cables_1.List(SelCable)
UpdateStamps

End Sub

' ----------------------------------------------
Private Sub Command_Cables_EXIT_Click()
End
End Sub

Private Sub Command1_Click()
Unload WSDOT_CBL
Load WSDOT_ALL
End Sub

Private Sub InitListBox()
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

End Sub

' -------------------------------------------
' Misc processing that must happen on each second tick.
' -------------------------------------------
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
Private Sub GetTideNow()
  TideElev = CrntTide
End Sub

' ----------------------------------------------------------
' Stamp current date/time into 'Calculation Time' in window.
' ----------------------------------------------------------
Private Sub UpdateStamps()

lbl_CalculationTime.Caption = Str_CalcTime
lbl_TideElev.Caption = STR_Tide
End Sub

' --------------------------------------------
' Using current tide values, calculate tension and gauge reading,
' and update each cable line in the list box.
' --------------------------------------------
Private Sub UpdateCableList()

Dim LineBfr As String
Dim i As Integer
Dim s As Integer

For i = 0 To ((NumCables / 2) - 1)
' --------"   8445   233    N -U- S   189    9241"
LineBfr = "                 N -A- S              "
s = 0
If CInt(TensionAry(i * 2)) < 100 Then
s = 1
End If
Mid(LineBfr, ofstTensionL + s, lenTension) = CInt(TensionAry(i * 2))
Mid(LineBfr, ofstGaugeL, lenGauge) = GaugeAry(i * 2)
Mid(LineBfr, ofstPairNam, lenPairNam) = PairNam(i)
s = 0
If CInt(TensionAry((i * 2) + 1)) < 100 Then s = 1
Mid(LineBfr, ofstTensionR + s, lenTension) = CInt(TensionAry((i * 2) + 1))
Mid(LineBfr, ofstGaugeR, lenGauge) = GaugeAry((i * 2) + 1)

ListBox_Cables_1.RemoveItem i
ListBox_Cables_1.AddItem LineBfr, i

Next i

End Sub
' --------------------------------------------

' --------------------------------------------
' END OF WSDOT_CBL.FRM CODE
' --------------------------------------------
