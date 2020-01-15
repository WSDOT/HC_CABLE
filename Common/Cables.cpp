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
#include "Cables.h"
#include "ErrorCodes.h"
#include <fstream>
#include <sstream>

CCables::CCables(void)
{
}

CCables::~CCables(void)
{
}

int CCables::LoadFromFile(const CString& strFile,CLoadCableCallback* pCallback)
{
	m_Cables.clear();

	std::ifstream file;
	file.open(strFile);
	if ( !file )
		return FILE_NOT_FOUND;

	char buffer[100];

	while (file)
	{
		file.getline(buffer,100);
		if ( strlen(buffer) == 0 || buffer[0] == '#' ) // skip blank lines and comments
			continue;

		CCable northCable = CableFromLine(buffer);

		file.getline(buffer,100);
		if ( strlen(buffer) == 0 || buffer[0] == '#' ) // skip blank lines and comments
			continue;

		CCable southCable = CableFromLine(buffer);

		m_Cables.push_back(std::make_pair(northCable,southCable));

		if ( pCallback )
			pCallback->OnNewCable(northCable,southCable);
	}

	return SUCCESS;
}

void CCables::GetCablePair(long index,CCable* pNorthCable,CCable* pSouthCable) const
{
	std::vector<std::pair<CCable,CCable> >::const_iterator iter = m_Cables.begin() + index;
	*pNorthCable = (*iter).first;
	*pSouthCable = (*iter).second;
}

void CCables::GetCable(long index,CCable::Orientation orientation,CCable* pCable) const
{
	std::vector<std::pair<CCable,CCable> >::const_iterator iter = m_Cables.begin() + index;
	*pCable = (orientation == CCable::North     || 
		       orientation == CCable::NorthEast ||
		       orientation == CCable::NorthWest ? (*iter).first : (*iter).second);
}

long CCables::GetCableCount() const
{
	return (long)m_Cables.size();
}

bool CCables::FindCable(const CString& strName,long* pIndex,CCable::Orientation* pOrientation) const
{
	std::vector<std::pair<CCable,CCable> >::const_iterator iter;
	for ( iter = m_Cables.begin(); iter != m_Cables.end(); iter++ )
	{
		const CCable& north = (*iter).first;
		const CCable& south = (*iter).second;

		if ( north.GetName() == strName )
		{
			*pIndex = long(std::distance(m_Cables.begin(),iter));
			*pOrientation = north.GetOrientation();
			return true;
		}
		else if ( south.GetName() == strName )
		{
			*pIndex = long(std::distance(m_Cables.begin(),iter));
			*pOrientation = south.GetOrientation();
			return true;
		}
	}

	return false;
}

CCable CCables::CableFromLine(const char* strInput)
{
	// parse the data out of the line
	std::istringstream is(strInput);
	std::string name;
	float H, DN, W, Thi, A, E, Area;
	CCable::Orientation orientation;

	is >> name >> H >> DN >> W >> Thi >> A >> E >> Area;

	// name has to have at least 2 characters
	if ( name.length() < 2 )
		return false;

	// determine the cable orientation from its name
	int nRemove = 1;
	std::string::value_type c = name[name.length()-1];
	if ( c == 'N' )
		orientation = CCable::North;
	else if ( c == 'S' )
		orientation = CCable::South;
	else
	{
		ASSERT(c == 'E' || c == 'W');
		std::string::value_type c1 = name[name.length()-2];
		std::string strOrientation(std::string(1,c1) + std::string(1,c));
		if ( strOrientation == "NW" )
			orientation = CCable::NorthWest;
		else if ( strOrientation == "NE" )
			orientation = CCable::NorthEast;
		else if ( strOrientation == "SW" )
			orientation = CCable::SouthWest;
		else if ( strOrientation == "SE" )
			orientation = CCable::SouthEast;
		else
			return false;

		nRemove = 2;
	}

	// get the cable id by remove the orientation from the end of the name
	std::string id = name.substr(0,name.length()-nRemove);
	CCable cable(name.c_str(),id.c_str(),orientation,H,DN,W,Thi,A,E,Area);

	return cable;
}
