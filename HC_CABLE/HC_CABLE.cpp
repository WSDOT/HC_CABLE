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

// HC_CABLE.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HC_CABLE.h"
#include "MainPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCableApp

BEGIN_MESSAGE_MAP(CCableApp, CWinApp)
END_MESSAGE_MAP()


// CCableApp construction

CCableApp::CCableApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCableApp object

CCableApp theApp;


// CCableApp initialization

BOOL CCableApp::InitInstance()
{
#ifdef _WIN32_WCE
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();
#else
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
#endif

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(IDS_COMPANY);

	CMainPropSheet dlg(_T("Cable Tension Calculator"));
	m_pMainWnd = &dlg;

#if defined _WIN32_WCE
	dlg.IsDST( IsDST() );
#endif

   CString strCableFile = GetProfileString(_T("Options"), _T("CableFile"), dlg.GetOptionsPage().GetCableFile());
   dlg.GetOptionsPage().SetCableFile(strCableFile);

	INT_PTR nResponse = dlg.DoModal();

   WriteProfileString(_T("Options"), _T("CableFile"), dlg.GetOptionsPage().GetCableFile());

#if defined _WIN32_WCE
	WriteProfileInt(_T("Options"),_T("IsDST"),(int)dlg.IsDST());
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

bool CCableApp::IsDST()
{
#if defined _WIN32_WCE
	// Dell Axim doesn't have DST so we have to read a setting from the registry
	UINT bIsDST = GetProfileInt(_T("Options"),_T("IsDST"),0);
	return bIsDST == 1 ? true : false;
#else
	// Desktop computer can get DST from operating system
	CTime now = CTime::GetCurrentTime();
	tm local_time;
	now.GetLocalTm(&local_time);
	return local_time.tm_isdst ? true : false;
#endif
}
