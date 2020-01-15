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
#include "CableTensionSolution.h"

CCableTensionSolution::CCableTensionSolution(void)
{
	m_Pressure = 0;
	m_Tension = 0;
}


CCableTensionSolution::~CCableTensionSolution(void)
{
}

void CCableTensionSolution::SetWaterElevation(float waterElevation)
{
	m_WaterElevation = waterElevation;
}

float CCableTensionSolution::GetWaterElevation() const
{
	return m_WaterElevation;
}

void CCableTensionSolution::SetCalculationTime(const CTime& time)
{
	m_CalculationTime = time;
}

CTime CCableTensionSolution::GetCalculationTime() const
{
	return m_CalculationTime;
}

void CCableTensionSolution::SetGaugePressure(float pressure)
{
	m_Pressure = pressure;
}

float CCableTensionSolution::GetGaugePressure() const
{
	return m_Pressure;
}

void CCableTensionSolution::SetTension(float tension)
{
	m_Tension = tension;
}

float CCableTensionSolution::GetTension() const
{
	return m_Tension;
}
