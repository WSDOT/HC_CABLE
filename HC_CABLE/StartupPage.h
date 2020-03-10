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


// CStartupPage dialog

class CStartupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CStartupPage)

public:
	CStartupPage();
	virtual ~CStartupPage();

// Dialog Data
	enum { IDD = IDD_STARTUPPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_CableFile;
	CString m_TideFile;
#if defined _WIN32_WCE
	BOOL m_bIsDST;
#endif

	DECLARE_MESSAGE_MAP()
public:
   void SetCableFile(CString& strCableFile);
	const CString& GetCableFile() const;
	const CString& GetTideFile() const;
#if defined _WIN32_WCE
	bool IsDST();
	void IsDST(bool bIsDST);
	afx_msg void OnBnClickedDst();
#endif

	afx_msg void OnSelectCableFile();
	afx_msg void OnSelectTideFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
