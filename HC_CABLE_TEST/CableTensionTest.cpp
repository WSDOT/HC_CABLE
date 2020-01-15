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
#include "CableTensionTest.h"
#include "CableTensionSolver.h"
#include <fstream>
#include <iomanip>

CCableTensionTest::CCableTensionTest(void)
{
}

CCableTensionTest::~CCableTensionTest(void)
{
}

void CCableTensionTest::SetResultsFile(const CString& strFile)
{
	m_strFile = strFile;
}

void CCableTensionTest::SetCables(const CCables& cables)
{
	m_Cables = cables;
}

struct TestResults
{
	CCable NorthCable;
	CCableTensionSolution NorthCableSolution;
	CCable SouthCable;
	CCableTensionSolution SouthCableSolution;
};

void CCableTensionTest::RunTest(float tideElevation)
{
	CWaterElevation waterElevation(CTime::GetCurrentTime(),tideElevation);
	RunTest(waterElevation);
}

void CCableTensionTest::RunTest(const CWaterElevation& waterElevation)
{
   USES_CONVERSION;
	CCableTensionSolver solver;

	long nCables = m_Cables.GetCableCount();
	std::vector<TestResults> solutions;
	for ( long index = 0; index < nCables; index++ )
	{
		CCable northCable, southCable;
		m_Cables.GetCablePair(index,&northCable,&southCable);

		CCableTensionSolution northSolution = solver.Solve(waterElevation,northCable);
		CCableTensionSolution southSolution = solver.Solve(waterElevation,southCable);

		TestResults results;
		results.NorthCable = northCable;
		results.NorthCableSolution = northSolution;
		results.SouthCable = southCable;
		results.SouthCableSolution = southSolution;

		solutions.push_back(results);
	}

	std::ofstream file;
	file.open(m_strFile);
	file << "Tide : " << waterElevation.GetElevation() << " ft" << std::endl;
	file << "----- North -----              ---- South ------" << std::endl;
	file << "   Gauge  Tension     Cable    Tension     Gauge" << std::endl;
	
	std::vector<TestResults>::iterator iter;
	for ( iter = solutions.begin(); iter != solutions.end(); iter++ )
	{
		TestResults results = *iter;
		file << std::setw(8) << std::setprecision(4)
			 << results.NorthCableSolution.GetGaugePressure() << "  " 
			 << std::setw(7) << std::setprecision(3)
			 << results.NorthCableSolution.GetTension() << "  "
			 << std::setw(2)
			 << OLE2A(GetOrientation(results.NorthCable.GetOrientation())) << " -"
			 << std::setw(2)
			 << OLE2A(results.NorthCable.GetID()) << "- "
			 << std::setw(2)
			 << OLE2A(GetOrientation(results.SouthCable.GetOrientation())) << "  "
			 << std::setw(7) << std::setprecision(3)
			 << results.SouthCableSolution.GetTension() << "  "
		     << std::setw(8) << std::setprecision(4)
			 << results.SouthCableSolution.GetGaugePressure() << std::endl;
	}
	file.close();
}

CString CCableTensionTest::GetOrientation(CCable::Orientation orientation)
{
	switch(orientation)
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
