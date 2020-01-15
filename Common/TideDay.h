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

#pragma once
#include "Tide.h"
#include <set>

////////////////////////////////////////////////
// Represents all of the tides in a day, stored in
// chronological order
class CTideDay
{
public:
	CTideDay(const CTime& time = CTime::GetCurrentTime());
	~CTideDay(void);

   const CTime& GetTime() const { return m_Time; }

	void AddTide(const CTide& tide);
	long GetTideCount() const;
	CTide GetTide(long idx);

private:
	CTime m_Time;
	std::set<CTide> m_Tides;
};
