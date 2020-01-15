VERSION 5.00
Begin VB.MDIForm WSDOT_MAIN 
   BackColor       =   &H8000000C&
   Caption         =   "WSDOT  Hood Canal Bridge"
   ClientHeight    =   7080
   ClientLeft      =   555
   ClientTop       =   1185
   ClientWidth     =   10260
   LinkTopic       =   "MDIForm1"
   ScrollBars      =   0   'False
End
Attribute VB_Name = "WSDOT_MAIN"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ------------------------------------------------
'       Copyright(C) 1997 HNTB Corporation
' ------------------------------------------------
' WSDOT_MAIN.FRM - Main window
' ------------------------------------------------
' Revisions
' ------------------------------------------------
' rev   by   date       description
' ----  ---  ------     --------------------------
' 0.01  elr  02/28/97   original design
' ------------------------------------------------
Private Sub MDIForm_Load()

Load WSDOT_LOGO
MsgBox "  Loading Tide Data. . . . . . . . . . . . . . . . . . . .   "
Initialize          ' general program init function
FetchConfig         ' get cable input data
FetchDayTides       ' get the tides for current day
GetNowTide          ' get the tide for this 10-min period
' -------------------------------------------------
msg = " Are wind and tide ( " & CrntTide & " FEET ) at acceptable levels ? "
Style = vbQuestion + vbYesNo
Title = "Verify Wind and Tide"
Response = MsgBox(msg, Style, Title)
If Response = vbNo Then End
' -------------------------------------------------
msg = " Please verify current date and time.  If system date or time need to be reset, EXIT the program and correct these settings. " & Format(Date, "dddd, mm/dd/yy") & " - " & Format(Time, "hh:mm:ss") & Chr(10) & "(USE STANDARD TIME ONLY; DO NOT USE DAYLIGHT SAVINGS TIME). Is this correct ?"
Style = vbInformation + vbYesNo
Title = "Verify System Date and Time  -  Note: Time MUST be in PST and NOT in PDT"
Response = MsgBox(msg, Style, Title)
If Response = vbNo Then End
' --------------------------------------------------
CalcTension         ' go calc all cable tensions for crnt tide
'Load WSDOT_LOGO     ' and load the big list box

End Sub
