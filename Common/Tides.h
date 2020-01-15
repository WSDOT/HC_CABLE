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

#ifndef INCLUDED_TIDES_H_
#define INCLUDED_TIDES_H_

#pragma once
#include <map>
#include "TideDay.h"
#include "WaterElevations.h"

class CLoadTideCallback
{
public:
	virtual void OnNewTide(const CTide& tide) = 0;
};

class CTides
{
public:
	CTides(void);
	~CTides(void);

   static CString GetTidesFolder();
   static CString GetTidesFile(CString& strPath, int month, int year);

   void SetTideCorrectionParameters(float htf, int hto, float ltf, int lto);

   int LoadFromFile(const CString& strFile, CLoadTideCallback* pCallback = nullptr);
   int LoadFromFile2012(const CString& strFile, CLoadTideCallback* pCallback = nullptr);
   int LoadFromXMLFile(const CString& strFile, CLoadTideCallback* pCallback = nullptr);
   int SaveWaterElevations(const CString& strFile,CTime startTime,CTime endTime);

	void AddTideDay(const CTideDay& tideDay);
	void AddTide(const CTide& tide);
	long GetTideDayCount() const;
	CTideDay GetTideDay(long index);

private:
   std::map<CTime, CTideDay> m_TideDays;
   CWaterElevations m_WaterElevations;

	void UpdateWaterElevations();
	bool GetTideDay(const CTime& time,CTideDay** ppTideDay);
   CTide GetCorrectedTide(CTime time, float elevation, CTide::TideType type);

	CTimeSpan m_HighTideTimeOffset;
	CTimeSpan m_LowTideTimeOffset;
	int m_HighTideTimeOffsetSign;
	int m_LowTideTimeOffsetSign;
	float m_HighTideElevationFactor;
	float m_LowTideElevationFactor;

	bool m_bDirty;

	long m_TimeInterval;
};

#endif // INCLUDED_TIDES_H_