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

// HC_CABLE_TEST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HC_CABLE_TEST.h"
#include "ErrorCodes.h"
#include "CableTensionTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// command line arguments
		// output file name (default: cable_tensions.txt)
		// tide elevation (default: 5.7)
		// cable file name (default: HC_CABLE.CFG)
		CString strOutputFile("cable_tensions.txt");
		float tide_elevation = 5.7;
		CString strCableConfig("HC_CABLE.CFG");

		if ( 2 <= argc )
		{
			strOutputFile = argv[1];
		}

		if ( 3 <= argc )
		{
			tide_elevation = _ttof(argv[2]);
		}

		if ( 4 <= argc )
		{
			strCableConfig = argv[3];
		}

		// load cable configuration
		CCables cables;
		if ( cables.LoadFromFile(strCableConfig) != SUCCESS )
		{
			std::cout << "Error loading cable configuration" << std::endl;
			return -1;
		}

		CCableTensionTest test;
		test.SetCables(cables);
		test.SetResultsFile(strOutputFile);
		test.RunTest(tide_elevation);
	}

	return nRetCode;
}
