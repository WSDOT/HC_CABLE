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

#ifndef INCLUDED_CABLETENSIONSOLVER_H_
#define INCLUDED_CABLETENSIONSOLVER_H_

#pragma once

#include "WaterElevation.h"
#include "Cable.h"
#include "CableTensionSolution.h"

class CCableTensionSolver
{
public:
	CCableTensionSolver(void);
	~CCableTensionSolver(void);

	CCableTensionSolution Solve(const CWaterElevation& waterElevation,const CCable& cable);
	void SetNormalWaterElevation(float normal);
	float GetNormalWaterElevation() const;

private:
	float m_NormalWaterElevation;
};

#endif // INCLUDED_CABLETENSIONSOLVER_H_
