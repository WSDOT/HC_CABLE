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
#include "WaterElevation.h"

CWaterElevation::CWaterElevation(void)
{
}

CWaterElevation::~CWaterElevation(void)
{
}
	
CWaterElevation::CWaterElevation(CTime time,float elevation)
{
	m_Time = time;
	m_Elevation = elevation;
}

CTime CWaterElevation::GetTime() const
{
	return m_Time;
}

float CWaterElevation::GetElevation() const
{
	return m_Elevation;
}

bool CWaterElevation::operator<(const CWaterElevation& other) const
{
	return m_Time < other.m_Time;
}

bool CWaterElevation::operator==(const CWaterElevation& other) const
{
	return (m_Time == other.m_Time) && (m_Elevation || other.m_Elevation);
}
