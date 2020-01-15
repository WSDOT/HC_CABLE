VERSION 5.00
Begin VB.Form WSDOT_LOGO 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   0  'None
   Caption         =   "Logo Window"
   ClientHeight    =   6975
   ClientLeft      =   810
   ClientTop       =   1380
   ClientWidth     =   10260
   FillColor       =   &H000000FF&
   ForeColor       =   &H00808080&
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   6975
   ScaleWidth      =   10260
   ShowInTaskbar   =   0   'False
   Begin VB.Timer Timer1 
      Interval        =   1000
      Left            =   480
      Top             =   4920
   End
   Begin VB.PictureBox Picture1 
      Height          =   4455
      Left            =   1080
      Picture         =   "WSDOT_LO.frx":0000
      ScaleHeight     =   4395
      ScaleWidth      =   7995
      TabIndex        =   8
      Top             =   120
      Width           =   8055
      Begin VB.Label lbl_WaitWarning 
         BackStyle       =   0  'Transparent
         Caption         =   "Now Loading Data Tables ...     please stand by."
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   18
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   975
         Left            =   1440
         TabIndex        =   9
         Top             =   720
         Width           =   4935
      End
   End
   Begin VB.TextBox txtbox_PWInput 
      Height          =   315
      Left            =   960
      TabIndex        =   6
      Top             =   6480
      Width           =   735
   End
   Begin VB.CommandButton btn_Exit 
      Caption         =   "Exit"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3000
      TabIndex        =   1
      Top             =   6480
      Width           =   1215
   End
   Begin VB.CommandButton btn_Proceed 
      Caption         =   "Proceed"
      Default         =   -1  'True
      Enabled         =   0   'False
      Height          =   375
      Left            =   6240
      TabIndex        =   0
      Top             =   6480
      Width           =   1215
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
      Left            =   8160
      TabIndex        =   10
      Top             =   6600
      Width           =   975
   End
   Begin VB.Image Image1 
      Height          =   600
      Left            =   4800
      Picture         =   "WSDOT_LO.frx":26312
      Top             =   6360
      Width           =   645
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      Caption         =   "Password"
      Height          =   375
      Left            =   120
      TabIndex        =   7
      Top             =   6480
      Width           =   855
   End
   Begin VB.Label Label3 
      BackStyle       =   0  'Transparent
      Caption         =   $"WSDOT_LO.frx":26E34
      ForeColor       =   &H00FF0000&
      Height          =   495
      Left            =   480
      TabIndex        =   5
      Top             =   5640
      Width           =   9255
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Caption         =   "version 1.1"
      Height          =   255
      Left            =   9000
      TabIndex        =   4
      Top             =   6600
      Width           =   1215
   End
   Begin VB.Shape Shape1 
      BorderStyle     =   6  'Inside Solid
      FillColor       =   &H00FFFF80&
      FillStyle       =   0  'Solid
      Height          =   735
      Left            =   360
      Shape           =   4  'Rounded Rectangle
      Top             =   5520
      Width           =   9495
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "Cable Tension Program"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   3360
      TabIndex        =   3
      Top             =   5160
      Width           =   3375
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "WSDOT  -  Hood Canal Bridge"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   18
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   495
      Left            =   1920
      TabIndex        =   2
      Top             =   4680
      Width           =   6015
   End
End
Attribute VB_Name = "WSDOT_LOGO"
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

Private Sub btn_Exit_Click()
End
End Sub

Private Sub btn_Proceed_Click()
  PassWord = txtbox_PWInput.Text
  Unload WSDOT_LOGO
  Load WSDOT_ALL

End Sub

Private Sub Form_Load()

WSDOT_LOGO.Move 0, 0
WSDOT_MAIN.Caption = "WSDOT  -  Hood Canal Bridge Cable Tension Program"
WSDOT_MAIN.Width = WSDOT_LOGO.ScaleWidth * 1.02 + (2 * Borderw)
WSDOT_MAIN.Height = WSDOT_LOGO.ScaleHeight * 1.06 + (Borderw + TitleH)
WSDOT_MAIN.Move (Screen.Width / 2) - WSDOT_MAIN.Width / 2, (Screen.Height / 2) - WSDOT_MAIN.Height / 1.92
End Sub

' ------------------------------------------------------------------------

Private Sub Timer1_Timer()
  lbl_WaitWarning.Visible = False
  btn_Proceed.Enabled = True
  btn_Exit.Enabled = True
  Timer1.Enabled = False
  Beep
End Sub
