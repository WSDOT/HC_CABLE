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
#include "Cables.h"
#include "WaterElevations.h"
#include "CableTensionSolver.h"
#include "CableRecords.h"
#include "CableTensions.h"
#include "CableRecordDlg.h"
#include "afxwin.h"
#include "afxdtctl.h"

// CCableViewPage dialog

class CCableViewPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCableViewPage)

public:
	CCableViewPage();
	virtual ~CCableViewPage();

// Dialog Data
	enum { IDD = IDD_CABLEVIEWPAGE };

	void UpdateForTime(CTime now);
	void IsDST(bool bIsDST);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL LoadCableData();
	BOOL LoadWaterElevations();

	void UpdateCableTensions(const CWaterElevation& waterElevation);
	void UpdateCableList();

	void StartRealTimeCalculations();
	void EndRealTimeCalculations();

	void UpdateColumnWidths(bool bForceUpdate = false);

	CCableRecordDlg* m_pCableRecordDlg;

	CStatic m_TideElevationLabel;
	CListCtrl m_CableList;
	CFont m_Font;
   CDateTimeCtrl m_ctrlDate;

   UINT_PTR m_ClockEventID;
   UINT_PTR m_RealTimeEventID;
	int m_TimeInterval;

	CWaterElevation m_CalculationWaterElevation;
	CCableTensions m_CableTensions;

	CString m_strCurrentTimeFormat;
	CString m_strCalculationTimeFormat;
	CCableTensionSolver m_Solver;
	CCableRecords m_CableRecords;

   CTime m_UserDate;
   float m_TideElevation;

	CCables m_Cables;
	CWaterElevations m_WaterElevations;

	bool LoadCableRecords();
	bool SaveCableRecords();

	bool m_bIsDirty;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnModeChanged();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateForTide();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnSetActive();
	afx_msg void OnCableDblClicked(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSave();
	virtual BOOL OnKillActive();
	afx_msg void OnBnClickedLoad();
};
