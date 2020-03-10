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
#include "CableTensions.h"

CCableTensions::CCableTensions(void)
{
}

CCableTensions::~CCableTensions(void)
{
}

void CCableTensions::Init(long nCables)
{
	m_CableTensions.reserve(nCables);
   m_CableTensions.clear();
	for ( int i = 0; i < nCables; i++ )
	{
		m_CableTensions.push_back(std::make_pair(CCableTensionSolution(),CCableTensionSolution()));
	}
}

void CCableTensions::SetCableTensionSolutions(long index,const CCableTensionSolution& northCable,const CCableTensionSolution& southCable)
{
	m_CableTensions[index].first  = northCable;
	m_CableTensions[index].second = southCable;
}

void CCableTensions::SetCableTensionSolution(long index,CCable::Orientation orientation,const CCableTensionSolution& record)
{
	if ( orientation == CCable::North     ||
 		 orientation == CCable::NorthEast ||
		 orientation == CCable::NorthWest )
	{
		m_CableTensions[index].first  = record;
	}
	else
	{
		m_CableTensions[index].second = record;
	}
}

void CCableTensions::GetCableTensionSolutions(long index,CCableTensionSolution* pNorthCableRecord,CCableTensionSolution* pSouthCableRecord) const
{
	std::vector<std::pair<CCableTensionSolution,CCableTensionSolution> >::const_iterator iter = m_CableTensions.begin() + index;
	*pNorthCableRecord = (*iter).first;
	*pSouthCableRecord = (*iter).second;
}

void CCableTensions::GetCableTensionSolution(long index,CCable::Orientation orientation,CCableTensionSolution* pRecord) const
{
	std::vector<std::pair<CCableTensionSolution,CCableTensionSolution> >::const_iterator iter = m_CableTensions.begin() + index;
	*pRecord = (orientation == CCable::North     || 
		        orientation == CCable::NorthEast ||
		        orientation == CCable::NorthWest ? (*iter).first : (*iter).second);
}

long CCableTensions::GetRecordCount() const
{
	return (long)m_CableTensions.size();
}
