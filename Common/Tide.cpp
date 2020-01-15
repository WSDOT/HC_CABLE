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
#include "Tide.h"

CTide::CTide(void)
{
}

CTide::~CTide(void)
{
}
	
CTide::CTide(CTime time,float elevation,CTide::TideType type)
{
	m_Type = type;
	m_Time = time;
	m_Elevation = elevation;
}

CTide::TideType CTide::GetType() const
{
	return m_Type;
}

CTime CTide::GetTime() const
{
	return m_Time;
}

float CTide::GetElevation() const
{
	return m_Elevation;
}

bool CTide::operator<(const CTide& other) const
{
	return m_Time < other.m_Time;
}

bool CTide::operator==(const CTide& other) const
{
	return (m_Time == other.m_Time) && (m_Elevation || other.m_Elevation) && (m_Type == other.m_Type);
}
