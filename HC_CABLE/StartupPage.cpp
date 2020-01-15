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

// StartupPage.cpp : implementation file
//

#include "stdafx.h"
#include "HC_CABLE.h"
#include "StartupPage.h"
#include "MainPropSheet.h"

// CStartupPage dialog

IMPLEMENT_DYNAMIC(CStartupPage, CPropertyPage)

CStartupPage::CStartupPage()
	: CPropertyPage(CStartupPage::IDD)
	, m_LastCableFile(_T(""))
	, m_LastTideFile(_T(""))
	, m_TideFile(_T(""))
#if defined _WIN32_WCE
	, m_bIsDST(FALSE)
#endif
{
   // Application path - location of the CFG file
   TCHAR szBuff[_MAX_PATH];
   ::GetModuleFileName(AfxGetInstanceHandle(), szBuff, _MAX_PATH);
   CString filename(szBuff);
   CFile file;
   file.SetFilePath(filename);
   CString path = file.GetFilePath();
   path.Replace(file.GetFileName(),_T(""));

   CTime now = CTime::GetCurrentTime();

   CString strTidesFolder = CTides::GetTidesFolder();
   m_TideFile = CTides::GetTidesFile(strTidesFolder, now.GetMonth(), now.GetYear());

	m_CableFile.Format(_T("%s%s"),path,_T("HC_Cable.CFG"));

	m_LastCableFile = m_CableFile;
	m_LastTideFile = m_TideFile;
}

CStartupPage::~CStartupPage()
{
}

void CStartupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	if ( pDX->m_bSaveAndValidate )
	{
		m_LastCableFile = m_CableFile;
		m_LastTideFile  = m_TideFile;
	}

	DDX_Text(pDX,IDC_CABLE_FILE,m_CableFile);
	DDX_Text(pDX,IDC_TIDE_FILE,m_TideFile);

#if defined _WIN32_WCE
	DDX_Check(pDX,IDC_DST,m_bIsDST);
#endif
}

#if defined _WIN32_WCE
bool CStartupPage::IsDST()
{
	return m_bIsDST == TRUE ? true : false;
}

void CStartupPage::IsDST(bool bIsDST)
{
	m_bIsDST = (bIsDST ? TRUE : FALSE);
}
#endif


BEGIN_MESSAGE_MAP(CStartupPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CABLE_FILE_BTN, &CStartupPage::OnSelectCableFile)
	ON_BN_CLICKED(IDC_TIDE_FILE_BTN, &CStartupPage::OnSelectTideFile)
	ON_WM_SIZE()
#if defined _WIN32_WCE
	ON_BN_CLICKED(IDC_DST, &CStartupPage::OnBnClickedDst)
#endif
END_MESSAGE_MAP()


// CStartupPage message handlers

void CStartupPage::OnSelectCableFile()
{
	CFileDialog dlg(TRUE);
	dlg.GetOFN().lpstrFilter = _T("Cable Files\0*.CFG\0\0"); 
	if ( dlg.DoModal() == IDOK )
	{
		m_CableFile = dlg.GetPathName(); // returns the fully qualified path name
		UpdateData(FALSE);
	}
}

void CStartupPage::OnSelectTideFile()
{
	CFileDialog dlg(TRUE);
	dlg.GetOFN().lpstrFilter = _T("Tide Files\0*.tides\0\0"); 
	if ( dlg.DoModal() == IDOK )
	{
		m_TideFile = dlg.GetPathName(); // returns the fully qualified path name
		UpdateData(FALSE);
	}
}

BOOL CStartupPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

#if defined _WIN32_WCE
	GetDlgItem(IDC_DST)->ShowWindow(SW_SHOW);
#else
	GetDlgItem(IDC_DST)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStartupPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// adjust the controls to fill the screen
	CWnd* pCableEdit = GetDlgItem(IDC_CABLE_FILE);
	CWnd* pCableBtn  = GetDlgItem(IDC_CABLE_FILE_BTN);
	CWnd* pTideEdit  = GetDlgItem(IDC_TIDE_FILE);
	CWnd* pTideBtn   = GetDlgItem(IDC_TIDE_FILE_BTN);

	if ( pCableEdit == 0 || pCableBtn == 0 || pTideEdit == 0 || pTideBtn == 0 )
		return;

	CRect rCableEdit, rCableBtn, rTideEdit, rTideBtn;
	pCableEdit->GetWindowRect(&rCableEdit);
	pCableBtn->GetWindowRect(&rCableBtn);
	pTideEdit->GetWindowRect(&rTideEdit);
	pTideBtn->GetWindowRect(&rTideBtn);

	CWnd* pParent = GetParent();
	CRect rDlg;
	pParent->GetWindowRect(&rDlg);

	CRect rMe;
	GetWindowRect(&rMe);
	rMe.right = rDlg.right - 10;

	int wBtn = rCableBtn.Width();
	rCableBtn.right = rMe.right - 20;
	rCableBtn.left = rCableBtn.right - wBtn;

	rTideBtn.right = rCableBtn.right;
	rTideBtn.left = rCableBtn.left;

	rCableEdit.right = rCableBtn.left - 7;
	rTideEdit.right  = rTideBtn.left - 7;

	ScreenToClient(rCableEdit);
	ScreenToClient(rCableBtn);
	ScreenToClient(rTideEdit);
	ScreenToClient(rTideBtn);

	pCableEdit->MoveWindow(rCableEdit);
	pCableBtn->MoveWindow(rCableBtn);
	pTideEdit->MoveWindow(rTideEdit);
	pTideBtn->MoveWindow(rTideBtn);
}

BOOL CStartupPage::DidCableDataChange()
{
	return m_LastCableFile != m_CableFile;
}

BOOL CStartupPage::DidTideDataChange()
{
	return m_LastTideFile != m_TideFile;
}

CString CStartupPage::GetCableFile()
{
	return m_CableFile;
}

CString CStartupPage::GetTideFile()
{
	return m_TideFile;
}

#if defined _WIN32_WCE
void CStartupPage::OnBnClickedDst()
{
	bool bIsDST = (IsDlgButtonChecked(IDC_DST) == 1 ? true : false);

#if defined _WIN32_WCE
	AfxGetApp()->WriteProfileInt(_T("Options"),_T("IsDST"),(int)bIsDST);
#endif

	CMainPropSheet* pParent = (CMainPropSheet*)GetParent();
	pParent->IsDST(bIsDST);
}
#endif
