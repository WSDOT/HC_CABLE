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

// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HC_CABLE.h"
#include "ProgressDlg.h"

void CLoadWaterElevationCallbackImpl::SetDialog(CProgressDlg* pDlg)
{
	m_pDlg = pDlg;
	m_CurrentTime = 0; // before Jan
}

void CLoadWaterElevationCallbackImpl::OnNewWaterElevation(const CWaterElevation& waterElevation)
{
	CTime time = waterElevation.GetTime();
#if defined _WIN32_WCE
	if ( time.GetDay() != m_CurrentTime )
	{
			CString strMessage;
			strMessage.Format(_T("Loading water level data for %s"),time.Format(_T("%m/%d/%y")));
			m_pDlg->m_Message.SetWindowText(strMessage);
			m_pDlg->m_Message.UpdateWindow();
			m_CurrentTime = time.GetDay();
	}

	tm local_time;
	time.GetLocalTm(&local_time);
	m_pDlg->m_Progress.SetPos(local_time.tm_mday);
#else
	if ( time.GetMonth() != m_CurrentTime )
	{
		// updating this message slows the loading process way down... just
		// update once a month
	    CString strMessage;
		strMessage.Format(_T("Loading water level data for %s"),time.Format(_T("%B %Y")));
		m_pDlg->m_Message.SetWindowText(strMessage);
		m_CurrentTime = time.GetMonth();
	}

	tm local_time;
	time.GetLocalTm(&local_time);
	m_pDlg->m_Progress.SetPos(local_time.tm_yday);
#endif
}

// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
	
{
	m_TideCallback.SetDialog(this);
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
#if defined _WIN32_WCE
	m_Progress.SetRange(0,30);
#else
	m_Progress.SetRange(0,365);
#endif

	return TRUE;
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
END_MESSAGE_MAP()


// CProgressDlg message handlers
