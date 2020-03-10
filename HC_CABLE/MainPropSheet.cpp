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

// MainPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "HC_CABLE.h"
#include "MainPropSheet.h"
#include "ErrorCodes.h"
#include "ProgressDlg.h"


// CMainPropSheet

IMPLEMENT_DYNAMIC(CMainPropSheet, CPropertySheet)

CMainPropSheet::CMainPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Init();
}

CMainPropSheet::CMainPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Init();
}

CMainPropSheet::~CMainPropSheet()
{
}


BEGIN_MESSAGE_MAP(CMainPropSheet, CPropertySheet)
END_MESSAGE_MAP()


// CMainPropSheet message handlers

void CMainPropSheet::Init(void)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	AddPage(&m_CableViewPage);
	AddPage(&m_OptionsPage);
	AddPage(&m_AboutPage);

	m_bCablesLoaded = FALSE;
	m_bTidesLoaded = FALSE;
}

#if defined _WIN32_WCE
bool CMainPropSheet::IsDST()
{
	return m_OptionsPage.IsDST();
}

void CMainPropSheet::IsDST(bool bIsDST)
{
	m_OptionsPage.IsDST(bIsDST);
	m_CableViewPage.IsDST(bIsDST);
}
#endif

BOOL CMainPropSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

#if !defined _WIN32_WCE
	// change "OK" to "Exit"
	CWnd* pOK = GetDlgItem(IDOK);
	pOK->SetWindowText(_T("Exit"));
	
	// Hide the Cancel button
	CWnd* pCancel = GetDlgItem(IDCANCEL);
	pCancel->ShowWindow(SW_HIDE);

	// move the Ok button to the location of the Cancel button
	CRect rCancel;
	pCancel->GetWindowRect(&rCancel);
	ScreenToClient(rCancel);
	pOK->MoveWindow(rCancel);
#endif
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON,0,0,LR_DEFAULTCOLOR);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

   m_CableFile = m_OptionsPage.GetCableFile();
   m_TideFile = m_OptionsPage.GetTideFile();

	return bResult;
}

BOOL CMainPropSheet::LoadWaterElevations(CWaterElevations* pWaterElevations)
{
	if ( m_TideFile == m_OptionsPage.GetTideFile() && m_bTidesLoaded )
		return TRUE;

	CProgressDlg progress;
	CLoadWaterElevationCallback*  pLoadTideCallback = &progress.m_TideCallback;

   m_TideFile = m_OptionsPage.GetTideFile();

	progress.Create(CProgressDlg::IDD);

	// station id for Bangor
//	int result = pTides->LoadFromFile(9445133,m_TideFile,pLoadTideCallback);
	int result = pWaterElevations->LoadFromFile(m_TideFile,pLoadTideCallback);

	if ( result == FILE_NOT_FOUND )
	{
		CString strMessage;
		strMessage.Format(_T("Error loading tidal data. File %s could not be found"), m_TideFile);
		AfxMessageBox(strMessage,MB_OK | MB_ICONEXCLAMATION );
		m_bTidesLoaded = FALSE;
		return FALSE;
	}
	else if ( result == INVALID_FORMAT )
	{
		AfxMessageBox(_T("Tidal data is not in a valid format"),MB_OK | MB_ICONEXCLAMATION );
		m_bTidesLoaded = FALSE;
		return FALSE;
	}
	else if ( result == INVALID_STATION )
	{
		AfxMessageBox(_T("Tidal data is not for the Port Gamble station"),MB_OK | MB_ICONEXCLAMATION );
		m_bTidesLoaded = FALSE;
		return FALSE;
	}

	CCableApp* pApp = (CCableApp*)AfxGetApp();
	bool bIsDST = pApp->IsDST();
	pWaterElevations->IsDST(bIsDST);

	progress.DestroyWindow();
	m_bTidesLoaded = TRUE;
	return TRUE;
}

BOOL CMainPropSheet::LoadCableData(CCables* pCables)
{
	if ( m_CableFile == m_OptionsPage.GetCableFile() && m_bCablesLoaded )
		return TRUE;

   m_CableFile = m_OptionsPage.GetCableFile();
	int result = pCables->LoadFromFile(m_CableFile);

	if ( result == FILE_NOT_FOUND )
	{
		CString strMessage;
		strMessage.Format(_T("Error loading cable data. File %s could not be found"), m_CableFile);
		AfxMessageBox(strMessage,MB_OK | MB_ICONEXCLAMATION );
		m_bCablesLoaded = FALSE;
		return FALSE;
	}
	else if ( result == INVALID_FORMAT )
	{
		AfxMessageBox(_T("Cable data is not in a valid format"),MB_OK | MB_ICONEXCLAMATION );
		m_bCablesLoaded = FALSE;
		return FALSE;
	}

	m_bCablesLoaded = TRUE;
	return TRUE;
}
