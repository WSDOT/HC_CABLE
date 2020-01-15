VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Begin VB.Form frmMain 
   AutoRedraw      =   -1  'True
   Caption         =   "YEARTIDE"
   ClientHeight    =   3270
   ClientLeft      =   120
   ClientTop       =   1605
   ClientWidth     =   9435
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3270
   ScaleWidth      =   9435
   Begin VB.PictureBox picMain 
      AutoRedraw      =   -1  'True
      Height          =   2175
      Left            =   60
      ScaleHeight     =   2115
      ScaleWidth      =   9195
      TabIndex        =   0
      Top             =   60
      Width           =   9255
      Begin VB.Frame Frame1 
         Caption         =   "Seattle Tides"
         Height          =   1635
         Left            =   240
         TabIndex        =   1
         Top             =   240
         Width           =   8835
         Begin VB.CommandButton Command1 
            Caption         =   "Run"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   7680
            TabIndex        =   13
            Top             =   600
            Width           =   855
         End
         Begin VB.TextBox Text2 
            BeginProperty Font 
               Name            =   "Courier"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   360
            Index           =   2
            Left            =   6300
            TabIndex        =   12
            Text            =   "99.9"
            Top             =   1080
            Width           =   1215
         End
         Begin VB.TextBox Text2 
            BeginProperty Font 
               Name            =   "Courier"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   360
            Index           =   1
            Left            =   4980
            TabIndex        =   11
            Text            =   "9999"
            Top             =   1080
            Width           =   1215
         End
         Begin VB.TextBox Text2 
            BeginProperty Font 
               Name            =   "Courier"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   360
            Index           =   0
            Left            =   3660
            TabIndex        =   10
            Text            =   "9"
            Top             =   1080
            Width           =   1215
         End
         Begin VB.TextBox Text1 
            BeginProperty Font 
               Name            =   "Courier"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   360
            Index           =   2
            Left            =   6300
            TabIndex        =   9
            Text            =   "99.9"
            Top             =   600
            Width           =   1215
         End
         Begin VB.TextBox Text1 
            BeginProperty Font 
               Name            =   "Courier"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   360
            Index           =   1
            Left            =   4980
            TabIndex        =   8
            Text            =   "9999"
            Top             =   600
            Width           =   1215
         End
         Begin VB.TextBox Text1 
            BeginProperty Font 
               Name            =   "Courier"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   360
            Index           =   0
            Left            =   3660
            TabIndex        =   7
            Text            =   "9"
            Top             =   600
            Width           =   1215
         End
         Begin VB.Label Label5 
            Caption         =   "Height"
            Height          =   195
            Left            =   6600
            TabIndex        =   6
            Top             =   240
            Width           =   675
         End
         Begin VB.Label Label4 
            Caption         =   "Time"
            Height          =   195
            Left            =   5340
            TabIndex        =   5
            Top             =   240
            Width           =   675
         End
         Begin VB.Label Label3 
            Caption         =   "Enter H or L"
            Height          =   195
            Left            =   3840
            TabIndex        =   4
            Top             =   240
            Width           =   1095
         End
         Begin VB.Label Label2 
            Caption         =   "Following High or Low Tide ( January 1 )"
            Height          =   195
            Left            =   300
            TabIndex        =   3
            Top             =   1140
            Width           =   3255
         End
         Begin VB.Label Label1 
            Caption         =   "Preceeding High or Low Tide ( December 31 )"
            Height          =   195
            Left            =   300
            TabIndex        =   2
            Top             =   660
            Width           =   3255
         End
      End
   End
   Begin ComctlLib.ProgressBar ProgressBar1 
      Height          =   375
      Left            =   60
      TabIndex        =   15
      Top             =   2400
      Width           =   9255
      _ExtentX        =   16325
      _ExtentY        =   661
      _Version        =   327682
      Appearance      =   1
      Min             =   1
      Max             =   366
   End
   Begin VB.Label Label6 
      Caption         =   "Time and Height must be four characters long.     Include leading spaces, and one decimal place for height."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   14
      Top             =   2940
      Width           =   9255
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
    Call Main
End Sub


