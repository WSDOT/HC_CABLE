///////////////////////////////////////////////////////////////////////
// HC Cable
// Copyright © 2008-2020  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// CableViewPage.cpp : implementation file
//

#include "stdafx.h"
#include "HC_CABLE.h"
#include "MainPropSheet.h"
#include "CableViewPage.h"
#include <fstream>
#include <math.h>

// calculation modes
#define REALTIME_MODE    100
#define ELEVATION_MODE   200
#define DATETIME_MODE    300

// CCableViewPage dialog

IMPLEMENT_DYNAMIC(CCableViewPage, CPropertyPage)

CCableViewPage::CCableViewPage()
	: CPropertyPage(CCableViewPage::IDD)
	, m_TideElevation(0.0)
	, m_bIsDirty(false)
{
	m_ClockEventID    = 1000;
	m_RealTimeEventID = 2000;

	m_strCurrentTimeFormat = _T("%m/%d/%Y %I:%M:%S %p");

	m_strCalculationTimeFormat = _T("%m/%d/%Y %r");
	m_TimeInterval = 10; // minutes;

	m_TideElevation = m_Solver.GetNormalWaterElevation();

	m_pCableRecordDlg = NULL;

   m_UserDate = CTime::GetCurrentTime();
}

CCableViewPage::~CCableViewPage()
{
}

void CCableViewPage::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_CABLE_LIST, m_CableList);
   DDX_Control(pDX, IDC_DATEPICKER, m_ctrlDate);

   DDX_Text(pDX, IDC_TIDE, m_TideElevation);

   DDX_DateTimeCtrl(pDX, IDC_DATEPICKER, m_UserDate);
}

BOOL CCableViewPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_CableList.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL, NULL);
	m_CableList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// set up the list control
	m_CableList.InsertColumn(0,_T("Gauge(psi)"),LVCFMT_LEFT);
	m_CableList.InsertColumn(1,_T("Cable"),LVCFMT_CENTER);
	m_CableList.InsertColumn(2,_T("Gauge(psi)"),LVCFMT_RIGHT);

	m_Font.CreatePointFont(80,_T("Courier New"));
	m_CableList.SetFont(&m_Font);

	// start the timer that shows the running clock
	SetTimer(m_ClockEventID,1000,NULL);

   m_ctrlDate.SetFormat(_T("MM'/'dd'/'yyyy' 'hh':'mm tt"));

   return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CCableViewPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_MODE, &CCableViewPage::OnModeChanged)
	ON_BN_CLICKED(IDC_UPDATE, &CCableViewPage::OnUpdateForTide)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_CABLE_LIST, &CCableViewPage::OnCableDblClicked)
	ON_BN_CLICKED(IDC_SAVE, &CCableViewPage::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CCableViewPage::OnBnClickedLoad)
END_MESSAGE_MAP()


// CCableViewPage message handlers

void CCableViewPage::OnModeChanged()
{
	// combo box changed... determine what mode we are in
	// and enable/disable, show/hide individual tide controls
	// accordingly
	CComboBox* pModes = (CComboBox*)GetDlgItem(IDC_MODE);
	int idx = pModes->GetCurSel(); 
	int mode = (int)pModes->GetItemData(idx);

	// get the controls so we can manipulate them
	CWnd* pTideLabel = GetDlgItem(IDC_TIDE_LABEL);
	CWnd* pTide      = GetDlgItem(IDC_TIDE);
	CWnd* pTideUnit  = GetDlgItem(IDC_TIDE_UNIT);
	CWnd* pUpdate    = GetDlgItem(IDC_UPDATE);

	// show/hide the controls
	pTideLabel->ShowWindow(mode == ELEVATION_MODE ? SW_SHOW : SW_HIDE);
	pTide->ShowWindow(mode == ELEVATION_MODE ? SW_SHOW : SW_HIDE);
	pTideUnit->ShowWindow(mode == ELEVATION_MODE ? SW_SHOW : SW_HIDE);
	pUpdate->ShowWindow(mode == ELEVATION_MODE || mode == DATETIME_MODE ? SW_SHOW : SW_HIDE);
   m_ctrlDate.ShowWindow(mode == DATETIME_MODE ? SW_SHOW : SW_HIDE);

	// enable/disable the controls
	pTideLabel->EnableWindow(mode == ELEVATION_MODE);
	pTide->EnableWindow(mode == ELEVATION_MODE);
	pTideUnit->EnableWindow(mode == ELEVATION_MODE);
	pUpdate->EnableWindow(mode == ELEVATION_MODE || mode == DATETIME_MODE);
   m_ctrlDate.EnableWindow(mode == DATETIME_MODE);

	// adjust the list control to maximize it's height
	CRect rTide;
	pTide->GetWindowRect(&rTide);
	ScreenToClient(rTide);

	CRect rList;
	m_CableList.GetWindowRect(&rList);
	ScreenToClient(rList);

   CRect rUpdate;
   pUpdate->GetWindowRect(&rUpdate);
   ScreenToClient(rUpdate);

	if (mode == ELEVATION_MODE || mode == DATETIME_MODE)
	{
		rList.top = rUpdate.bottom/*rTide.bottom*/ + 5;
		m_CableList.MoveWindow(rList);
	}
	else
	{
		rList.top = rTide.top;
		m_CableList.MoveWindow(rList);
	}

	// start/stop the real time calculations
	if (mode == ELEVATION_MODE || mode == DATETIME_MODE)
	{
		EndRealTimeCalculations();
      OnUpdateForTide();
	}
	else
	{
		StartRealTimeCalculations();
	}
}

void CCableViewPage::StartRealTimeCalculations()
{
	OnTimer(m_RealTimeEventID); 
}

void CCableViewPage::EndRealTimeCalculations()
{
	// not doing real time calculations any more... kill the timer
	KillTimer(m_RealTimeEventID);
}

void CCableViewPage::OnUpdateForTide()
{
	// User press the Update button
   CComboBox* pModes = (CComboBox*)GetDlgItem(IDC_MODE);
   int idx = pModes->GetCurSel();
   int mode = (int)pModes->GetItemData(idx);

	UpdateData(TRUE); // get the data from the UI

   CTime time;
   if (mode == ELEVATION_MODE)
   {
      time = CTime::GetCurrentTime();
   }
   else
   {
      time = m_UserDate;
   }

	CCableApp* pApp = (CCableApp*)AfxGetApp();
	if ( pApp->IsDST() )
	{
		// if it is DST, subtract 1 hour because it will be
		// added back in UpdateCableTensions
		time -= CTimeSpan(0,1,0,0);
	}

   if (mode == ELEVATION_MODE)
   {
      CWaterElevation waterElevation(time, m_TideElevation);
      UpdateCableTensions(waterElevation);
      UpdateCableList();
   }
   else
   {
      UpdateForTime(time);
   }
}

void CCableViewPage::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == m_ClockEventID )
	{
		// update the clock
		CTime now = CTime::GetCurrentTime();
      CString strCurrentTime;
      strCurrentTime.Format(_T("Current time: %s"),now.Format(m_strCurrentTimeFormat));
		GetDlgItem(IDC_CURRENT_TIME)->SetWindowText(strCurrentTime);
	}
	else if ( nIDEvent == m_RealTimeEventID )
	{
		// time interval event for computing table tensions
      UpdateForTime(CTime::GetCurrentTime());

		KillTimer(m_RealTimeEventID);
		CTime now = CTime::GetCurrentTime(); // get the time again, because it may have changed
		int min = now.GetMinute();
		int rmin = m_TimeInterval - (min % m_TimeInterval) - 1; // minutes to go until next interval
		int rsec = 60 - now.GetSecond(); // seconds to go until next interval
		int time_to_next = (rmin*60 + rsec + 5)*1000; // + 5 is a 5 second delay so the event occurs 
                                                	 // just after the theoretical interval. 
		                                              // This will help account for inaccuracies in the Windows clock
    	SetTimer(m_RealTimeEventID,time_to_next,NULL);

		if ( m_pCableRecordDlg )
			m_pCableRecordDlg->UpdateTides();
 	}
}

void CCableViewPage::UpdateForTime(CTime now)
{
	CWaterElevation waterElevation;
	if ( !m_WaterElevations.GetWaterElevation(now,&waterElevation) )
	{
		CString strMessage;
		strMessage.Format(_T("Error finding tidal data for %s"),now.Format(m_strCurrentTimeFormat));
		AfxMessageBox(strMessage);

		m_CableList.DeleteAllItems();
	    GetDlgItem(IDC_CALCULATION_TIME)->SetWindowText(_T("Error"));
	}
	else
	{
		UpdateCableTensions(waterElevation);
		UpdateCableList();
	}
}

BOOL CCableViewPage::LoadWaterElevations()
{
	CMainPropSheet* pParent = (CMainPropSheet*)GetParent();
	return pParent->LoadWaterElevations(&m_WaterElevations);
}

BOOL CCableViewPage::LoadCableData()
{
	CMainPropSheet* pParent = (CMainPropSheet*)GetParent();
	BOOL bResult = pParent->LoadCableData(&m_Cables);

	// initialize the recorded pressures
	if ( bResult )
	{
		long nCables = m_Cables.GetCableCount();
		m_CableRecords.Init(nCables);
		m_CableTensions.Init(nCables);
	}

	return bResult;
}

void CCableViewPage::UpdateCableTensions(const CWaterElevation& waterElevation)
{
	// do the work of computing cable tensions and updating
	// the cable list
	CWaitCursor wait;

	// capture the tide used for the computations
	m_CalculationWaterElevation = waterElevation;

	CTime calculationTime = m_CalculationWaterElevation.GetTime();
	float waterLevel = m_CalculationWaterElevation.GetElevation();


	// do the calculations for each cable pair
	long nCables = m_Cables.GetCableCount();
	for ( long i = 0; i < nCables; i++ )
	{
		// get the cables
		CCable northCable, southCable;
		m_Cables.GetCablePair(i,&northCable,&southCable);

		// do the calculations
		CCableTensionSolution northSolution = m_Solver.Solve(m_CalculationWaterElevation,northCable);
		CCableTensionSolution southSolution = m_Solver.Solve(m_CalculationWaterElevation,southCable);

		northSolution.SetCalculationTime(calculationTime);
		northSolution.SetWaterElevation(waterLevel);

		southSolution.SetCalculationTime(calculationTime);
		southSolution.SetWaterElevation(waterLevel);

		// update the calculation and observation record
		CCableRecord northCableRecord, southCableRecord;
		m_CableRecords.GetCableRecords(i,&northCableRecord,&southCableRecord);

		// only update records if values have not yet been recorded
		if (northCableRecord.GetActualPressure() < 0 )
		{
			CTime northMeasurementTime = calculationTime;
			float northMeasuredPressure = northCableRecord.GetActualPressure();
			CString northRemark = northCableRecord.GetRemark();

			northCableRecord.Init(calculationTime,waterLevel,northSolution.GetGaugePressure(),northMeasuredPressure,northMeasurementTime,northRemark);
		}

		if ( southCableRecord.GetActualPressure() < 0 )
		{
			CTime southMeasurementTime = calculationTime;
			float southMeasuredPressure = southCableRecord.GetActualPressure();
			CString southRemark = southCableRecord.GetRemark();

			southCableRecord.Init(calculationTime,waterLevel,southSolution.GetGaugePressure(),southMeasuredPressure,southMeasurementTime,southRemark);
		}

		m_CableRecords.SetCableRecords(i,northCableRecord,southCableRecord);
		m_CableTensions.SetCableTensionSolutions(i,northSolution,southSolution);
	}
}

void CCableViewPage::UpdateCableList()
{
	// store the index of the current selection
	int selCount = m_CableList.GetSelectedCount();
	ATLASSERT(selCount == 0 || selCount == 1);
	int iSelItem;
	iSelItem = m_CableList.GetNextItem(0,LVNI_SELECTED);

	// clear out the list
	m_CableList.DeleteAllItems();

	// list the values for each cable pair
	long nCables = m_Cables.GetCableCount();
	for ( long i = 0; i < nCables; i++ )
	{
		CCable northCable, southCable;
		m_Cables.GetCablePair(i,&northCable,&southCable);

		// format the output
		CString strNorthGauge, strSouthGauge;

		CCableTensionSolution northSolution, southSolution;
		m_CableTensions.GetCableTensionSolutions(i,&northSolution,&southSolution);

		CCableRecord northCableRecord, southCableRecord;
		m_CableRecords.GetCableRecords(i,&northCableRecord,&southCableRecord);

		float northMeasuredPressure = northCableRecord.GetActualPressure();
		float southMeasuredPressure = southCableRecord.GetActualPressure();

		strNorthGauge.Format(_T("%.0f%4s"), northSolution.GetGaugePressure(), northMeasuredPressure < 0 ? _T("") : _T(" (*)"));

		strSouthGauge.Format(_T("%4s%.0f"), southMeasuredPressure < 0 ? _T("") : _T("(*) "),southSolution.GetGaugePressure());

		CString strCableID;
		strCableID.Format(_T("%-2s %-2s %-2s"),
			              northCable.GetOrientationString(),
			              northCable.GetID(),
						  southCable.GetOrientationString());

		// fill up the list control UI
		m_CableList.InsertItem(i,strNorthGauge);
		m_CableList.SetItem(i,1,LVIF_TEXT,strCableID,0,0,0,0);
		m_CableList.SetItem(i,2,LVIF_TEXT,strSouthGauge,0,0,0,0);
	}

	// update the column widths
	UpdateColumnWidths();

	m_CableList.SetItemState(iSelItem,LVNI_SELECTED,LVNI_SELECTED);
    m_CableList.EnsureVisible(iSelItem,FALSE);

	// update the calculation time

	CTime calc_time = m_CalculationWaterElevation.GetTime();
	CCableApp* pApp = (CCableApp*)AfxGetApp();
	if ( pApp->IsDST() )
   {
      calc_time += CTimeSpan(0,1,0,0); // if PDT then add one hour for PST
   }

	float waterLevel = m_CalculationWaterElevation.GetElevation();

   CComboBox* pModes = (CComboBox*)GetDlgItem(IDC_MODE);
   int idx = pModes->GetCurSel();
   int mode = (int)pModes->GetItemData(idx);	
   CString strCalcTime;
   if (mode == ELEVATION_MODE)
   {
      strCalcTime.Format(_T("Prediction time: __/__/____ __:__:__ __ Tide: %.1f ft"), waterLevel);
   }
   else
   {
      strCalcTime.Format(_T("Prediction time: %s Tide: %.1f ft"), calc_time.Format(m_strCalculationTimeFormat), waterLevel);
   }
   GetDlgItem(IDC_CALCULATION_TIME)->SetWindowText(strCalcTime);
}

void CCableViewPage::UpdateColumnWidths(bool bForceUpdate)
{
	if ( m_CableList.GetSafeHwnd() == NULL )
		return;

   RECT rect;
   m_CableList.GetClientRect(&rect);
   int width = rect.right / m_CableList.GetHeaderCtrl()->GetItemCount();
	m_CableList.SetColumnWidth(0,width);
	m_CableList.SetColumnWidth(1,width);
	m_CableList.SetColumnWidth(2,width);
}

HBRUSH CCableViewPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// make the text color of the CALCULATION_TIME label red
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( pWnd->GetSafeHwnd() == GetDlgItem(IDC_CALCULATION_TIME)->GetSafeHwnd()
		 && nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	return hbr;
}

void CCableViewPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	if ( m_CableList.GetSafeHwnd() == NULL )
		return; // controls aren't ready yet

	// adjust the controls to fit
	CWnd* pParent = GetParent();
	CRect rDlg;
	pParent->GetWindowRect(&rDlg);

	CRect rMe;
	GetWindowRect(&rMe);

	CRect rPage;
#if defined _WIN32_WCE
	rPage = rDlg;
#else
	rMe.right = rDlg.right - 20;
	rPage = rMe;
#endif

	CRect rList;
	m_CableList.GetWindowRect(&rList);
	rList.right = rPage.right - 10;

	CRect rSave;
	CWnd* pSave = GetDlgItem(IDC_SAVE);
	pSave->GetWindowRect(&rSave);
	int w = rSave.Width();
	int h = rSave.Height();
	rSave.right = rList.right;
	rSave.left = rSave.right - w;
	rSave.top = rPage.top + 5;
	rSave.bottom = rSave.top + h;

	CRect rLoad;
	CWnd* pLoad = GetDlgItem(IDC_LOAD);
	pLoad->GetWindowRect(&rLoad);
	rLoad.right = rSave.left - 5;
	rLoad.left = rLoad.right - w;
	rLoad.top = rSave.top;
	rLoad.bottom = rLoad.top + h;

	CRect rModes;
	CWnd* pModes = GetDlgItem(IDC_MODE);
	pModes->GetWindowRect(&rModes);
	h = rModes.Height();
	rModes.right = rLoad.left - 5;
	rModes.top = rPage.top + 5;
	rModes.bottom = rModes.top + h;

	ScreenToClient(rList);
	m_CableList.MoveWindow(rList);

	ScreenToClient(rSave);
	pSave->MoveWindow(rSave);

	ScreenToClient(rLoad);
	pLoad->MoveWindow(rLoad);

	ScreenToClient(rModes);
	pModes->MoveWindow(rModes);

	UpdateColumnWidths(true);
}

BOOL CCableViewPage::OnSetActive()
{
	// load the cable and tide data
	if ( !LoadCableData() )
		return FALSE; // can't do anything without the cable data

	LoadWaterElevations();

	CComboBox* pModes = (CComboBox*)GetDlgItem(IDC_MODE);
	int curSel = pModes->GetCurSel();
	int count = pModes->GetCount();

	pModes->ResetContent();

	int index;
	if ( 0 < m_WaterElevations.GetCount() )
	{
		// if there is tide data, we can run in real time mode
		CString strInterval;
		strInterval.Format(_T("Calculate at %d min. interval"),m_TimeInterval);
		index = pModes->AddString(strInterval);
		pModes->SetItemData(index,REALTIME_MODE);

      index = pModes->AddString(_T("Calculate for a date/time"));
      pModes->SetItemData(index, DATETIME_MODE);
   }

	index = pModes->AddString(_T("Calculate for a tidal elevation"));
	pModes->SetItemData(index, ELEVATION_MODE);

	if ( count == pModes->GetCount() && curSel != CB_ERR )
	{
		pModes->SetCurSel(curSel);
	}
	else
	{
		pModes->SetCurSel(0);
		OnModeChanged();
	}

	return CPropertyPage::OnSetActive();
}

void CCableViewPage::IsDST(bool bIsDST)
{
	m_WaterElevations.IsDST(bIsDST);

   if (GetSafeHwnd())
   {
      UpdateForTime(CTime::GetCurrentTime());
   }
}

void CCableViewPage::OnCableDblClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	int cableIdx = -1;
	bool bNorthCable = true;

#if defined _WIN32_WCE
	if ( pNMHDR->idFrom == IDC_CABLE_LIST )
	{
		POSITION pos = m_CableList.GetFirstSelectedItemPosition();
		cableIdx = m_CableList.GetNextSelectedItem(pos);
	}
#else
	NMITEMACTIVATE* pNMItemActivate = (NMITEMACTIVATE*)(pNMHDR);
	cableIdx = pNMItemActivate->iItem;

	bNorthCable = pNMItemActivate->iSubItem == 2 ? false : true;
#endif

	*pResult = 0;

	if ( cableIdx < 0 )
		return;

	CCableRecordDlg dlg(&m_Cables,&m_CableRecords, &m_CableTensions, cableIdx,bNorthCable ? CCable::North : CCable::South, this);
	m_pCableRecordDlg = &dlg;
	dlg.DoModal();
	m_pCableRecordDlg = NULL;
	m_bIsDirty = true;
	UpdateForTime(CTime::GetCurrentTime());
}

void CCableViewPage::OnBnClickedSave()
{
	SaveCableRecords();
}

void CCableViewPage::OnBnClickedLoad()
{
	LoadCableRecords();
	UpdateCableList();
}

bool CCableViewPage::LoadCableRecords()
{
	USES_CONVERSION;

	CString strFileName;
#if defined _WIN32_WCE
	strFileName.Format(_T("%s"),_T("\\My Documents\\*.hcc"));
#else
	strFileName.Format(_T("%s"),_T("*.hcc"));
#endif
	CFileDialog dlg(TRUE,
		            _T("hcc"),
					strFileName,
					OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, 
					_T("Cable Tension Files|*.hcc||")); 

	if ( dlg.DoModal() != IDOK )
	{
		return false;
	}

	CString strFile = dlg.GetPathName();
	std::ifstream ifile(strFile.GetBuffer());

	if ( !ifile )
		return false;

	char buffer[100];
	while (ifile)
	{
		ifile.getline(buffer,100);
		if ( strlen(buffer) == 0 )
			continue;

		CString strBuffer(buffer);
		int curPos = 0;
		CString strCable = strBuffer.Tokenize(_T(","),curPos);
		CString strTideDate = strBuffer.Tokenize(_T(","),curPos);
		CString strTideTime = strBuffer.Tokenize(_T(","),curPos);
		CString strWaterElevation = strBuffer.Tokenize(_T(","),curPos);
		CString strPredictedPressure = strBuffer.Tokenize(_T(","),curPos);
		CString strMeasuredDate = strBuffer.Tokenize(_T(","),curPos);
		CString strMeasuredTime = strBuffer.Tokenize(_T(","),curPos);
		CString strMeausredPressure = strBuffer.Tokenize(_T(","),curPos);
		CString strRemark = strBuffer.Mid(curPos);

		int year, month, day, hour, min;
		curPos = 0;
		month = atoi(OLE2A(strTideDate.Tokenize(_T("/"),curPos)));
		day   = atoi(OLE2A(strTideDate.Tokenize(_T("/"),curPos)));
		year  = atoi(OLE2A(strTideDate.Tokenize(_T("/"),curPos)));
		hour  = atoi(OLE2A(strTideTime.Left(2)));
		min   = atoi(OLE2A(strTideTime.Right(2)));

		CTime tideTime(year,month,day,hour,min,0);

		curPos = 0;
		month = atoi(OLE2A(strMeasuredDate.Tokenize(_T("/"),curPos)));
		day   = atoi(OLE2A(strMeasuredDate.Tokenize(_T("/"),curPos)));
		year  = atoi(OLE2A(strMeasuredDate.Tokenize(_T("/"),curPos)));
		hour  = atoi(OLE2A(strMeasuredTime.Left(2)));
		min   = atoi(OLE2A(strMeasuredTime.Right(2)));

		CTime measuredTime(year,month,day,hour,min,0);

		float waterElevation    = (float)atof(OLE2A(strWaterElevation));
		float predictedPressure = (float)atof(OLE2A(strPredictedPressure));
		float measuredPressure  = (float)atof(OLE2A(strMeausredPressure));

		long idx;
		CCable::Orientation orientation;
		m_Cables.FindCable(strCable,&idx,&orientation);

		CCableRecord record;
		m_CableRecords.GetCableRecord(idx,orientation,&record);
		record.Init(tideTime,waterElevation,predictedPressure,measuredPressure,measuredTime,strRemark);
		m_CableRecords.SetCableRecord(idx,orientation,record);
	}


	return true;
}

bool CCableViewPage::SaveCableRecords()
{
	USES_CONVERSION;

	CString strFileName;
	CTime now = CTime::GetCurrentTime();
#if defined _WIN32_WCE
	strFileName.Format(_T("\\My Documents\\CableTensions_%s.hcc"),now.Format(_T("%m%d%Y_%H%M%S")));
#else
	strFileName.Format(_T("CableTensions_%s.hcc"),now.Format(_T("%m%d%Y_%H%M%S")));
#endif
	CFileDialog dlg(FALSE,
		            _T("hcc"),
					strFileName,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					_T("Cable Tension Files|*.hcc||")); 

	if ( dlg.DoModal() != IDOK )
	{
		return false;
	}

	CString strFile = dlg.GetPathName();
	std::ofstream ofile(strFile.GetBuffer());

	if ( !ofile )
		return false;

	long nCables = m_Cables.GetCableCount();
	for ( long i = 0; i < nCables; i++ )
	{
		CCable northCable, southCable;
		m_Cables.GetCablePair(i,&northCable,&southCable);

		CCableRecord northCableRecord, southCableRecord;
		m_CableRecords.GetCableRecords(i,&northCableRecord,&southCableRecord);

		CString strNorthTime = northCableRecord.GetTime().Format(_T("%m/%d/%Y,%H%M"));
		CString strSouthTime = southCableRecord.GetTime().Format(_T("%m/%d/%Y,%H%M"));

		CString strNorthMeasuredTime = northCableRecord.GetMeasurementTime().Format(_T("%m/%d/%Y,%H%M"));
		CString strSouthMeasuredTime = southCableRecord.GetMeasurementTime().Format(_T("%m/%d/%Y,%H%M"));

		CString strNorth,strSouth;
		strNorth.Format(_T("%s,%s,%.1f,%.0f,%s,%.0f,%s\n"),northCable.GetName(),strNorthTime,northCableRecord.GetTideElevation(),northCableRecord.GetPredictedPressure(),strNorthMeasuredTime,northCableRecord.GetActualPressure(),northCableRecord.GetRemark());
		strSouth.Format(_T("%s,%s,%.1f,%.0f,%s,%.0f,%s\n"),southCable.GetName(),strSouthTime,southCableRecord.GetTideElevation(),southCableRecord.GetPredictedPressure(),strSouthMeasuredTime,southCableRecord.GetActualPressure(),southCableRecord.GetRemark());
		
		ofile << OLE2A(strNorth);
		ofile << OLE2A(strSouth);
	}

	m_bIsDirty = false;

	return true;
}

BOOL CCableViewPage::OnKillActive()
{
	if ( m_bIsDirty )
	{
		int result = AfxMessageBox(_T("Save cable tension data?"),MB_YESNOCANCEL);
		if ( result == IDCANCEL )
		{
			return FALSE;
		}
		else if ( result == IDYES )
		{
			if ( !SaveCableRecords() )
				return FALSE;
		}
	}

	return CPropertyPage::OnKillActive();
}
