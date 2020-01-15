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
#include "CableTensionSolver.h"
#include <math.h>

CCableTensionSolver::CCableTensionSolver(void)
{
	m_NormalWaterElevation = 5.7; // feet
}

CCableTensionSolver::~CCableTensionSolver(void)
{
}

CCableTensionSolution CCableTensionSolver::Solve(const CWaterElevation& waterElevation,const CCable& cable)
{
	CCableTensionSolution solution;

	float A    = cable.GetA();
	float Area = cable.GetArea();
	float DN   = cable.GetDN();
	float E    = cable.GetE();
	float H    = cable.GetH();
	float Thi  = cable.GetThi();
	float W    = cable.GetW();

	// constants that control the Th iteration
	float Th_Step        = 0.02; // Th tuhe step rate in KIPS
	float Thi_Gain_Limit = 20000; // max adjustment for Th
	float Th_Thi_Diff    = 0.002; // percent diff Th:Thi/100

	//
	// Step 1: Find the unstressed cable length at normal water elevation
	//

	// Find length along chord between cable supports at normal water elevation
	float Lci = sqrt(H*H + DN*DN);

	// compute constant Ki
	float Ki = W*H*H/(8*Thi*Lci);

	// Find length along cable at normal water level
	float Li = Lci*(1 + 8*pow(Ki,2)/3 - 32*pow(Ki,4)/5 + 256*pow(Ki,6)/7);

	// Find change in cable length due to stress at normal water elevation
	float delta_Li = Thi*Li*Li/(H*A*E);

	// Find unstressed cable length at normal water elevation
	float UL = Li - delta_Li;

	//
	// Step 2: Calculate cable tension at jack gauge pressure for the tide level at the time of calculation
	//

	// find length along chord between cable supports
	float current_water_level = waterElevation.GetElevation();
	float D = DN - m_NormalWaterElevation + current_water_level;
	float Lc = sqrt(H*H + D*D);

	float Th_factor = 0;
	float Th_calc;

	if ( current_water_level < m_NormalWaterElevation )
	{
		Th_factor = -Th_Step;
	}
	else if ( m_NormalWaterElevation < current_water_level )
	{
		Th_factor = Th_Step;
	}

	float Th = Thi;
	do 
	{
		Th += Th_factor;

		// compute K
		float K = W*H*H/(8*Th*Lc);

		// compute L
		float L = Lc*(1 + 8*pow(K,2)/3 - 32*pow(K,4)/5 + 256*pow(K,6)/7);

		// compute delta L
		float delta_L = L - UL;

		// compute Th
		Th_calc = delta_L*H*A*E/(L*L);

		if ( fabs(Th - Th_calc) < Th*Th_Thi_Diff )
			break;

	} while (fabs(Th - Th_calc) < Thi_Gain_Limit);

	if ( fabs(Th-Th_calc) > Thi_Gain_Limit )
		Th = -1; // solution did not converge

	// Compute cable tension
	float T = 0;
	if ( Th < 0 )
	{
		T = 0;
	}
	else
	{
		float tan_beta = ((W*Lc*H) + (2*Th*D))/(2*Th*H);
		float beta = atan(tan_beta); // radians
		T = Th/cos(beta);
	}

	// Compute Gauge Pressure
	float G = T*1000/Area;

	solution.SetTension(T);
	solution.SetGaugePressure(G);

	return solution;
}

void CCableTensionSolver::SetNormalWaterElevation(float normal)
{
	m_NormalWaterElevation = normal;
}

float CCableTensionSolver::GetNormalWaterElevation() const
{
	return m_NormalWaterElevation;
}
