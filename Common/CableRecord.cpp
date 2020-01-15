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

#include "stdafx.h"
#include "CableRecord.h"

CCableRecord::CCableRecord(void)
{
	CTime time = CTime::GetCurrentTime();
	Init(time,-1,-1,-1,time,CString(""));
}

CCableRecord::CCableRecord(const CTime& time,float tideElevation,float predictedPressure,float actualPressure,const CTime& measureTime,const CString& strRemark)
{
	Init(time,tideElevation,predictedPressure,actualPressure,measureTime,strRemark);
}

CCableRecord::~CCableRecord(void)
{
}

void CCableRecord::Init(const CTime& time,float tideElevation,float predictedPressure,float actualPressure,const CTime& measureTime,const CString& strRemark)
{
	m_Time = time;
	m_TideElevation = tideElevation;
	m_PredictedPressure = predictedPressure;

	m_MeasurementTime = measureTime;
	m_ActualPressure = actualPressure;
	m_Remark = strRemark;
}

void CCableRecord::SetTime(const CTime& time)
{
	m_Time = time;
}

CTime CCableRecord::GetTime() const
{
	return m_Time;
}

void CCableRecord::SetMeasurementTime(const CTime& time)
{
	m_MeasurementTime = time;
}

CTime CCableRecord::GetMeasurementTime() const
{
	return m_MeasurementTime;
}

void CCableRecord::SetTideElevation(float elevation)
{
	m_TideElevation = elevation;
}

float CCableRecord::GetTideElevation() const
{
	return m_TideElevation;
}

void CCableRecord::SetPredictedPressure(float pressure)
{
	m_PredictedPressure = pressure;
}

float CCableRecord::GetPredictedPressure() const
{
	return m_PredictedPressure;
}

void CCableRecord::SetActualPressure(float pressure)
{
	m_ActualPressure = pressure;
}

float CCableRecord::GetActualPressure() const
{
	return m_ActualPressure;
}

void CCableRecord::SetRemark(const CString& strRemark)
{
	m_Remark = strRemark;
}

CString CCableRecord::GetRemark() const
{
	return m_Remark;
}
