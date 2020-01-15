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
#include "WaterElevations.h"
#include "ErrorCodes.h"
#include <fstream>
#include <sstream>
#include <iomanip>

CWaterElevations::CWaterElevations(void)
{
	m_bIsDST = false;
}

CWaterElevations::~CWaterElevations(void)
{
}

int CWaterElevations::LoadFromFile(const CString& strFile,CLoadWaterElevationCallback* pCallback)
{
	m_WaterElevations.clear();

	std::ifstream file;
	file.open(strFile);

	if ( !file )
		return FILE_NOT_FOUND;

	char buffer[100];
	while (file)
	{
		file.getline(buffer,100);
		if ( strlen(buffer) == 0 )
			continue;

		std::istringstream is(buffer);
		int year, month, day, hour, min;
		float elevation;
		is >> year >> month >> day >> hour >> min >> elevation;

		CTime theTime(year,month,day,hour,min,0);

		CWaterElevation waterElevation(theTime,elevation);
		m_WaterElevations.insert(waterElevation);

		if ( pCallback )
			pCallback->OnNewWaterElevation(waterElevation);
	}

	return SUCCESS;
}

int CWaterElevations::SaveWaterElevations(const CString& strFile,CTime startTime,CTime endTime)
{
	USES_CONVERSION;
	std::ofstream file;
	file.open(strFile);

	if ( !file )
		return FILE_NOT_FOUND;

	std::set<CWaterElevation>::iterator iter;
	for ( iter = m_WaterElevations.begin(); iter != m_WaterElevations.end(); iter++ )
	{
		CWaterElevation waterElevation = *iter;
		CTime time = waterElevation.GetTime();
		if (startTime <= time && time <= endTime )
		{
			CString strTime = time.Format(_T("%Y %m %d %H %M"));
			int month = time.GetMonth();
			int day = time.GetDay();
			file << OLE2A(strTime) << " "
				 << std::setw(5) << std::setprecision(4) << waterElevation.GetElevation() << "\n";
		}
	}

	file.close();
	return SUCCESS;
}

bool CWaterElevations::IsDST() const
{
	return m_bIsDST;
}

void CWaterElevations::IsDST(bool bIsDST)
{
	m_bIsDST = bIsDST;
}

void CWaterElevations::Clear()
{
	m_WaterElevations.clear();
}

long CWaterElevations::GetCount() const
{
	return (long)m_WaterElevations.size();
}

void CWaterElevations::Add(const CWaterElevation& waterElevation)
{
	m_WaterElevations.insert(waterElevation);
}

CWaterElevation CWaterElevations::GetWaterElevation(long index) const
{
	std::set<CWaterElevation>::const_iterator iter = m_WaterElevations.begin();
	for ( long i = 0; i < index; i++, iter++ ) {}

	return *iter;
}

bool CWaterElevations::GetWaterElevation(CTime time,CWaterElevation* pWaterElevation) const
{
	// round time to 10 minute interval
	int min = time.GetMinute();
	int r = min % 10;
	time -= CTimeSpan(0,0,r,time.GetSecond());

	if ( m_bIsDST )
	{
		time -= CTimeSpan(0,1,0,0); // if this is daylight savings time, deduct 1 hour 
	                                // to make it standard time
	}

	CWaterElevation key(time,0);
	std::set<CWaterElevation>::const_iterator found = m_WaterElevations.find(key);
	if ( found != m_WaterElevations.end() )
	{
		*pWaterElevation = (*found);
		return true;
	}
	return false;
}

