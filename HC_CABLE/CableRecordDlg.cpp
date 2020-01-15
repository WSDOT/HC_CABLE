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

// CableRecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h>
#include "afxpriv.h"
#include "HC_CABLE.h"
#include "CableRecordDlg.h"
#include <math.h>

void DDX_Pressure(CDataExchange* pDX, int nIDC, float& value)
{
	pDX->PrepareEditCtrl(nIDC);
	HWND hWndCtrl;
	pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);
	
	const int TEXT_BUFFER_SIZE = 400;
	TCHAR szBuffer[TEXT_BUFFER_SIZE];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		float d;
#ifndef _WIN32_WCE
		int nChar = _sntscanf_s(szBuffer, _countof(szBuffer), _T("%lf"), &d);
#else
		int nChar = _stscanf(szBuffer, _T("%lf"), &d);
#endif // !_WIN32_WCE

		if ( nChar <= 0 )
			d = -1; // field was blank

		value = d;
	}
	else
	{
		if ( value < 0 )
		{
			_tcscpy_s(szBuffer,_countof(_T("")),_T("")); // if neg, use blank in the edit box
		}
		else
		{
#ifndef _WIN32_WCE
		ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer) -1, _T("%.0f"), value));
#else
		_sntprintf(szBuffer, TEXT_BUFFER_SIZE, _T("%.0f"), value);
		szBuffer[TEXT_BUFFER_SIZE - 1] = 0;		
#endif // !_WIN32_WCE
		}

		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}


// CCableRecordDlg dialog

IMPLEMENT_DYNAMIC(CCableRecordDlg, CDialog)

CCableRecordDlg::CCableRecordDlg(CCables* pCables,CCableRecords* records,const CCableTensions* cableTensions,long cableIdx,CCable::Orientation cableOrientation,CWnd* pParent /*=NULL*/)
	: CDialog(CCableRecordDlg::IDD, pParent)
	, m_Remarks(_T(""))
{
	m_Time = CTime::GetCurrentTime();

	m_ClockEventID    = 1000;
	m_bLocked = FALSE;

	m_pCables = pCables;
	m_pCableRecords = records;
	m_pCableTensions = cableTensions;
	m_CableIdx = cableIdx;
	m_CableOrientation = cableOrientation;
}

CCableRecordDlg::~CCableRecordDlg()
{
}

void CCableRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	CCableApp* pApp = (CCableApp*)AfxGetApp();
	if ( !pDX->m_bSaveAndValidate && pApp->IsDST() )
	{
		// if it is DST, add 1 hour (spring ahead)
		m_Time += CTimeSpan(0,1,0,0);
	}

	if ( pDX->m_bSaveAndValidate )
	{
		DDX_Text(pDX,IDC_NTIDE,m_TideElevation);
	}
	else
	{
		CString strElevation;
		strElevation.Format(_T("%0.1f"),m_TideElevation);
		DDX_Text(pDX,IDC_NTIDE,strElevation);
	}

	DDX_Control(pDX, IDC_NORTHDATETIMEPICKER, m_TimeCtrl);
	DDX_DateTimeCtrl(pDX, IDC_NORTHDATETIMEPICKER, m_Time);

	DDX_Pressure(pDX, IDC_NPREDPRESSURE,   m_PredictedPressure);
	DDX_Pressure(pDX, IDC_NACTUALPRESSURE, m_ActualPressure);


	if ( pDX->m_bSaveAndValidate && pApp->IsDST() )
	{
		// if it is DST, subtract 1 hour (make it standard time again)
		m_Time -= CTimeSpan(0,1,0,0);
	}
	DDX_Text(pDX, IDC_REMARK, m_Remarks);
}


BEGIN_MESSAGE_MAP(CCableRecordDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV, &CCableRecordDlg::OnPrevRecord)
	ON_BN_CLICKED(IDC_NEXT, &CCableRecordDlg::OnNextRecord)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET, &CCableRecordDlg::OnBnClickedReset)
END_MESSAGE_MAP()


// CCableRecordDlg message handlers
BOOL CCableRecordDlg::OnInitDialog()
{
	OnNewCable();

	CDialog::OnInitDialog();
	m_TimeCtrl.SetFormat(_T("hh':'mm tt"));

	CClientDC dc(this);
	CWnd* pHeading = GetDlgItem(IDC_CABLE_HEADING);
	CFont* pCtrlFont = pHeading->GetFont();
	LOGFONT lf;
	pCtrlFont->GetLogFont(&lf);

	m_Font.CreatePointFont(160,lf.lfFaceName,&dc);
	pHeading->SetFont(&m_Font);

	// start the timer that shows the running clock
	SetTimer(m_ClockEventID,1000,NULL);
	OnTimer(m_ClockEventID);

	SetControlState();

	return TRUE;
}

void CCableRecordDlg::OnPrevRecord()
{
	UpdateData(TRUE);
	SaveCableRecord();

	if ( m_CableOrientation == CCable::South )
	{
		m_CableOrientation = CCable::North;
	}
	else
	{
		m_CableOrientation = CCable::South;
		m_CableIdx--;

		if ( m_CableIdx < 0 )
			m_CableIdx = m_pCables->GetCableCount()-1;
	}

	OnNewCable();
	UpdateData(FALSE);
	SetControlState();
}

void CCableRecordDlg::OnNextRecord()
{
	UpdateData(TRUE);
	SaveCableRecord();

	if ( m_CableOrientation == CCable::North )
	{
		m_CableOrientation = CCable::South;
	}
	else
	{
		m_CableOrientation = CCable::North;
		m_CableIdx++;

		if ( m_pCables->GetCableCount() <= m_CableIdx )
			m_CableIdx = 0;
	}

	OnNewCable();
	UpdateData(FALSE);
	SetControlState();
}

void CCableRecordDlg::OnNewCable()
{
	CCable cable;
	m_pCables->GetCable(m_CableIdx,m_CableOrientation,&cable);

	m_strCableName = cable.GetID();
	m_strCableOrientation = cable.GetOrientationString();

	CString strHeading;
	CWnd* pHeading = GetDlgItem(IDC_CABLE_HEADING);
	strHeading.Format(_T("Cable %s-%s"),m_strCableName,m_strCableOrientation);
	pHeading->SetWindowText(strHeading);

	CCableRecord cableRecord;
	m_pCableRecords->GetCableRecord(m_CableIdx,cable.GetOrientation(),&cableRecord);
	m_Time              = cableRecord.GetTime();
	m_TideElevation     = cableRecord.GetTideElevation();
	m_ActualPressure    = cableRecord.GetActualPressure();
	m_PredictedPressure = cableRecord.GetPredictedPressure();
	m_Remarks           = cableRecord.GetRemark();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NACTUALPRESSURE);
	pEdit->SetFocus();

    m_bLocked = ( m_ActualPressure < 0 ) ? FALSE : TRUE;
}

void CCableRecordDlg::SaveCableRecord()
{
	CCable cable;
	m_pCables->GetCable(m_CableIdx,m_CableOrientation,&cable);

	CCableRecord cableRecord;
	m_pCableRecords->GetCableRecord(m_CableIdx,cable.GetOrientation(),&cableRecord);

	cableRecord.SetTime(m_Time);
	cableRecord.SetTideElevation(m_TideElevation);
	cableRecord.SetActualPressure(m_ActualPressure);
	cableRecord.SetPredictedPressure(m_PredictedPressure);
	cableRecord.SetRemark(m_Remarks);

	CTime measuredTime = CTime::GetCurrentTime();
	CCableApp* pApp = (CCableApp*)AfxGetApp();
	if ( pApp->IsDST() )
	{
		// if it is DST, subtract 1 hour (make it standard time)
		measuredTime -= CTimeSpan(0,1,0,0);
	}

	cableRecord.SetMeasurementTime( measuredTime );
	m_pCableRecords->SetCableRecord(m_CableIdx,cable.GetOrientation(),cableRecord);
}

void CCableRecordDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( nState == WA_INACTIVE )
	{
		SaveCableRecord();
#if defined _WIN32_WCE
		SipShowIM(SIPF_OFF);
#endif
	}
}

void CCableRecordDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

#if defined _WIN32_WCE
	SipShowIM(SIPF_ON);
#endif
}

HBRUSH CCableRecordDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( pWnd->GetSafeHwnd() == GetDlgItem(IDC_NORTHDATETIMEPICKER)->GetSafeHwnd() )
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	if ( pWnd->GetSafeHwnd() == GetDlgItem(IDC_NTIDE)->GetSafeHwnd() )
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	return hbr;
}

void CCableRecordDlg::UpdateTides()
{
	if ( m_bLocked )
		return;

	// get the data from the controls
	UpdateData(TRUE);

	// save the use input
	float actualPressure =  m_ActualPressure;
	CString strRemarks    = m_Remarks;

	// re-populate the data fields for the current cable record
	OnNewCable();

	// restore the current input values
	m_ActualPressure = actualPressure;
	m_Remarks        = strRemarks;

	// put the data back in the controls
	UpdateData(FALSE);
}

void CCableRecordDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == m_ClockEventID )
	{
		// update the clock
		CTime now = CTime::GetCurrentTime();
      GetDlgItem(IDC_CURRENT_TIME)->SetWindowText(now.Format(_T("%I:%M:%S %p")));
	}
}

void CCableRecordDlg::SetControlState()
{
	GetDlgItem(IDC_NORTHDATETIMEPICKER)->EnableWindow(!m_bLocked);
	GetDlgItem(IDC_NTIDE)->EnableWindow(!m_bLocked);
	GetDlgItem(IDC_NACTUALPRESSURE)->EnableWindow(!m_bLocked);
	GetDlgItem(IDC_NPREDPRESSURE)->EnableWindow(!m_bLocked);
	GetDlgItem(IDC_REMARK)->EnableWindow(!m_bLocked);

	GetDlgItem(IDC_RESET)->ShowWindow(m_bLocked);
}

void CCableRecordDlg::OnBnClickedReset()
{
	m_bLocked = FALSE;

	// get the cable
	CCable cable;
	m_pCables->GetCable(m_CableIdx,m_CableOrientation,&cable);

	// get the cable solution
	CCableTensionSolution cableSolution;
	m_pCableTensions->GetCableTensionSolution(m_CableIdx,cable.GetOrientation(),&cableSolution);

	// get the cable record
	CCableRecord cableRecord;
	m_pCableRecords->GetCableRecord(m_CableIdx,cable.GetOrientation(),&cableRecord);

	// update its values with the current tide prediction data
	cableRecord.SetTime( cableSolution.GetCalculationTime() );
	cableRecord.SetTideElevation( cableSolution.GetWaterElevation() );
	cableRecord.SetPredictedPressure( cableSolution.GetGaugePressure() );

	m_pCableRecords->SetCableRecord(m_CableIdx,cable.GetOrientation(),cableRecord);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NACTUALPRESSURE);
	pEdit->SetWindowText(_T(""));
	SetControlState();

	OnNewCable();
	UpdateData(FALSE);
}

void CCableRecordDlg::OnOK()
{
	CDialog::OnOK();
    SaveCableRecord();
}
