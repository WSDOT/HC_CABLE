#ifndef INCLUDED_WATERELEVATIONS_H_
#define INCLUDED_WATERELEVATIONS_H_

#pragma once
#include <set>
#include "WaterElevation.h"

class CLoadWaterElevationCallback
{
public:
	virtual void OnNewWaterElevation(const CWaterElevation& waterElevation) = 0;
};

class CWaterElevations
{
public:
	CWaterElevations(void);
	~CWaterElevations(void);

	void Clear();
	long GetCount() const;
	void Add(const CWaterElevation& waterElevation);
	CWaterElevation GetWaterElevation(long index) const;
	bool GetWaterElevation(CTime time,CWaterElevation* pWaterElevation) const;

	int LoadFromFile(const CString& strFile,CLoadWaterElevationCallback* pCallback = NULL);
	int SaveWaterElevations(const CString& strFile,CTime startTime,CTime endTime);

	bool IsDST() const;
	void IsDST(bool bIsDST);

private:
	std::set<CWaterElevation> m_WaterElevations;
	bool m_bIsDST;
};

#endif // INCLUDED_WATERELEVATIONS_H_
