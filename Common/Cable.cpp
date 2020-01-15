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
#include "Cable.h"

CCable::CCable(void)
{
	Initialize("NoName","NoName",North,0,0,0,0,0,0,0);
}

CCable::CCable(const char* strName)
{
	Initialize(strName,strName,North,0,0,0,0,0,0,0);
}

CCable::CCable(const char* strName,const char* strID,CCable::Orientation orientation,float H,float DN,float W,float Thi,float A,float E,float Area)
{
	Initialize(strName,strID,orientation,H,DN,W,Thi,A,E,Area);
}

CCable::~CCable(void)
{
}
void CCable::Initialize(const char* strName,const char* strID,CCable::Orientation orientation,float H,float DN,float W,float Thi,float A,float E,float Area)
{
	m_Name = strName;
	m_ID = strID;
	m_Orientation = orientation;
	m_H = H;
	m_DN = DN;
	m_W = W;
	m_Thi = Thi;
	m_A = A;
	m_E = E;
	m_Area = Area;
}

CString CCable::GetName() const
{
	return m_Name;
}

CString CCable::GetID() const
{
	return m_ID;
}

float CCable::GetH() const
{
	return m_H;
}

float CCable::GetDN() const
{
	return m_DN;
}

float CCable::GetW() const
{
	return m_W;
}

float CCable::GetThi() const
{
	return m_Thi;
}

float CCable::GetA() const
{
	return m_A;
}

float CCable::GetE() const
{
	return m_E;
}

float CCable::GetArea() const
{
	return m_Area;
}

CCable::Orientation CCable::GetOrientation() const
{
	return m_Orientation;
}

CString CCable::GetOrientationString()
{
	switch(m_Orientation)
	{
	case CCable::North:
		return CString(_T("N"));
	case CCable::NorthEast:
		return CString(_T("NE"));
	case CCable::NorthWest:
		return CString(_T("NW"));
	case CCable::South:
		return CString(_T("S"));
	case CCable::SouthEast:
		return CString(_T("SE"));
	case CCable::SouthWest:
		return CString(_T("SW"));
	}

	return CString("");
}

bool CCable::operator<(const CCable& other) const
{
	return m_Name < other.m_Name;
}

