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
#include "afxdtctl.h"
#include "afxwin.h"
#include "Cables.h"
#include "CableRecords.h"
#include "CableTensions.h"


// CCableRecordDlg dialog

class CCableRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(CCableRecordDlg)

public:
	CCableRecordDlg(CCables* pCables,CCableRecords* records,const CCableTensions* cableTensions,long cableIdx,CCable::Orientation cableOrientation,CWnd* pParent = NULL);   // standard constructor
	virtual ~CCableRecordDlg();

// Dialog Data
	enum { IDD = IDD_RECORD };

	void UpdateTides();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
private:
	CDateTimeCtrl m_TimeCtrl;
	CFont m_Font;

	UINT m_ClockEventID;

	long m_CableIdx;
	CCable::Orientation m_CableOrientation;
	CCables* m_pCables;
	CCableRecords* m_pCableRecords;
	const CCableTensions* m_pCableTensions;

	BOOL m_bLocked;

	CTime m_Time;
	CString m_strCableName;
	CString m_strCableOrientation;

	float m_PredictedPressure;
	float m_ActualPressure;
	float m_TideElevation;
	CString m_Remarks;

	afx_msg void OnPrevRecord();
	afx_msg void OnNextRecord();
	void OnNewCable();
	void SaveCableRecord();
	void SetControlState();

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedReset();
};
