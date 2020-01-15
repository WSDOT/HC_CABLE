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
#include "Tides.h"
#include "ErrorCodes.h"
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include "TidalPredictions.hxx"

#pragma warning(disable:4244)

template <class T>
inline int BinarySign(const T& a)
{
   return int(a < 0 ? -1 : 1);
}

CTides::CTides(void) :
m_HighTideTimeOffset(0,0,0,0),
m_HighTideTimeOffsetSign(1),
m_HighTideElevationFactor(0.0),
m_LowTideTimeOffset(0,0,0,0),
m_LowTideTimeOffsetSign(1),
m_LowTideElevationFactor(0),
m_TimeInterval(10),
m_bDirty(true)
{
}

CTides::~CTides(void)
{
}

CString CTides::GetTidesFolder()
{
   // Tides files are stored in C:\ProgramData\HC_CABLE
   PWSTR pPath;
   HRESULT result = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, 0, &pPath);
   CString strRoot(pPath);
   ::CoTaskMemFree((LPVOID)pPath);

   strRoot += _T("\\HC_Tides");

   return strRoot;
}

CString CTides::GetTidesFile(CString& strPath, int month, int year)
{
   CString strTidesFile;
   strTidesFile.Format(_T("%s\\PortGamble_%02d_%4d.tides"), strPath, month, year);
   return strTidesFile;
}

void CTides::SetTideCorrectionParameters(float htf, int hto, float ltf, int lto)
{
   m_HighTideTimeOffset = CTimeSpan(0, 0, abs(hto), 0);
   m_LowTideTimeOffset = CTimeSpan(0,0,abs(ltf),0);
   m_HighTideTimeOffsetSign = ::BinarySign(hto);
   m_LowTideTimeOffsetSign = ::BinarySign(lto);
   m_HighTideElevationFactor = htf;
   m_LowTideElevationFactor = ltf;
}

int CTides::LoadFromFile(const CString& strFile,CLoadTideCallback* pCallback)
{
	m_bDirty = true;

	m_TideDays.clear();

	std::ifstream file;
	file.open(strFile);

	if ( !file )
		return FILE_NOT_FOUND;

	const int BUFFER_SIZE = 400;
	char buffer[BUFFER_SIZE];
	while (file)
	{
		file.getline(buffer,BUFFER_SIZE);
		
		std::string strBuffer(buffer);
		
		// if the buffer is less than 10, then there is no way it can be a date record
		if ( strBuffer.length() < 10 )
			continue;

		// date is in the for mm/dd/yyyy... if the character at index 0 and 5 aren't /
		// that this isn't a tide record
		if ( !(buffer[2] == '/' && buffer[5] == '/') )
			continue;

		// read date
		int month, day, year;
		month = atoi(strBuffer.substr(0,2).c_str());
		day   = atoi(strBuffer.substr(3,2).c_str());
		year  = atoi(strBuffer.substr(6,4).c_str());

		// read tide
		for ( int i = 0; i < 5; i++ )
		{
			// is there more???... if we are going to read past the 
			// end of the buffer, then there isn't any more
			if (strBuffer.length() <= 15+i*20)
				break;

			// read time
			int hour, min;
			hour = atoi(strBuffer.substr(15+i*20,2).c_str());
			min  = atoi(strBuffer.substr(18+i*20,2).c_str());

			std::string am_pm = strBuffer.substr(20+i*20,2);
			
			if ( hour == 12 && am_pm == std::string("AM") )
				hour = 0;

			if ( am_pm == std::string("PM") && hour < 12 )
			{
				hour += 12;
			}

			CTime time(year,month,day,hour,min,0);

			std::string lst_ldt = strBuffer.substr(23+i*20,3);
			if ( lst_ldt == std::string("LDT") )
			{
				// deduct one hour if day saving time to make it standard time
				time -= CTimeSpan(0,1,0,0);
			}

			// read elevation
			float elevation = atof(strBuffer.substr(27+i*20,4).c_str());

			// type
			CTide::TideType type = (buffer[32+i*20] == 'H' ? CTide::High : CTide::Low);

         // get corrected tide data
         CTide tide = GetCorrectedTide(time, elevation, type);

			// save tide
			AddTide(tide);
			
			if ( pCallback )
				pCallback->OnNewTide(tide);
		}
	}

	return SUCCESS;
}

int CTides::LoadFromFile2012(const CString& strFile,CLoadTideCallback* pCallback)
{
	m_bDirty = true;

	m_TideDays.clear();

	std::ifstream file;
	file.open(strFile);

	if ( !file )
		return FILE_NOT_FOUND;

	const int BUFFER_SIZE = 400;
	char buffer[BUFFER_SIZE];
   std::string lst_ldt;
#if defined _DEBUG
   CTide::TideType prev_type;
#endif
	while (file)
	{
		file.getline(buffer,BUFFER_SIZE);
		
		std::string strBuffer(buffer);
		
		// if the buffer is less than 10, then there is no way it can be a date record
		if ( strBuffer.length() < 10 )
			continue;

      if ( strBuffer.substr(0,9) == std::string("Stationid") )
      {
         long stationId = atoi(strBuffer.substr(11,7).c_str());
         if ( stationId != 9447130 )
            return INVALID_STATION; // expecting the Seattle tide station
      }

      if ( strBuffer.substr(0,9) == std::string("Time Zone") )
      {
         lst_ldt = strBuffer.substr(11,3);
      }

		// date is in the for yyyy/mm/dd... if the character at index 4 and 7 aren't /
		// that this isn't a tide record
		if ( !(buffer[4] == '/' && buffer[7] == '/') )
			continue;

		// read date
		int month, day, year;
		year  = atoi(strBuffer.substr(0,4).c_str());
		month = atoi(strBuffer.substr(5,2).c_str());
		day   = atoi(strBuffer.substr(8,2).c_str());

		// read tide data (one per line)

		// read time
		int hour, min;
		hour = atoi(strBuffer.substr(15,2).c_str());
		min  = atoi(strBuffer.substr(18,2).c_str());

		std::string am_pm = strBuffer.substr(21,2);
		
		if ( hour == 12 && am_pm == std::string("AM") )
			hour = 0;

		if ( am_pm == std::string("PM") && hour < 12 )
		{
			hour += 12;
		}

		CTime time(year,month,day,hour,min,0);

		if ( lst_ldt == std::string("LDT") )
		{
			// deduct one hour if day saving time to make it standard time
			time -= CTimeSpan(0,1,0,0);
		}

		// read elevation
		float elevation = atof(strBuffer.substr(24,5).c_str());

		// type (high/low)
		CTide::TideType type = (strBuffer.at(strBuffer.length()-1) == 'H' ? CTide::High : CTide::Low);

#if defined _DEBUG
      if ( this->m_TideDays.size() == 0 )
      {
         prev_type = type;
      }
      else
      {
         ASSERT(prev_type != type);
         prev_type = type;
      }
#endif

      // get corrected tide data
      CTide tide = GetCorrectedTide(time, elevation, type);

		// save tide
		AddTide(tide);
		
		if ( pCallback )
			pCallback->OnNewTide(tide);
	}

	return SUCCESS;
}

int CTides::LoadFromXMLFile(const CString& strFile, CLoadTideCallback* pCallback)
{
   USES_CONVERSION;

   m_bDirty = true;
   m_TideDays.clear();

   // Get application path... the location of the XSD file
   TCHAR szBuff[_MAX_PATH];
   ::GetModuleFileName(AfxGetInstanceHandle(), szBuff, _MAX_PATH);
   CString filename(szBuff);
   CFile file;
   file.SetFilePath(filename);
   CString path = file.GetFilePath();
   path.Replace(file.GetFileName(), _T(""));

   path += _T("TidalPredictions.xsd");

   xml_schema::properties props;
   props.no_namespace_schema_location(std::wstring(path));
   std::auto_ptr<data> data;
   try
   {
      data = data_(OLE2T(strFile.AllocSysString()), 0, props);
   }
   catch (xml_schema::exception& /*e*/)
   {
      return ERROR_PARSING_XML;
   }

   const data::pr_sequence& records = data->pr();
   for (const auto& record : records)
   {
      const auto& t = record.t(); // date and time in "yyyy-mm-dd hh:mm" format
      const auto& v = record.v();
      const auto& type = record.type();

      // date and time
      CString strDateTime(t.get().c_str());
      int year = _ttoi(strDateTime.Left(4));
      int month = _ttoi(strDateTime.Mid(5, 2));
      int day = _ttoi(strDateTime.Mid(8, 2));
      int hour = _ttoi(strDateTime.Mid(11, 2));
      int minute = _ttoi(strDateTime.Mid(14, 2));

      CTime time(year, month, day, hour, minute, 0);

      // elevation
      float elevation = v.get();

      // tide type
      CTide::TideType tide_type = (type.get().c_str()[0] == 'H' ? CTide::High : CTide::Low);

      // Correct the tide information
      CTide tide = GetCorrectedTide(time, elevation, tide_type);

      // save tide
      AddTide(tide);

      if (pCallback)
         pCallback->OnNewTide(tide);
   }

   return SUCCESS;
}

int CTides::SaveWaterElevations(const CString& strFile,CTime startTime,CTime endTime)
{
	UpdateWaterElevations();
	return m_WaterElevations.SaveWaterElevations(strFile,startTime,endTime);
}

void CTides::AddTideDay(const CTideDay& tideDay)
{
	m_bDirty = true;

	m_TideDays.insert(std::make_pair(tideDay.GetTime(),tideDay));
}

void CTides::AddTide(const CTide& tide)
{
	m_bDirty = true;

	CTideDay* pTideDay = 0;
	if ( !GetTideDay(tide.GetTime(),&pTideDay) )
	{
		CTideDay newTideDay(tide.GetTime());
		m_TideDays.insert(std::make_pair(newTideDay.GetTime(),newTideDay));
		GetTideDay(tide.GetTime(),&pTideDay);
	}

	pTideDay->AddTide(tide);
}

long CTides::GetTideDayCount() const
{
	return (long)m_TideDays.size();
}

CTideDay CTides::GetTideDay(long index)
{
	auto iter = m_TideDays.begin();
	for ( long i = 0; i < index; i++, iter++ ) {}

	return iter->second;
}

bool CTides::GetTideDay(const CTime& time,CTideDay** ppTideDay)
{
	auto found = m_TideDays.find(time);
	if ( found == m_TideDays.end() )
		return false;

	(*ppTideDay) = &(found->second);
	return true;
}

void CTides::UpdateWaterElevations()
{
	if ( !m_bDirty )
		return;

	m_WaterElevations.Clear();

	// fill up a vector with tides to make the calculations easier
	std::vector<CTide> tides;

   for( auto& tideDayItem : m_TideDays)
	{
      CTideDay& tideDay = tideDayItem.second;

		long nTides = tideDay.GetTideCount();
		for ( long tideIdx = 0; tideIdx < nTides; tideIdx++ )
		{
			CTide tide = tideDay.GetTide(tideIdx);

//			TRACE(_T("%s %s %.1f\n"),tide.GetTime().Format(_T("%m/%d/%Y %H:%M")),tide.GetType() == CTide::High ? "H" : "L",tide.GetElevation());

			tides.push_back(tide);
		}
	}

	// do the water elevation predictions
	std::vector<CTide>::iterator iterCurrentTide, iterNextTide;
	iterCurrentTide = tides.begin();
	iterNextTide = iterCurrentTide + 1;
	for ( ; iterNextTide != tides.end(); iterCurrentTide++, iterNextTide++ )
	{
		CTide currentTide = *iterCurrentTide;
		CTide nextTide    = *iterNextTide;
		//TRACE(_T("%s - %s\n"),currentTide.GetTime().Format(_T("%m/%d/%Y %H:%M")),nextTide.GetTime().Format(_T("%m/%d/%Y %H:%M")));

		// must be opposite types (High/Low)
		ASSERT(currentTide.GetType() != nextTide.GetType());

		CTime currentTime = currentTide.GetTime();
		CTime nextTime    = nextTide.GetTime();

		float currElevation = currentTide.GetElevation();
		float elevationChange = nextTide.GetElevation() - currentTide.GetElevation();

		CTime startTime = currentTime;
		int min = startTime.GetMinute();
		int r = m_TimeInterval - (min % m_TimeInterval); // number of minutes to the next interval
		r = (r == m_TimeInterval ? 0 : r); // if we have to increment a full interval, then the time is ok
		startTime += CTimeSpan(0,0,r,0);
		startTime -= CTimeSpan(0,0,0,startTime.GetSecond()); // remove the seconds

		CTimeSpan timeSpan = nextTime - currentTime;
		LONGLONG duration = timeSpan.GetTotalMinutes();
		long nIntervals = duration/m_TimeInterval + (r == 0 ? 1 : 0);

		CTime time = startTime;
		while ( time < nextTime )
		{
			TRACE(_T("Computing water elevation for %s\n"),time.Format(_T("%m/%d/%Y %H:%M")));

			CTimeSpan runningTime = time - currentTime;
			LONGLONG runningDuration = runningTime.GetTotalMinutes();

			float t = runningDuration*3.14159/duration;
			float elevation = (elevationChange - elevationChange*cos(t))/2.0 + currElevation;

			TRACE(_T("\tTime from last tide %I64d : Time between tides %I64d : Time as fraction of pi %.5f\n"),runningDuration,duration,t);
			TRACE(_T("\tLast tide elevation %.1f : elevation %.1f\n"),currElevation,elevation);
			TRACE(_T("\tWater elevation %s : %.1f\n"),time.Format(_T("%m/%d/%Y %H:%M")),elevation);

			CWaterElevation waterElevation(time,elevation);
			m_WaterElevations.Add(waterElevation);

			time += CTimeSpan(0,0,m_TimeInterval,0);
		}
	}

	m_bDirty = false;
}

CTide CTides::GetCorrectedTide(CTime time,float elevation,CTide::TideType type)
{
   // apply the corrections
   if (type == CTide::High)
   {
      if (m_HighTideTimeOffsetSign < 0)
         time -= m_HighTideTimeOffset;
      else
         time += m_HighTideTimeOffset;

      elevation *= m_HighTideElevationFactor;
   }
   else
   {
      if (m_LowTideTimeOffsetSign < 0)
         time -= m_LowTideTimeOffset;
      else
         time += m_LowTideTimeOffset;

      elevation *= m_LowTideElevationFactor;
   }

   CTide tide(time, elevation, type);

   return tide;
}