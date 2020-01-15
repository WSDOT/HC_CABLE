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

#ifndef INCLUDED_CABLE_H_
#define INCLUDED_CABLE_H_
#pragma once

class CCable
{
public:
	enum Orientation { North,
					   NorthWest,
					   NorthEast,
					   South,
					   SouthWest,
					   SouthEast
	                 };

	CCable(void);
	CCable(const char* strName,const char* strID,Orientation orientation,float H,float DN,float W,float Thi,float A,float E,float Area);
	CCable(const char* strName);
	~CCable(void);

	void Initialize(const char* strName,const char* strID,Orientation orientation,float H,float DN,float W,float Thi,float A,float E,float Area);
	CString GetName() const;
	CString GetID() const;
	float GetH() const;
	float GetDN() const;
	float GetW() const;
	float GetThi() const;
	float GetA() const;
	float GetE() const;
	float GetArea() const;
	Orientation GetOrientation() const;
	CString GetOrientationString();

	bool operator<(const CCable& other) const;

private:
	CString m_Name;
	CString m_ID;
	Orientation m_Orientation;
	float m_H;
	float m_DN;
	float m_W;
	float m_Thi;
	float m_A;
	float m_E;
	float m_Area;
};

#endif // INCLUDED_CABLE_H_
