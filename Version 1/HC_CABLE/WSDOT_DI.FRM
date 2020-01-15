VERSION 4.00
Begin VB.Form WSDOT_DIAG 
   Caption         =   "WSDOT  -  Cable Tension Program Diagnostic"
   ClientHeight    =   2370
   ClientLeft      =   1485
   ClientTop       =   2535
   ClientWidth     =   5490
   Height          =   2775
   Left            =   1425
   LinkTopic       =   "Form1"
   ScaleHeight     =   2370
   ScaleWidth      =   5490
   Top             =   2190
   Width           =   5610
   Begin VB.VScrollBar VSkrl_ManualTide 
      Height          =   735
      Left            =   3960
      TabIndex        =   2
      Top             =   840
      Width           =   255
   End
   Begin VB.CommandButton btn_ApplyManualTide 
      BackColor       =   &H000000FF&
      Caption         =   "Apply"
      BeginProperty Font 
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   12
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   960
      TabIndex        =   1
      Top             =   960
      Width           =   1335
   End
   Begin VB.TextBox txtbox_ManualTide 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   24
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   735
      Left            =   2640
      TabIndex        =   0
      Text            =   "00.0"
      Top             =   840
      Width           =   1335
   End
   Begin VB.Frame Frame1 
      Caption         =   "Manual Tide Elevation Entry"
      BeginProperty Font 
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   12
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00404040&
      Height          =   1815
      Left            =   240
      TabIndex        =   3
      Top             =   240
      Width           =   4575
   End
End
Attribute VB_Name = "WSDOT_DIAG"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
