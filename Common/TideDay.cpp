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
#include "TideDay.h"

CTideDay::CTideDay(const CTime& time)
{
	m_Time = time;
}

CTideDay::~CTideDay(void)
{
}

void CTideDay::AddTide(const CTide& tide)
{
	ASSERT( tide.GetTime().GetYear()  == m_Time.GetYear() &&
		    tide.GetTime().GetMonth() == m_Time.GetMonth() &&
			tide.GetTime().GetDay()   == m_Time.GetDay());

	m_Tides.insert(tide);
}

long CTideDay::GetTideCount() const
{
	return (long)m_Tides.size();
}

CTide CTideDay::GetTide(long index)
{
	std::set<CTide>::const_iterator iter = m_Tides.begin();
	for ( long i = 0; i < index; i++, iter++ ) {}

	return *iter;
}
//
//bool CTideDay::operator<(const CTideDay& other) const
//{
//	// only compare date
//	CTime me(m_Time.GetYear(),m_Time.GetMonth(),m_Time.GetDay(),0,0,0);
//	CTime you(other.m_Time.GetYear(),other.m_Time.GetMonth(),other.m_Time.GetDay(),0,0,0);
//	return me < you;
//}
