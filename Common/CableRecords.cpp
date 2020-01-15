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
#include "CableRecords.h"

CCableRecords::CCableRecords(void)
{
}

CCableRecords::~CCableRecords(void)
{
}

void CCableRecords::Init(long nCables)
{
	m_Records.reserve(nCables);
	for ( int i = 0; i < nCables; i++ )
	{
		m_Records.push_back(std::make_pair(CCableRecord(),CCableRecord()));
	}
}

void CCableRecords::SetCableRecords(long index,const CCableRecord& northCableRecord,const CCableRecord& southCableRecord)
{
	m_Records[index].first  = northCableRecord;
	m_Records[index].second = southCableRecord;
}

void CCableRecords::SetCableRecord(long index,CCable::Orientation orientation,const CCableRecord& record)
{
	if ( orientation == CCable::North     ||
 		 orientation == CCable::NorthEast ||
		 orientation == CCable::NorthWest )
	{
		m_Records[index].first  = record;
	}
	else
	{
		m_Records[index].second = record;
	}
}

void CCableRecords::GetCableRecords(long index,CCableRecord* pNorthCableRecord,CCableRecord* pSouthCableRecord) const
{
	std::vector<std::pair<CCableRecord,CCableRecord> >::const_iterator iter = m_Records.begin() + index;
	*pNorthCableRecord = (*iter).first;
	*pSouthCableRecord = (*iter).second;
}

void CCableRecords::GetCableRecord(long index,CCable::Orientation orientation,CCableRecord* pRecord) const
{
	std::vector<std::pair<CCableRecord,CCableRecord> >::const_iterator iter = m_Records.begin() + index;
	*pRecord = (orientation == CCable::North     || 
		        orientation == CCable::NorthEast ||
		        orientation == CCable::NorthWest ? (*iter).first : (*iter).second);
}

long CCableRecords::GetRecordCount() const
{
	return (long)m_Records.size();
}
