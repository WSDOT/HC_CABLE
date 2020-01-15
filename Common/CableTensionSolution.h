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

#ifndef INCLUDED_CABLETENSIONSOLUTION_H_
#define INCLUDED_CABLETENSIONSOLUTION_H_

#pragma once

class CCableTensionSolution
{
public:
	CCableTensionSolution(void);
	~CCableTensionSolution(void);

	void SetGaugePressure(float pressure);
	float GetGaugePressure() const;
	void SetTension(float tension);
	float GetTension() const;
	void SetWaterElevation(float waterElevation);
	float GetWaterElevation() const;
	void SetCalculationTime(const CTime& time);
	CTime GetCalculationTime() const;

private:
	CTime m_CalculationTime;
	float m_WaterElevation;
	float m_Pressure;
	float m_Tension;
};

#endif // INCLUDED_CABLETENSIONSOLUTION_H_
