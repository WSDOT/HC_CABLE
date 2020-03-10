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

#pragma once

#include "StartupPage.h"
#include "CableViewPage.h"
#include "AboutPage.h"
#include "Tides.h"
#include "Cables.h"

// CMainPropSheet

class CMainPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMainPropSheet)

public:
	CMainPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMainPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMainPropSheet();

	BOOL LoadWaterElevations(CWaterElevations* pWaterElevatinos);
	BOOL LoadCableData(CCables* pCables);

#if defined _WIN32_WCE
	bool IsDST();
	void IsDST(bool bIsDST);
#endif

   CStartupPage& GetOptionsPage() { return m_OptionsPage; }


protected:
	DECLARE_MESSAGE_MAP()

	CStartupPage m_OptionsPage;
	CCableViewPage m_CableViewPage;
	CAboutPage m_AboutPage;

   CString m_CableFile;
   CString m_TideFile;

	BOOL m_bTidesLoaded;
	BOOL m_bCablesLoaded;

private:
	void Init(void);

public:
	virtual BOOL OnInitDialog();
};
