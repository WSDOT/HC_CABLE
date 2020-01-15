#ifndef INCLUDED_WATERELEVATION_H_
#define INCLUDED_WATERELEVATION_H_

#pragma once

class CWaterElevation
{
public:
	CWaterElevation(void);
	~CWaterElevation(void);
	CWaterElevation(CTime time,float elevation);

	CTime GetTime() const;
	float GetElevation() const;

	bool operator<(const CWaterElevation& other) const;
	bool operator==(const CWaterElevation& other) const;

private:
	CTime m_Time;
	float m_Elevation;
};

#endif // INCLUDED_WATERELEVATION_H_

