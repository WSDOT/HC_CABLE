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

#ifndef INCLUDED_CABLES_H_
#define INCLUDED_CABLES_H_

#pragma once
#include "Cable.h"
#include <vector>

class CLoadCableCallback
{
public:
	virtual void OnNewCable(const CCable& northCable,const CCable& southCable) = 0;
};

class CCables
{
public:
	CCables(void);
	~CCables(void);

	int LoadFromFile(const CString& strFile,CLoadCableCallback* pCallback = NULL);
	void GetCablePair(long index,CCable* pNorthCable,CCable* pSouthCable) const;
	void GetCable(long index,CCable::Orientation orientation,CCable* pCable) const;
	long GetCableCount() const;
	bool FindCable(const CString& strName,long* pIndex,CCable::Orientation* pOrientation) const;

private:
	std::vector< std::pair<CCable,CCable> > m_Cables;
	CCable CableFromLine(const char* strInput);
};

#endif // INCLUDED_CABLES_H_
