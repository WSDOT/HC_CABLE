Attribute VB_Name = "Module1"
' ------------------------------------------------------
'       Copyright(C) 1997 HNTB Corporation
' ------------------------------------------------------
' MODULE1.BAS - Public functions, constants and vars
' ------------------------------------------------------
' Revisions
' ------------------------------------------------------
' rev   by   date       description
' ----  ---  ------     --------------------------------
' 0.01  elr  02/28/97   original design
' 0.02  elr  06/03/97   enabled config and tide data
'                       file readers, added to equation
'                       processors.
' 1.0   elr  02/10/98   revised window sizes, disabled
'                       diag code for final release.
' ------------------------------------------------------
' Public Constants
' ------------------------------------------------------
Public Const DEV = True
Public Const NumCables = 42         '
Public Const Found = True           '
Public Const ofstGaugeL = 4         '
Public Const ofstTensionL = 11      '
Public Const ofstPairNam = 18
Public Const ofstTensionR = 28      '
Public Const ofstGaugeR = 35        '
Public Const ofst1 = 3              '
Public Const ofst2 = 11             '
Public Const ofst3 = 20             '
Public Const ofst4 = 27             '
Public Const ofst5 = 37             '
Public Const ofst6 = 46             '
Public Const ofst7 = 49             '
Public Const ofst8 = 58             '
Public Const ofst9 = 68             ' from "69" ; changed 2/3/98
Public Const ofst10 = 74            '
Public Const ofst11 = 85            '
Public Const ofst12 = 93            '

Public Const len1 = 3               '
Public Const len2 = 4               '
Public Const len3 = 2               '
Public Const len4 = 6               '
Public Const len5 = 4               '
Public Const len6 = 3               '
Public Const len7 = 8               '
Public Const len8 = 9               '
Public Const len9 = 6               ' from "5" ; changed 2/3/98
Public Const len10 = 8              ' from "6" ; changed 2/3/98
Public Const len11 = 5              '
Public Const len12 = 5              '

Public Const lenTension = 3         '
Public Const lenGauge = 4           '
Public Const lenPairNam = 7
Public Const TideRecLen = 55        ' Len of one tide record line
                                    ' Following are Th tune loop constants
Public Const Thi_Gain_Limit = 20000 ' max adjustment for Th
Public Const Th_Thi_Diff = 0.002    ' percent diff Th:Thi / 100
Public Const Th_Step = 0.02         ' Th tune step rate in KIPS
' -------------------------------------------------------
' Public Variables
' -------------------------------------------------------
Public bDebugPrint  As Boolean      '
Public bDebugPrint2 As Boolean      '
Public bDebugPrint3 As Boolean      '
Public bDebugPrint4 As Boolean      ' dump Th, UL and L values
Public bDebugPrint5 As Boolean      ' for Th, UL and L file out dump

Public PassWord As String           ' diagnostic password
Public bManualTideAdjust As Boolean '
Public bPgmJustStarting As Boolean  '
Public TideElev As Single           '
Public Tension As Single            '
Public CableName(42) As String      '
Public HDfeet(42) As Single         '
Public OFFSETfeet(42) As Single     '
Public Hfeet(42) As Single          '
Public DEPTHfeet(42) As Single      '
Public CORRfeet(42) As Single       '
Public DNfeet(42) As Single         '
Public W_Kfeet(42) As Single        '
Public Thi_KIPS(42) As Single       '
Public A_SQ_IN(42) As Single        ' Cable area array
Public E_KSI(42) As Single          ' Modulus of elasticity array
Public Area(42) As Single           ' Area array
Public TensionAry(42) As Single
Public GaugeAry(42) As Single
Public PairNam(21) As String

Public TideArray(24, 6) As Single   ' tide values for entire day (24 hrs x 6 values / hr)
Public TblOfst(6) As Single         ' column offset into tide table row
Public CrntTide As Single           ' current tide elev value
Public CrntCable As Single          ' index for current cable
Public Str_CalcTime As String       '
Public STR_Tide As String           '
Public bTimesUp As Boolean          '
Public SelCable As Single

' ------------------------------------
' Vars for equations
' ------------------------------------
Public H As Single                  '
Public D As Single                  '
Public deltaL As Single             '
Public Lc As Single                 '
Public Lci As Single                '
Public Li As Single                 '
Public deltaLi As Single            '
Public tan_alpha As Single          '
Public cos_alpha As Single          '
Public sec_alpha As Single          '
Public K As Single                  '
Public l As Single                  '
Public W As Single                  '
Public A As Single                  '
Public E As Single                  '
Public T As Single                  '
Public Th As Single                 '
Public Th_calc As Single            '
Public Thi As Single                '
Public tan_beta As Single           '
Public UL As Single                 '
Public gauge As Single              '
Public Wh As Single                 '
Public Th_factor As Single          ' + or - 1 to determine direction of adjustment


' -----------------------------------------
' Initialization routine for main program
' -----------------------------------------
Public Sub Initialize()
  TblOfst(1) = 19
  TblOfst(2) = 25
  TblOfst(3) = 31
  TblOfst(4) = 37
  TblOfst(5) = 43
  TblOfst(6) = 49
  bTimesUp = False
  bPgmJustStarting = True
  
PairNam(0) = "N -A- S"
PairNam(1) = "N -B- S"
PairNam(2) = "N -C- S"
PairNam(3) = "N -D- S"
PairNam(4) = "N -E- S"
PairNam(5) = "N -F- S"
PairNam(6) = "N -G- S"
PairNam(7) = "N -H- S"
PairNam(8) = "N -J- S"
PairNam(9) = "N  L1 S"
PairNam(10) = "N  L2 S"
PairNam(11) = "N -M- S"
PairNam(12) = "NW N SW"
PairNam(13) = "NE N SE"
PairNam(14) = "N -O- S"
PairNam(15) = "N -P- S"
PairNam(16) = "N -Q- S"
PairNam(17) = "N -R- S"
PairNam(18) = "N -S- S"
PairNam(19) = "N -T- S"
PairNam(20) = "N -U- S"
  
End Sub


' -------------------------------------------------
' Opens config file "WSDOT.CFG" and loads data for
'  each cable.
' -------------------------------------------------
Public Sub FetchConfig()

Dim STR_BFR As String
Dim i As Integer
Dim TestStr As String
Dim Chr_Index As Integer

' ---- get contents of config file into bfr & close file
  Open "c:\Progra~1\wsdotbrg\WSDOT.CFG" For Binary Access Read As #1
  STR_BFR = String(4500, " ")
  Get #1, , STR_BFR
  Close #1

' ---- open debug data dump file
'  Open "c:\Progra~1\wsdotbrg\outfile.txt" For Binary Access Write As #1

' ---- skip config file header
  For i = 1 To 4        ' allow up to four lines of header
    If Mid(STR_BFR, 1, 1) = "#" Then
      Chr_Index = InStr(STR_BFR, Chr(13))
      STR_BFR = Mid(STR_BFR, Chr_Index + 2, Len(STR_BFR) - Chr_Index)
    End If
  Next i
  
' ---- extract cable data
  For i = 0 To NumCables - 1
    CableName(i) = Mid(STR_BFR, ofst1, len1)
'    HDfeet(i) = Mid(STR_BFR, ofst2, len2)
'    OFFSETfeet(i) = Mid(STR_BFR, ofst3, len3)
    Hfeet(i) = Mid(STR_BFR, ofst4, len4)
'    DEPTHfeet(i) = Mid(STR_BFR, ofst5, len5)
'    CORRfeet(i) = Mid(STR_BFR, ofst6, len6)
    DNfeet(i) = Mid(STR_BFR, ofst7, len7)
    W_Kfeet(i) = Mid(STR_BFR, ofst8, len8)
    Thi_KIPS(i) = Mid(STR_BFR, ofst9, len9)
    A_SQ_IN(i) = Mid(STR_BFR, ofst10, len10)
    E_KSI(i) = Mid(STR_BFR, ofst11, len11)
    Area(i) = Mid(STR_BFR, ofst12, len12)
    
    Chr_Index = InStr(STR_BFR, Chr(13))
    STR_BFR = Mid(STR_BFR, Chr_Index + 2, Len(STR_BFR) - Chr_Index)

  Next i
End Sub
' --------------------------------------------
' Stamp current date/time into 'Calculation Time' in window.
' --------------------------------------------
Public Sub StampCalcTime()
Str_CalcTime = Format(Date, "dddd, mm/dd/yy") & " - " & Format(Time, "hh:mm:ss")
STR_Tide = CrntTide & " ft"
End Sub
' --------------------------------------------
Public Sub FetchDayTides()

Dim STR_BFR As String
Dim i, A, b As Integer
Dim cp As Variant

Dim Chr_Index As Integer
Dim day As Integer
Dim month As Integer
Dim tidedate As Boolean
Dim STR_DATE As String
Dim sAccum As String

' ------------------
' scan for date
' ------------------

tidedate = False
CalcChksum  ' verify the chksum of the tide table file

'Open "c:\Progra~1\yeartide\year" & Format(Date, "yy") & ".tid" For Binary Access Read As #1
Open "c:\ARP\yeartide\year" & Format(Date, "yy") & ".tid" For Binary Access Read As #1

'--------
' first, check the checksum of the tide data file (YEARxx.TID)
'cp = 1
'For i = 1 To LOF(1) - 7
'
'Next i
'--------

cp = 1

Do
  STR_BFR = String(TideRecLen, " ")
  Get #1, cp, STR_BFR
  cp = cp + TideRecLen
'  'Debug.Print Mid(STR_BFR, 1, 8)
' ------------------------------------------
  STR_DATE = String(8, " ")
  Mid(STR_DATE, 1, 8) = Format(Date, "mm/dd/yy")
  
  If Mid(STR_BFR, 1, 8) = STR_DATE Then
    tidedate = Found
    Exit Do
  End If
  
  If EOF(1) Then
    msg = "End of tide data file (YEARxx.TID) reached without finding tide data for this date."
    Style = vbexclaimation
    Title = "Tide Data File ERROR ! !"
    Response = MsgBox(msg, Style, Title)
    'end of file error
    End
  End If
Loop Until tidedate = Found

' ---- get selected contents of Tide Data file into bfr & close file
  cp = cp - TideRecLen      ' decrement the cp back to head of this record
  For i = 1 To 24           ' 1 line / hour, 24 hours / day
  STR_BFR = String(TideRecLen, " ")
  Get #1, cp, STR_BFR
  STR_BFR = Mid(STR_BFR, 1, TideRecLen - 2)
  For b = 1 To 6
    TideArray(i, b) = Mid(STR_BFR, TblOfst(b), 5)
  Next b
  
  'For b = 1 To 6
  'Debug.Print TideArray(i, b)
  'Next b
  
  cp = cp + TideRecLen
  Next i
  
  Close #1

End Sub

' --------------------------------------
' Load current tide elevation value and
' current cable parameters.
' --------------------------------------
Public Sub LoadCrntValues()
' ----------------------------
' DEV: Dummy value for testing
' ----------------------------
  GetNowTide
  'CrntTide = 13.2
  'CrntCable = 32

End Sub

' ---------------------------------

Public Sub GetNowTide()
  Dim Thour As Single
  Dim TTenMin As Single
    
  Thour = Hour(Time)
  TTenMin = Int(Minute(Time) / 10)
      
  ' load the next day's tide data if at the end of this day
  If (Thour = 0 And TTenMin = 0) Then
    FetchDayTides
  End If

  If bManualTideAdjust = False Then
    CrntTide = TideArray(Thour + 1, TTenMin + 1)
  End If
  
End Sub
' ---------------------------------
' Calculate Tension Value
' ---------------------------------
Public Sub CalcTension()
Dim i As Integer
  
  LoadCrntValues
  StampCalcTime

  For i = 0 To 41
  CrntCable = i
  'WSDOT_ALL.lblProgressIndicator.Caption = WSDOT_ALL.lblProgressIndicator.caption & "."
    
  Fn_1                  ' get D and Lci
  Fn_2                  ' calc K
  Fn_3                  ' calc Li
  Fn_4                  ' calc deltaLi
  Fn_5                  ' calc UL
  Fn_6a                 ' Calc Lc
  Fn_6b                 ' tweak Th
  'Fn_6c                 ' Solve K
  'Fn_6d                 ' Solve L
  'Fn_6e                 ' calc deltaL
  'Fn_6f                 ' Solve Th
  'Fn_7                  ' Calc Tension
  'Fn_8                  ' Calc Gauge pressure
If bDebugPrint = True Then
  Debug.Print "Cable: " & CableName(CrntCable) & "  Tension: " & T & "  Gauge: " & gauge & "  Tide: " & CrntTide
End If
  Next i
End Sub

' ---------------------------------
' Cable Tension Function 1
' ---------------------------------
Public Sub Fn_1()
  Lci = Sqr(Hfeet(CrntCable) ^ 2 + DNfeet(CrntCable) ^ 2)
  If bDebugPrint = True Then
    Debug.Print "Fn_1: D= " & DNfeet(CrntCable) & " Lci= " & Lci
  End If
End Sub

' ---------------------------------
' Cable Tension Function 2
' ---------------------------------
Public Sub Fn_2()
  K = (W_Kfeet(CrntCable) * Hfeet(CrntCable) ^ 2) / (8 * Thi_KIPS(CrntCable) * Lci)
If bDebugPrint = True Then
  Debug.Print "Fn_2: K= " & K
End If

End Sub

' ---------------------------------
' Cable Tension Function 3
' ---------------------------------
Public Sub Fn_3()
  Li = Lci * (1 + ((8 / 3) * K ^ 2) - ((32 / 5) * K ^ 4) + ((256 / 7) * K ^ 6))
If bDebugPrint = True Then
  Debug.Print "Fn_3: Li= " & Li
End If

End Sub

' ---------------------------------
' Cable Tension Function 4
' ---------------------------------
Public Sub Fn_4()
  deltaLi = (Thi_KIPS(CrntCable) * Li ^ 2) / (Hfeet(CrntCable) * A_SQ_IN(CrntCable) * E_KSI(CrntCable))
If bDebugPrint = True Then
  Debug.Print "Fn_4: Thi(" & CrntCable & ")= " & Thi_KIPS(CrntCable) & ", H()= " & Hfeet(CrntCable) & ", E()= " & E_KSI(CrntCable) & ", deltaLi= " & deltaLi
End If

End Sub

' ---------------------------------
' Cable Tension Function 5
' ---------------------------------
Public Sub Fn_5()
UL = Li - deltaLi
If bDebugPrint = True Then
  Debug.Print "Fn_5: UL= " & UL & ", Li=" & Li & ", deltaLi=" & deltaLi & ", Cable=" & CableName(CrntCable)
End If

End Sub

' ---------------------------------
' Cable Tension Function 6a
' ---------------------------------
Public Sub Fn_6a()
  D = DNfeet(CrntCable) - 5.7 + CrntTide
  Lc = Sqr(Hfeet(CrntCable) ^ 2 + D ^ 2)
  If bDebugPrint = True Then
    Debug.Print "Fn_6a: Lc= " & Lc & " Cable= " & CableName(CrntCable)
  End If
  
End Sub

' ---------------------------------
' Cable Tension Function 6b
' ---------------------------------
Public Sub Fn_6b()
Dim j As Integer

  Select Case CrntTide
    Case Is > 5.7
      Th_factor = Th_Step
      
    Case Is < 5.7
      Th_factor = -(Th_Step)
    Case Is = 5.7
      Th_factor = 0
  End Select
  
  Th = Thi_KIPS(CrntCable)
  j = 0         ' DEV: test index to trap data
  
  If bDebugPrint3 = True Then
    Debug.Print "---------------------------------------------------------------------------------"
  End If
  
  Do
    Th = Th + (Th_factor) ' assumed th
    If j < 3 Then
      If bDebugPrint2 = True Then
         Debug.Print "Fn_6b1: Cbl= " & CableName(CrntCable) & " Th= " & Th & " Th_calc= " & Th_calc & " Diff= " & (Th - Th_calc) & " %= " & (Th / (Abs(Th - Th_calc)))
      End If
    End If
    Fn_6c                              ' solve K
    Fn_6d                              ' solve L
    Fn_6e                              ' solve delta L
    Fn_6f                              ' solve Th
    If bDebugPrint3 = True Then
          If Abs(Th - Th_calc) < (Th * Th_Thi_Diff * 1.5) Then
            Debug.Print "Fn_6b3: Cable= " & CableName(CrntCable) & " Th= " & Th & " Th_calc= " & Th_calc & " Diff= " & Format((Th - Th_calc), "#0.00") & " %= " & Format((Abs(Th - Th_calc) / Th), "0.00%")
          End If
    End If
    If Abs(Th - Th_calc) < (Th * Th_Thi_Diff) Then
      Exit Do
    End If
    
    j = j + 1   'DEV:
    
  Loop Until (Abs(Th - Th_calc) > Thi_Gain_Limit) ' which means we
  
  If bDebugPrint2 = True Then
    Debug.Print "Fn_6b2: Cable= " & CableName(CrntCable) & " Th= " & Th & ", Th_calc= " & Th_calc & ", Limit= " & Thi_Gain_Limit
  End If
                                                            '  went too far...
 If Abs(Th - Th_calc) > Thi_Gain_Limit Then
   'MsgBox "Limit reached in Th adjustment loop for cable " & CableName(CrntCable)
   Th = -1          ' as a flag to abort tension calc for that cable
 End If
  

 Fn_7                                  ' Tension Calc
 Fn_8                                  ' Gauge Calc
 
End Sub

' ---------------------------------
' Cable Tension Function 6c
' ---------------------------------
Public Sub Fn_6c()
  K = (W_Kfeet(CrntCable) * Hfeet(CrntCable) ^ 2) / (8 * Th * Lc)
  If bDebugPrint = True Then
    Debug.Print "Fn_6c: K= " & K & " Cable= " & CableName(CrntCable)
  End If
End Sub

' ---------------------------------
' Cable Tension Function 6d
' ---------------------------------
Public Sub Fn_6d()
  l = Lc * (1 + ((8 / 3) * K ^ 2) - ((32 / 5) * K ^ 4) + ((256 / 7) * K ^ 6))
  If bDebugPrint = True Then
    Debug.Print "Fn_6d: L= " & l
  End If
End Sub

' ---------------------------------
' Cable Tension Function 6e
' solve for delta L
' ---------------------------------
Public Sub Fn_6e()
  deltaL = l - UL
  If bDebugPrint = True Then
    Debug.Print "Fn_6e: deltaL= " & deltaL & ", L=" & l & ", UL=" & UL
  End If
End Sub

' ---------------------------------
' Cable Tension Function 6f
' solve for Th
' ---------------------------------
Public Sub Fn_6f()
  If bDebugPrint = True Then
    Debug.Print "Fn_6f: deltaL= " & deltaL & ", Hfeet()= " & Hfeet(CrntCable) & ", A= " & A_SQ_IN(CrntCable) & ", E= " & E_KSI(CrntCable) & ", L= " & l
  End If
  Th_calc = (deltaL * Hfeet(CrntCable) * A_SQ_IN(CrntCable) * E_KSI(CrntCable)) / l ^ 2
  If bDebugPrint = True Then
    Debug.Print "Fn_6f: Th_calc= " & Th_calc
  End If

End Sub

' ---------------------------------
' Cable Tension Function 7
' Tension Calculation
' ---------------------------------
Public Sub Fn_7()
  If Th = -1 Then   ' if Th calc was in error, then
    T = 0           ' give Tension a value of zero to indicate error
  End If
  tan_beta = ((W_Kfeet(CrntCable) * Lc * Hfeet(CrntCable)) + (2 * Th_calc * D)) / (2 * Th_calc * Hfeet(CrntCable))
  Beta = Atn(tan_beta) * (180 / 3.14159)
  If bDebugPrint = True Then
    Debug.Print "Fn_7: tan_beta= " & tan_beta & " Beta= " & Beta & " degrees."
  End If
  T = Th / Cos(Beta * (3.14159 / 180))
  TensionAry(CrntCable) = T
  If bDebugPrint = True Then
    Debug.Print "Fn_7: Tension= " & T & " Cable= " & CableName(CrntCable)
  End If
End Sub

' ---------------------------------
' Cable Tension Function 8
' Gauge Pressure
' ---------------------------------
Public Sub Fn_8()
  gauge = (T * 1000) / Area(CrntCable)
  GaugeAry(CrntCable) = gauge
  If bDebugPrint = True Then
    Debug.Print "Fn_8: Gauge= " & gauge & " Cable= " & CableName(CrntCable)
  End If
  
  If bDebugPrint4 = True Then
    Debug.Print "Fn_8a: Cable= " & CableName(CrntCable) & " Tide= " & CrntTide & " Th= " & Th & " UL= " & UL & " L= " & l & " T= " & T & " (" & CInt(T) & ")"
  End If
  
  If bDebugPrint5 = True Then
    strbfr = "Fn_8a: Cable= " & CableName(CrntCable) & " Tide= " & CrntTide & " Th= " & Th & " UL= " & UL & " L= " & l & " T= " & T & " (" & CInt(T) & ")"
    Print #1, strbfr
    

  End If
  
End Sub

Public Sub CalcChksum()
  'sSourcePath = App.Path
  Dim i As Integer
  Dim sLineBfr As String
  Dim ChkSum As Long        ' to hold the just-calculated checksum
  Dim FileChkSum As String  ' to hold the file checksum
  
'  Open "\Progra~1\Yeartide\Year" & Format(Date, "yy") & ".tid" For Input As #1
  Open "C:\ARP\Yeartide\Year" & Format(Date, "yy") & ".tid" For Input As #1

  Dim ChkSumThisLine As Long

  On Error Resume Next
  Do While Not EOF(1)
    Line Input #1, sLineBfr
    ChkSumThisLine = 0
    For i = 1 To 55
      sChrBfr = Mid(sLineBfr, i, 1)
      ChkSumThisLine = ChkSumThisLine + Asc(sChrBfr)
    Next i
    ChkSumThisLine = ChkSumThisLine + Asc(Chr$(13)) + Asc(Chr$(10))
    ChkSum = ChkSum + ChkSumThisLine
  
  Loop
  
  'Exclude checksum value
  ChkSum = ChkSum - ChkSumThisLine

  FileChkSum = sLineBfr
    
  If Val("&H" & FileChkSum) <> ChkSum Then
      MsgBox "Tide data file (YEARxx.TID) has an invalid checksum."
      End
  End If
  Close #1
  
End Sub

' --------------------------------------------------
'     END OF MODULE1.BAS
' --------------------------------------------------

