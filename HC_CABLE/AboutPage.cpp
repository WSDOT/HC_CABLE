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

// AboutPage.cpp : implementation file
//

#include "stdafx.h"
#include "HC_CABLE.h"
#include "AboutPage.h"


// CAboutPage dialog

IMPLEMENT_DYNAMIC(CAboutPage, CPropertyPage)

CAboutPage::CAboutPage()
	: CPropertyPage(CAboutPage::IDD)
{

}

CAboutPage::~CAboutPage()
{
}

BEGIN_MESSAGE_MAP(CAboutPage, CPropertyPage)
END_MESSAGE_MAP()


// CAboutPage message handlers
