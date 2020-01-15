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

// HC_TIDES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HC_TIDES.h"
#include "Tides.h"
#include "ErrorCodes.h"
#include <afxinet.h>
#include <AclAPI.h>

#include <boost\program_options.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


BOOL GetFileFromHTTPServer(const CString& strFileURL, const CString& strLocalTargetFile)
{
   DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
   DWORD dwHttpRequestFlags = /*INTERNET_FLAG_EXISTING_CONNECT |*/ INTERNET_FLAG_DONT_CACHE;

   LPCTSTR pstrAgent = nullptr;

   //the verb we will be using for this connection
   //if nullptr then GET is assumed
   LPCTSTR pstrVerb = _T("GET");

   //the address of the url in the request was obtained from
   LPCTSTR pstrReferer = nullptr;

   //Http version we are using; nullptr = HTTP/1.0
   LPCTSTR pstrVersion = nullptr;

   //For the Accept request headers if we need them later on
   LPCTSTR pstrAcceptTypes = nullptr;
   CString szHeaders = _T("Accept: audio/x-aiff, audio/basic, audio/midi, audio/mpeg, audio/wav, image/jpeg, image/gif, image/jpg, image/png, image/mng, image/bmp, text/plain, text/html, text/htm\r\n");

   //Username we will use if a secure site comes into play
   LPCTSTR pstrUserName = nullptr;
   //The password we will use
   LPCTSTR pstrPassword = nullptr;

   //CInternetSession flags if we need them
   //DWORD dwFlags = INTERNET_FLAG_ASYNC;
   DWORD dwFlags = INTERNET_FLAG_DONT_CACHE;

   //Proxy setting if we need them
   LPCTSTR pstrProxyName = nullptr;
   LPCTSTR pstrProxyBypass = nullptr;

   // see if url parses before going further
   DWORD dwServiceType;
   CString strServer, strObject;
   INTERNET_PORT nPort;

   BOOL bSuccess = AfxParseURL(strFileURL, dwServiceType, strServer, strObject, nPort);
   if (!bSuccess || (dwServiceType != AFX_INET_SERVICE_HTTP && dwServiceType != AFX_INET_SERVICE_HTTPS))
   {
      return FALSE;//eafTypes::hgrInvalidUrl;
   }

   if (dwServiceType == AFX_INET_SERVICE_HTTPS)
   {
      dwFlags |= INTERNET_FLAG_SECURE;
      dwHttpRequestFlags |= INTERNET_FLAG_SECURE;
   }

   CInternetSession	session(pstrAgent, 1, dwAccessType, pstrProxyName, pstrProxyBypass, dwFlags);

   //Set any CInternetSession options we  may need
   int ntimeOut = 30;
   session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * ntimeOut);
   session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
   session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

   //Enable or disable status callbacks
   //session.EnableStatusCallback(FALSE);
   BOOL retVal = FALSE;//eafTypes::HttpGetResult retVal = eafTypes::hgrConnectionError;

   CHttpConnection*	pServer = nullptr;
   CHttpFile* pFile = nullptr;
   DWORD dwRet;
   try
   {
      pServer = session.GetHttpConnection(strServer, nPort, pstrUserName, pstrPassword);
      pFile = pServer->OpenRequest(pstrVerb, strObject, pstrReferer, 1, &pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);

      pFile->AddRequestHeaders(szHeaders);
      CString strHeader;
      strHeader.Format(_T("User-Agent: %s/3.3/r/n"), AfxGetAppName());
      pFile->AddRequestHeaders(strHeader, HTTP_ADDREQ_FLAG_ADD_IF_NEW);
      pFile->SendRequest();

      pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info
                                        //about the status codes


      if (dwRet == HTTP_STATUS_DENIED)
      {
         return FALSE;// eafTypes::hgrConnectionError;
      }

      if (dwRet == HTTP_STATUS_MOVED ||
         dwRet == HTTP_STATUS_REDIRECT ||
         dwRet == HTTP_STATUS_REDIRECT_METHOD)
      {
         CString strNewAddress;
         //again check wininet.h for info on the query info codes
         //there is alot one can do and re-act to based on these codes
         pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewAddress);

         int nPos = strNewAddress.Find(_T("Location: "));
         if (nPos == -1)
         {
            return FALSE;// eafTypes::hgrNotFound;
         }

         strNewAddress = strNewAddress.Mid(nPos + 10);
         nPos = strNewAddress.Find('\n');
         if (0 < nPos)
         {
            strNewAddress = strNewAddress.Left(nPos);
         }

         pFile->Close();
         delete pFile;
         pServer->Close();
         delete pServer;

         CString strServerName;
         CString strObject;
         INTERNET_PORT nNewPort;
         DWORD dwServiceType;

         if (!AfxParseURL(strNewAddress, dwServiceType, strServerName, strObject, nNewPort))
         {
            return FALSE;// eafTypes::hgrInvalidUrl;
         }

         pServer = session.GetHttpConnection(strServerName, nNewPort, pstrUserName, pstrPassword);
         pFile = pServer->OpenRequest(pstrVerb, strObject, pstrReferer, 1, &pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);
         pFile->AddRequestHeaders(szHeaders);
         pFile->SendRequest();

         pFile->QueryInfoStatusCode(dwRet);
         if (dwRet != HTTP_STATUS_OK)
         {
            return FALSE;// eafTypes::hgrNotFound;
         }
      }

      if (dwRet == HTTP_STATUS_OK)
      {
         // Copy file
         ULONGLONG len = pFile->GetLength();
         TCHAR buf[2000];
         int numread;

         CFile myfile(strLocalTargetFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
         while ((numread = pFile->Read(buf, sizeof(buf) / sizeof(TCHAR) - 1)) > 0)
         {
            buf[numread] = _T('\0');
            strObject += buf;
            myfile.Write(buf, numread);
         }
         myfile.Close();

         retVal = TRUE;// eafTypes::hgrOk; // only good exit
      }
      else
      {
         retVal = FALSE;// eafTypes::hgrNotFound;
      }

      pFile->Close();
      delete pFile;

      pServer->Close();
      delete pServer;

      session.Close();
   }
   catch (CInternetException* pEx)
   {
      // catch any exceptions from WinINet      
      TCHAR szErr[1024];
      szErr[0] = _T('\0');
      if (!pEx->GetErrorMessage(szErr, 1024))
      {
         _tcscpy_s(szErr, 1024, _T("Some crazy unknown error"));
      }
      pEx->Delete();
      if (pFile)
      {
         delete pFile;
      }
      if (pServer)
      {
         delete pServer;
      }
      session.Close();

      retVal = FALSE;// eafTypes::hgrNotFound;
   }

   return retVal;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	USES_CONVERSION;

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
		// TODO: code your application's behavior here.
	}

   CTime now = CTime::GetCurrentTime();
   int lastYear = now.GetYear() - 1;
   int thisYear = now.GetYear();
   int nextYear = now.GetYear() + 1;

   if (now.GetMonth() == 12 && 15 <= now.GetDay())
   {
      // it is near the end of the year... assume we want to 
      // generate data for next year
      lastYear++;
      thisYear++;
      nextYear++;
   }

   int stationID;
   float highTideFactor;
   int highTideOffset;
   float lowTideFactor;
   int lowTideOffset;
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()
      ("help", "lists command line options")
      ("year", boost::program_options::value<int>(&thisYear)->default_value(thisYear), "year")
      ("id", boost::program_options::value<int>(&stationID)->default_value(9447130), "tide station identifier")
      ("htf", boost::program_options::value<float>(&highTideFactor)->default_value(0.900), "high tide factor")
      ("hto", boost::program_options::value<int>(&highTideOffset)->default_value(-9),"high tide offset (minutes)")
      ("ltf", boost::program_options::value<float>(&lowTideFactor)->default_value(0.950), "low tide factor")
      ("lto", boost::program_options::value<int>(&lowTideOffset)->default_value(-5), "low tide offset (minutes)")
      ;

   boost::program_options::variables_map variables;
   try
   {
      boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables);
   }
   catch (boost::program_options::unknown_option& e)
   {
      std::cout << "Unknown option " << e.get_option_name() << std::endl;
      return -1;
   }

   boost::program_options::notify(variables);

   if (variables.count("help"))
   {
      std::cout << desc << std::endl;
      return 0;
   }

   if (variables.count("year"))
   {
      thisYear = variables["year"].as<int>();
      lastYear = thisYear - 1;
      nextYear = thisYear + 1;
   }

   if (variables.count("id"))
   {
      stationID = variables["id"].as<int>();
   }

   if (variables.count("htf"))
   {
      highTideFactor = variables["htf"].as<float>();
   }

   if (variables.count("hto"))
   {
      highTideOffset = variables["hto"].as<int>();
   }

   if (variables.count("ltf"))
   {
      lowTideFactor = variables["ltf"].as<float>();
   }

   if (variables.count("lto"))
   {
      lowTideOffset = variables["lto"].as<int>();
   }
   
   CTime startTime[12] =
	{
		CTime(lastYear,12,31,0,0,0), // Jan
		CTime(thisYear, 1,31,0,0,0), // Feb
		CTime(thisYear, 2,28,0,0,0), // Mar
		CTime(thisYear, 3,31,0,0,0), // Apr
		CTime(thisYear, 4,30,0,0,0), // May
		CTime(thisYear, 5,31,0,0,0), // Jun
		CTime(thisYear, 6,31,0,0,0), // Jul
		CTime(thisYear, 7,31,0,0,0), // Aug
		CTime(thisYear, 8,30,0,0,0), // Sep
		CTime(thisYear, 9,31,0,0,0), // Oct
		CTime(thisYear,10,30,0,0,0), // Nov
		CTime(thisYear,11,30,0,0,0), // Dec
	};

	CTime endTime[12] =
	{
		CTime(thisYear, 2, 1,23,0,0), // Jan
		CTime(thisYear, 3, 1,23,0,0), // Feb
		CTime(thisYear, 4, 1,23,0,0), // Mar
		CTime(thisYear, 5, 1,23,0,0), // Apr
		CTime(thisYear, 6, 1,23,0,0), // May
		CTime(thisYear, 7, 1,23,0,0), // Jun
		CTime(thisYear, 8, 1,23,0,0), // Jul
		CTime(thisYear, 9, 1,23,0,0), // Aug
		CTime(thisYear,10, 1,23,0,0), // Sep
		CTime(thisYear,11, 1,23,0,0), // Oct
		CTime(thisYear,12, 1,23,0,0), // Nov
		CTime(nextYear, 1, 1,23,0,0), // Dec
	};

   // We need to create the tides folder if it doesn't exist
   // We want everyone to be able to have full access to this folder.
   // Set up the security descriptor to give full generic access
   PSID pEveryoneSID = NULL;
   PACL pACL = NULL;
   PSECURITY_DESCRIPTOR pSD = NULL;
   EXPLICIT_ACCESS exp_access;
   SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;

   AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID);
   ZeroMemory(&exp_access, sizeof(EXPLICIT_ACCESS));
   exp_access.grfAccessPermissions = GENERIC_ALL;
   exp_access.grfAccessMode = SET_ACCESS;
   exp_access.grfInheritance = NO_INHERITANCE;
   exp_access.Trustee.TrusteeForm = TRUSTEE_IS_SID;
   exp_access.Trustee.ptstrName = (LPTSTR)pEveryoneSID;

   SetEntriesInAcl(1, &exp_access, NULL, &pACL);
   pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
   InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
   SetSecurityDescriptorDacl(pSD, TRUE, pACL, FALSE);

   SECURITY_ATTRIBUTES sa;
   sa.nLength = sizeof(SECURITY_ATTRIBUTES);
   sa.lpSecurityDescriptor = pSD;
   sa.bInheritHandle = FALSE;

   CString strTidesFolder = CTides::GetTidesFolder();
   CreateDirectory(strTidesFolder, &sa); // make sure the folder exists. 

   FreeSid(pEveryoneSID);
   LocalFree(pACL);
   LocalFree(pSD);

   CString strFileName;
   strFileName.Format(_T("%s\\%d_%d.xml"), strTidesFolder, stationID, thisYear);

   CString strURL;
   strURL.Format(_T("https://tidesandcurrents.noaa.gov/api/datagetter?product=predictions&application=NOS.COOPS.TAC.WL&begin_date=%4d0101&end_date=%4d1231&datum=MLLW&station=%d&time_zone=lst&units=english&interval=hilo&format=xml"),thisYear,thisYear,stationID);

   std::cout << "Reading tide prediction data from NOAA" << std::endl;
   std::cout << "Station ID: " << stationID << std::endl;
   std::cout << "Year: " << thisYear << std::endl;
   std::cout << "URL: " << OLE2A(strURL) << std::endl;
   BOOL bResult = GetFileFromHTTPServer(strURL, strFileName);
   if (bResult)
   {
      std::cout << std::endl;
   }
   else
   {
      std::cout << "Failed" << std::endl;
      return -1;
   }

   std::cout << "Loading tide data from " << OLE2A(strFileName) << std::endl;
   CTides tides;
   tides.SetTideCorrectionParameters(highTideFactor, highTideOffset, lowTideFactor, lowTideOffset);
   if (tides.LoadFromXMLFile(strFileName) != SUCCESS)
   {
      std::cout << "Error reading tide file." << std::endl;
      return -1;
   }

	std::cout << "Generating monthly tide files for " << thisYear << std::endl;
	for ( int i = 0; i < 12; i++)
	{
      int month = i + 1;
      CString strTideFile = CTides::GetTidesFile(strTidesFolder, month, thisYear);
      std::cout << month << "\\" << thisYear << " - " << OLE2A(strTideFile) << std::endl;
      tides.SaveWaterElevations(strTideFile,startTime[i],endTime[i]);
	}

	return nRetCode;
}
