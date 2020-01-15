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
#include "CableTensionSolution.h"

class CCableRecord
{
public:
	CCableRecord(void);
	CCableRecord(const CTime& time,float tideElevation,float predictedPressure,float actualPressure,const CTime& measureTime,const CString& strRemark);
	~CCableRecord(void);

	void Init(const CTime& time,float tideElevation,float predictedPressure,float actualPressure,const CTime& measureTime,const CString& strRemark);
	void SetTime(const CTime& time);
	CTime GetTime() const;
	void SetMeasurementTime(const CTime& time);
	CTime GetMeasurementTime() const;
	void SetTideElevation(float elevation);
	float GetTideElevation() const;
	void SetPredictedPressure(float pressure);
	float GetPredictedPressure() const;
	void SetActualPressure(float pressure);
	float GetActualPressure() const;
	void SetRemark(const CString& strRemark);
	CString GetRemark() const;

private:
	CTime m_Time;
	float m_TideElevation;
	float m_PredictedPressure;

	CTime m_MeasurementTime;
	float m_ActualPressure;
	CString m_Remark;
};
