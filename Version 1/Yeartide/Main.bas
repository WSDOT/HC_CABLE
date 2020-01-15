Attribute VB_Name = "Module1"
'Jacob E.Thomas
'Copyright 1991 - 1997
'All Rights Reserved

'Licensed to HNTB Corporation and the State of Washington for engineering purposes.

'Commercial distribution of the compiled program or publication of the program code
'in either printed or elecronic form is prohibited.

Option Explicit

'Correction factors may be adjusted based on observations
Public Const sPlaceName As String = "Port Gamble"
Public Const sPlaceNumber As String = "1011"
Public Const sStationRef As String = "1814"      'Correction on Seattle Tides
Public Const sHighTime As String = "-009"
Public Const sLowTime As String = "-005"
Public Const sHighLevel As String = "0.90"
Public Const sLowLevel As String = "0.95"

Type DecimalTideType
  iDecTime As Integer
  sngHeight As Single
End Type

Public HighLowArray() As DecimalTideType
Public BridgeArray() As DecimalTideType
Public sColumnNameArray() As String * 2
Public sDataArrayT() As String

Public iBridgeCounter As Integer
Public iDecimalTime As Integer
Public iOutOfData As Integer
Public iFirstDay As Integer
Public iLastPrediction As Integer
Public iSwitchHourTest As Integer
Public iOldHour As Integer
Public iXValue3 As Integer
Public sngYValue3 As Single

Public sPredictionDate As String
Public sPredictionHour As String
Public sVariablesData1 As String
Public sVariablesData2 As String
Public sVariablesData3 As String
Public lCheckSum As Long


Sub Load3DayArray()

'simple routine to load array containing three days of
'consecutive high and low predictions for Seattle

'first element is high/low indicator
'second element is time
'third element is height

Dim iRow As Integer
Dim iStart As Integer
Dim iSaveRow As Integer
Dim sDayData As String
Dim sHighLowBit As String
Dim sTimeColumn As String
Dim sHeightColumn As String
Dim iRecord As Integer
Dim i As Integer

ReDim sDataArrayT(1 To 18, 1 To 3) As String

On Local Error GoTo ExitLine14
''Err.Raise 6 ' Generate an "Overflow" error.

iStart = 16

For iRecord = 1 To 3
    Select Case iRecord
        Case 1
            sDayData = sVariablesData1
            sHighLowBit = Mid$(sVariablesData1, 2, 1)
            iRow = 0
        Case 2
            sDayData = sVariablesData2
            sHighLowBit = Mid$(sVariablesData2, 2, 1)
            iRow = iSaveRow
        Case 3
            sDayData = sVariablesData3
            sHighLowBit = Mid$(sVariablesData3, 2, 1)
            iRow = iSaveRow
    End Select

    iRow = iRow + 1
    For i = 1 To 6
        sTimeColumn = Mid$(sDayData, iStart + 1, 4)
        iStart = iStart + 5
        sHeightColumn = Mid$(sDayData, iStart + 1, 4)
        iStart = iStart + 5
        If sTimeColumn <> "9999" Then
            sDataArrayT(iRow, 1) = sHighLowBit
            sDataArrayT(iRow, 2) = sTimeColumn
            sDataArrayT(iRow, 3) = sHeightColumn
            If sHighLowBit = "1" Then
                sHighLowBit = "0"
            Else
                sHighLowBit = "1"
            End If
            If i < 6 Then
                iRow = iRow + 1
            Else
                iSaveRow = iRow
            End If
        Else
            iSaveRow = iRow - 1
            Exit For
        End If
    Next i
iStart = 16
Next iRecord


'DebugX routine to print out raw data for reference station
'rem out at compile time

''Dim DebugX As Integer
''Dim Row As Integer
''DebugX = True
''If DebugX Then
''  Debug.Print
''  Debug.Print "H/L", "Time", "Elev        "
''  For Row = 1 To 18 'test
''    If sDataArrayT(Row, 2) <> "9999" Then
''        Debug.Print sDataArrayT(Row, 1), sDataArrayT(Row, 2), sDataArrayT(Row, 3)  'test
''    End If
''  Next Row  'test
''End If
Exit Sub

ExitLine14:
  Error Err
End Sub


Public Sub Main()
    
    Dim sPredictionMonth As String
    Dim sPredicyionDay As String
    Dim sPredictionYear As String
    Dim sMsg As String
    Dim sSourcePath As String
    Dim sOutputPath As String
    Dim sSourceFile As String
    Dim sWholeLine As String
    Dim iColumnTime As Integer
    Dim sColumnNames As String
    Dim sStation As String
    Dim i As Integer
    Dim k As Integer
    Dim sTemp As String
    Dim sTemp1 As String
    Dim sTemp2 As String
    Dim sTemp3 As String

    ReDim sColumnNameArray(1 To 6) As String * 2
    
    On Error GoTo ErrorLine
    frmMain.Show
    frmMain.ProgressBar1.Value = frmMain.ProgressBar1.Min

    sSourcePath = App.Path
    sOutputPath = App.Path
    sSourceFile = InputBox("Enter Source File", "Source File", "SEATTLE.T" & Format(Date, "yy"))
    If sSourceFile = "" Then End
    Open sSourcePath & "\" & sSourceFile For Input As #1
    Line Input #1, sWholeLine
    Close #1
    
    sPredictionYear = Mid$(sWholeLine, 14, 2)
    If Left(sPredictionYear, 1) = " " Then
        sPredictionYear = Replace(sPredictionYear, " ", "0", 1, -1, vbTextCompare)
    End If
    
    
    'NOAA's data format is not "Year 2000 compliant"
    sStation = Mid$(sWholeLine, 4, 4)

    If sStation <> sStationRef Then
        MsgBox "NOAA data file station number not recognized as Seattle"
    End If
    
    Open sOutputPath & "\" & "YEAR" & sPredictionYear & ".TID" For Output As #2
        
    sColumnNames = "DATE     " & "TimeStmp "
    For k = 1 To 6
        sColumnNames = sColumnNames & Format((iColumnTime), "00:00") & " "
        sColumnNameArray(k) = Format((iColumnTime), "00")
        iColumnTime = iColumnTime + 10
    Next k
    ''Print #2, sColumnNames$
    
       
    iFirstDay = True
    
    'create null data strings
    sVariablesData1 = "09 1814  0  097 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
    sVariablesData2 = "09 1814  0  097 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
    sVariablesData3 = "09 1814  0  097 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
    Open sSourcePath & "\" & sSourceFile For Input As #1
    Screen.MousePointer = 11
    
    'main program routine
    Do While Not EOF(1)
    'load three days of Seattle data into three variables
    'include day befor and day after
        Line Input #1, sWholeLine
        sVariablesData1 = sVariablesData2
        sVariablesData2 = sVariablesData3
        sVariablesData3 = sWholeLine
        'parse the data
        sStation = Mid$(sWholeLine, 4, 4)
        sPredictionMonth$ = Format(Mid$(sVariablesData2, 12, 2), "00")
        sPredicyionDay$ = Format(Mid$(sVariablesData2, 9, 2), "00")
        sPredictionYear = Format(Mid$(sVariablesData2, 14, 2), "00")
        sPredictionDate$ = sPredictionMonth$ & "/" & sPredicyionDay$ & "/" & sPredictionYear
        frmMain.Caption = "Port Gamble Tides for " & sPredictionDate$
        
    'if first day contains null data, use text from frmMain
        If Mid$(sVariablesData1, 1, 1) = "0" Then
            If Mid$(sVariablesData2, 1, 1) <> "0" Then
                If frmMain.Text1(0).Text <> "9" Then
                    If InStr(UCase(frmMain.Text1(0).Text), "H") Then
                        sTemp1 = "0"
                    ElseIf InStr(UCase(frmMain.Text1(0).Text), "L") Then
                        sTemp1 = "1"
                    Else
                        sTemp1 = "999999999999999999999"
                    End If
                    sTemp2 = frmMain.Text1(1).Text
                    sTemp3 = frmMain.Text1(2).Text
                    sVariablesData1 = "9" & sTemp1 & " 1814  0  097 " & sTemp2 & " " & sTemp3 & " 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
                    If Len(sVariablesData1) <> 80 Then
                        sVariablesData1 = "09 1814  0  097 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
                    End If
                End If
                If Mid$(sVariablesData1, 2, 1) = "9" Then
                    iFirstDay = True
                Else
                    iFirstDay = False
                End If
            End If
        End If
    'process the data
        If Mid$(sVariablesData2, 1, 1) <> "0" Then
            frmMain.ProgressBar1.Value = Val(Mid$(sWholeLine, 78, 3))
            ''If sPredictionDate$ < "01/06/97" Then 'test1
        'load the Seattle data into an array
            Call Load3DayArray
        'calculate corrections for Port Gamble
            Call Calc3DayHighLow
        'connect the high and low values with cosine curve
            Call RunCurve1Day
            ''End If 'test2
        End If
        ''sTemp = MsgBox("Coninue?", 1)
        ''If sTemp = 2 Then End
    Loop
    
    'routine for last day (out of data) to load "day after" variable
    sVariablesData1 = sVariablesData2
    sVariablesData2 = sVariablesData3
    
    If frmMain.Text2(0).Text <> "9" Then
        If InStr(UCase(frmMain.Text2(0).Text), "H") Then
            sTemp1 = "0"
        ElseIf InStr(UCase(frmMain.Text2(0).Text), "L") Then
            sTemp1 = "1"
        Else
            sTemp1 = "999999999999999999999"
        End If
        sTemp2 = frmMain.Text2(1).Text
        sTemp3 = frmMain.Text2(2).Text
        sVariablesData3 = "9" & sTemp1 & " 1814  0  097 " & sTemp2 & " " & sTemp3 & " 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
        If Len(sVariablesData3) <> 80 Then
            sVariablesData3 = "09 1814  0  097 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
        End If
    Else
        sVariablesData3 = "09 1814  0  097 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9 9999 99.9    0"
    End If
    
    sStation = Mid$(sWholeLine, 4, 4)
    sPredictionMonth = Format(Mid$(sVariablesData2, 12, 2), "00")
    sPredicyionDay = Format(Mid$(sVariablesData2, 9, 2), "00")
    sPredictionYear = Format(Mid$(sVariablesData2, 14, 2), "00")
    sPredictionDate = sPredictionMonth$ & "/" & sPredicyionDay$ & "/" & sPredictionYear
    frmMain.Caption = "Port Gamble Tides for " & sPredictionDate$
 
    'process last day
    If Mid$(sVariablesData2, 1, 1) <> "0" Then
        Call Load3DayArray
        Call Calc3DayHighLow
        Call RunCurve1Day
    End If
    
    'calculate final check sum
    sTemp = "        "
    sTemp = Hex(lCheckSum)
'    For i = 8 - Len(sTemp) To 1 Step -1
'        sTemp = " " & sTemp
'    Next i
    
'    Dim l As Long
'    For i = 1 To 8
'     l = l + Asc(Mid$(sTemp, i))
'    Next i
'    Print #3, l, lCheckSum - l
'
'    l = Asc(Chr$(13)) + Asc(Chr$(10))
'    Print #3, l, lCheckSum - l
    
    Print #2, sTemp & Chr$(13) & Chr$(10);
    'Debug.Print Hex(lCheckSum)
    'Debug.Print "*" & sTemp & "*"
    
    Screen.MousePointer = 0

    MsgBox ("Last Day")
    Close #1
    Close #2
    End
Exit Sub

ErrorLine:
    Screen.MousePointer = 0
    If Err.Number <> 0 Then
        sMsg = "Error # " & Str(Err.Number) & " was generated by " & Err.Source & Chr(13) & Err.Description
        MsgBox sMsg, , "Error"
    End If
    Err.Clear
End Sub


Sub Calc3DayHighLow()
    
    Dim sHighOrLow As String
    Dim sTideTime As String
    Dim sTideHeight As String
    Dim sTimeAdjust As String
    Dim sHeightAdjust As String
    Dim i As Integer
    
    ReDim HighLowArray(18) As DecimalTideType
    On Local Error GoTo ExitLine13
    ''Err.Raise 6 ' Generate an "Overflow" error.

    'SwitchHourTest is variable to mark the transition to day two
    If iFirstDay Then
    'if it is January 1 and there is no data for December 31,
    'then there is no day one
        iSwitchHourTest = True
        iFirstDay = False
    Else
        iSwitchHourTest = False
    End If
    
    iLastPrediction = False: iOldHour = -1
    
    For i = 1 To 18
    
        If iLastPrediction Then Exit For
        
    'obtain data from the array
        sHighOrLow = sDataArrayT(i, 1)
        sTideTime = sDataArrayT(i, 2)
        sTideHeight = sDataArrayT(i, 3)
        
    'determine appropriate correction factors
        If sHighOrLow = "1" Then
            sTimeAdjust = sLowTime
            sHeightAdjust = sLowLevel
        ElseIf sHighOrLow = "0" Then
            sTimeAdjust = sHighTime
            sHeightAdjust = sHighLevel
        End If
        
        If sTideTime = "" Or sTideHeight = "" Then
            iOutOfData = True
        End If
        
    'calculate the time correction for Port Gamble
        sTideTime = Calc1Time$(sTideTime, sTimeAdjust)
        
    'calculate the height correction for Port Gamble
        sTideHeight = Format(Val(sTideHeight) * Val(sHeightAdjust), "#0.0")
        ''Debug.Print sTideTime
        ''Debug.Print sTideHeight
    'place the Port Gamble prediction into an array where time is
    'expressed as decimal number
        If iOutOfData = False Then
            HighLowArray(i).iDecTime = iDecimalTime 'Integer
            HighLowArray(i).sngHeight = Val(sTideHeight) 'Single
        Else
            HighLowArray(i).iDecTime = 9999 'Integer
            HighLowArray(i).sngHeight = "9999" 'Single
        End If
    Next i
Exit Sub

ExitLine13:
  Error Err
End Sub


Function Calc1Time(sStnRefTime As String, sTimeCorrection As String) As String
    'time arithmetic function to calculate time difference from Seattle

    Dim sMinutes As String
    Dim iStnRefTimeHrs As Integer
    Dim iStnRefTimeMin As Integer
    Dim iTimeCorrectionHrs As Integer
    Dim iTimeCorrectionMin As Integer
    Dim iHours  As Integer
    Dim iMin As Integer
    Dim iDecimalMinutes As Integer
    Dim iCurveHours As Integer
    Dim sPredictTime24hr As String
    Dim sTempTime As String
    
    On Local Error GoTo ExitLine12
    ''Err.Raise 6 ' Generate an "Overflow" error.

    iStnRefTimeHrs = Val(Left$(sStnRefTime, 2))
    iStnRefTimeMin = Val(Right$(sStnRefTime, 2))
    iTimeCorrectionHrs = Val(Left$(Right$(sTimeCorrection, 4), 2))
    iTimeCorrectionMin = Val(Right$(sTimeCorrection, 2))
    
    If Left$(sTimeCorrection, 1) = "-" Then
        iTimeCorrectionMin = -iTimeCorrectionMin
    End If
    
    iHours = iStnRefTimeHrs + iTimeCorrectionHrs
    iMin = iStnRefTimeMin + iTimeCorrectionMin
    
    If iMin >= 0 And iMin < 60 Then
        'Do nothing
    ElseIf iMin >= 60 Then
        iHours = iHours + 1
        iMin = iMin - 60
    ElseIf iMin < 0 Then
        iHours = iHours - 1
        iMin = iMin + 60
    End If
    
    If iHours >= 24 Then
        iHours = iHours - 24
    End If
        
    'convert ##:## time to decimal time
    iDecimalMinutes = iMin * 1.66666 'times 100 / 60
    
    'SwitchHourTest is True when the predicted time falls within day two
    'When the next hour is a number smaller than the previous hour, the
    'predictuion falls witin day three, so quit
    If iSwitchHourTest Then
        If iHours < iOldHour Then
            iOldHour = iHours
            Calc1Time = "9999"
            iCurveHours = iHours + 48
            iDecimalTime = iCurveHours * 100 + iDecimalMinutes
            iSwitchHourTest = False
            iLastPrediction = True
            Exit Function
        End If
        iCurveHours = iHours + 24
        iDecimalTime = iCurveHours * 100 + iDecimalMinutes
        iOldHour = iHours
    Else
        'so long as the hour is greater than the previous hour....
        If iHours >= iOldHour Then
        'quit if it is the same hour, this is not possible
            iOldHour = iHours
            Calc1Time = "9999"
            iCurveHours = iHours
            iDecimalTime = iCurveHours * 100 + iDecimalMinutes
            If iLastPrediction Then
                iDecimalTime = iDecimalTime + 4800
            End If
            Exit Function
        ElseIf iHours < iOldHour Or iFirstDay Then
        'When the next hour is a number smaller than the previous hour for
        'the first time, the predictuion falls witin day two, SwitchHourTest is True
            iCurveHours = iHours + 24
            iDecimalTime = iCurveHours * 100 + iDecimalMinutes
            iSwitchHourTest = True
            iOldHour = iHours
        End If
    End If
    
    'assemble time as string "##:##" and return result to the calling proceedure
    If iHours >= 0 And iHours < 24 Then
        If iMin < 10 Then
            sMinutes = "0" & LTrim$(Str$(iMin))
        Else
            sMinutes = LTrim$(Str$(iMin))
        End If
        sPredictTime24hr$ = Str$(iHours) & sMinutes
        sTempTime$ = sPredictTime24hr$
        Calc1Time = sTempTime$
    Else
        Calc1Time = "9999"
    End If
Exit Function

ExitLine12:
  Error Err
End Function
Sub RunCurve1Day()

    ReDim BridgeArray(1 To 5000) As DecimalTideType
    Dim sngValueOldY As Single, sngValueNewY As Single, sngChangeY As Single
    Dim iValueOldX As Integer, iValueNewX As Integer, iChangeX As Integer
    Dim sTideHeight As String * 5
    Dim sTideDecTime As String
    Dim i As Integer
    Dim j As Integer
    Dim iLastDay As Integer
    Dim iColumnMarker As Integer
    Dim sTemp As String
    Dim iTemp2 As Integer
    Dim sTideHour As String
    Dim sTideDecMin As String
    Dim sTideMin As String
    Dim sOldTime As String
    
    'initialize variables
    iBridgeCounter = 0
    iXValue3 = 0: sngYValue3 = 0
    For i = 1 To 5000
        BridgeArray(i).iDecTime = 9999
    Next i

    On Local Error GoTo ExitLine1
    ''Err.Raise 6 ' Generate an "Overflow" error.
    
    'Step through three days of predictions in HighLowArray
    For i = 1 To 18
        If HighLowArray(i).iDecTime >= 2400 And HighLowArray(i - 1).iDecTime <= 4800 Then
        
             If iLastDay And HighLowArray(i).iDecTime = 4800 Then Exit For
        'Calculate the difference in the X and Y coordinates corresponding to the
        'difference between each subsequent tide. ".iDecTime" is hours and minutes
        'expressed as a decimal equivalent.
             iValueOldX = HighLowArray(i - 1).iDecTime
             iValueNewX = HighLowArray(i).iDecTime
             iChangeX = iValueNewX - iValueOldX
            
             sngValueOldY = HighLowArray(i - 1).sngHeight
             sngValueNewY = HighLowArray(i).sngHeight
             sngChangeY = sngValueNewY - sngValueOldY
            
             iXValue3 = iValueOldX:   sngYValue3 = sngValueOldY:
             'Calculate the cosine curve connecting the previous point to the
             'next point on the graph. Store the incremental X and Y values
             'in BridgeArray().
             If HighLowArray(i).iDecTime <> 9999 And HighLowArray(i).sngHeight <> "9999" Then
                 Calc1Exchange iChangeX, sngChangeY
             End If
        End If
    Next i
        
    For i = 1 To 5000
        'When out of data, print the last result, add to the checksum
        'and quit the loop.
        If BridgeArray(i).iDecTime = 9999 Then
            If sTemp <> "" Then
                sTemp = sPredictionDate$ & " " & sPredictionHour$ & " " & sTemp & Chr$(13) & Chr$(10)
                Call CalcCheckSum(sTemp)
                Print #2, sTemp;
                ''Debug.Print sTemp;
                sTemp = ""
            End If
            Exit For
        End If
        

        ''If BridgeArray(i).iDecTime = 0 Then
        ''    x = dummy
        ''End If
       
        'When the time on the cosine curve equals the ten minute interval,
        'convert the time back to hours and minutes.
        sTideDecTime = Format(Str$(BridgeArray(i).iDecTime), "0000")
        Select Case Right(sTideDecTime, 2)
            Case "00", "17", "34", "50", "67", "84"
            ''Debug.Print i,: Debug.Print BridgeArray(i).iDecTime,: Debug.Print BridgeArray(i).sngHeight
            
            sTideHour$ = Mid$(sTideDecTime, 1, 2)
            sTideHour$ = Format(sTideHour$, "00")
            sPredictionHour$ = sTideHour$ & ":00:00"
            
            sTideDecMin$ = Mid$(sTideDecTime, 3, 2)
            sTideMin$ = Str$((Val(sTideDecMin$) / 100) * 60)
            sTideMin$ = Format(sTideMin$, "00")
            ''If BridgeArray(i).sngHeight < 0 Then
                ''x = dummy
            ''End If
            RSet sTideHeight = Format(Str$(BridgeArray(i).sngHeight), "#00.0")
            ''Debug.Print Len(sTideHeight)
            ''Debug.Print "*" & sTideHeight & "*"
            ''Debug.Print sTideHour$ & sTideMin$, BridgeArray(i).sngHeight
            'Check that rounding does not produce redundant values
            If sTideHour$ & sTideMin$ <> sOldTime$ Then
                For j = 1 To 6
                    'increment the implied time
                    iColumnMarker = iColumnMarker + 1
                    If iColumnMarker > 6 Then
                        iColumnMarker = 1
                    End If

                'If the predicted time matches the implied time, print the
                'last result, add to the checksum and quit the loop.
                    If sColumnNameArray(iColumnMarker) = sTideMin$ Then
                        sTemp = sTemp & " " & sTideHeight
                        sOldTime$ = sTideHour$ & sTideMin$
                        If iColumnMarker = 6 Then
                            sTemp = sPredictionDate$ & " " & sPredictionHour$ & sTemp & Chr$(13) & Chr$(10)
                            Call CalcCheckSum(sTemp)
                            Print #2, sTemp;
                            ''Debug.Print sTemp;
                            ''Debug.Print Len(sTemp)
                            sTemp = ""
                        End If
                        Exit For
                    Else
                    'Warn if there are missing values. This will trigger if the
                    'last prediction for previous year is not provided by the user.
                        iTemp2 = MsgBox("No data for " & sPredictionDate$ & " " & sColumnNameArray(iColumnMarker) & ".   Coninue?", 1)
                        If iTemp2 = 2 Then End
                        sTemp = sTemp & "      "
                    End If
                Next j
            End If
        End Select

    Next i

Exit Sub

ExitLine1:
  Error Err
End Sub
Sub Calc1Exchange(iTimeChangeX As Integer, sAmplitudeChangeY As Single)
    
    Dim sngXValue1 As Single
    Dim sngXValue2 As Single
    Dim sngYValue1 As Single
    Dim sngYValue2 As Single
    Dim sngTimeIncrement As Single
    Dim sngIncrementAmount As Single
    
    'time increment constant
    sngIncrementAmount = 0.003
    
    On Local Error GoTo ExitLine11

    'day two begins at 2400 and ends at 4800 on the decimal time scale
    If iXValue3 >= 4800 Or iXValue3 <= 1600 Then
        Exit Sub
    End If
    
    'the beginning point of the curve is the endpoint of the prior curve
    sngXValue1 = iXValue3: sngYValue1 = sngYValue3
    
    'step through the cosine curve by increments totaling to pi
    For sngTimeIncrement = 0 To 3.141593 Step sngIncrementAmount
    
        'calculate height at next increment
        sngYValue2 = (-sAmplitudeChangeY * Cos(sngTimeIncrement) + sAmplitudeChangeY) / 2 + sngYValue3 '-cos(x) is opposite
        
        'calculate time at next increment
        sngXValue2 = iTimeChangeX * sngTimeIncrement / 3.141593 + iXValue3
        
        'store in array if time value is within day two and subtract out day one (2400)
        If sngXValue1 >= 2400 Then
            iBridgeCounter = iBridgeCounter + 1
            BridgeArray(iBridgeCounter).iDecTime = Int(sngXValue1) - 2400
            BridgeArray(iBridgeCounter).sngHeight = sngYValue1
            'Debug.Print iBridgeCounter,: Debug.Print BridgeArray(iBridgeCounter).iDecTime,: Debug.Print BridgeArray(iBridgeCounter).sngHeight 'new
        End If
        If sngXValue2 >= 4800 Then
            Exit Sub
        End If
        
        'new beginning value becomes old ending value
        sngXValue1 = sngXValue2
        sngYValue1 = sngYValue2
    
    Next
    
    'record ending values in global variables
    iXValue3 = iXValue3 + iTimeChangeX
    sngYValue3 = sngYValue3 + sAmplitudeChangeY
    
Exit Sub

ExitLine11:
  Error Err

End Sub



Public Sub CalcCheckSum(sCheckString As String)

    Dim sTemp As String
    Dim i As Integer
    On Error Resume Next
    For i = 1 To 55
        'Debug.Print Len(sCheckString)
        sTemp = Mid$(sCheckString, i)
        lCheckSum = lCheckSum + Asc(sTemp)
        'Debug.Print i, Asc(sTemp), lCheckSum
    Next i
        
     Print #3, Left(sCheckString, 17), lCheckSum
    'Debug.Print lCheckSum

End Sub
