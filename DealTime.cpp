// DealTime.cpp: implementation of the CDealTime class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"

#include "DealTime.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDealTime::CDealTime()
{
	m_time	=	time(NULL);
}

CDealTime::CDealTime(time_t tm)
{
	m_time	=	tm;
}

CDealTime::CDealTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
	struct tm atm;
	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	assert(nDay >= 1 && nDay <= 31);
	atm.tm_mday = nDay;
	assert(nMonth >= 1 && nMonth <= 12);
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	assert(nYear >= 1900);
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;
	m_time = mktime(&atm);
	assert(m_time != -1);      
}

CDealTime::~CDealTime()
{

}

CDealTime CDealTime::GetLocalTime()
{
	return CDealTime(time(NULL));
}

time_t CDealTime::GetTime() const
{
	return m_time;
}

int CDealTime::GetYear() const
{
	return localtime(&m_time)->tm_year + 1900; //it's year since 1900
}


int CDealTime::GetMonth() const
{
	return localtime(&m_time)->tm_mon + 1;		//it's mon base on 0
}

int CDealTime::GetDay() const
{
	return localtime(&m_time)->tm_mday;
}

int CDealTime::GetHour() const
{
	return localtime(&m_time)->tm_hour;
}

int CDealTime::GetMinute() const
{
	return localtime(&m_time)->tm_min;
}

int CDealTime::GetSecond() const
{
	return localtime(&m_time)->tm_sec;
}

int	CDealTime::GetDayOfWeek() const
{
	return localtime(&m_time)->tm_wday+1;
}

int CDealTime::GetDayOfMonth() const
{
	return localtime(&m_time)->tm_mday;
}

int	CDealTime::GetDayOfYear() const
{
	return localtime(&m_time)->tm_yday+1;
}

CDealTime& CDealTime::operator=(const CDealTime& timesrc)
{
	m_time = timesrc.m_time;
	return *this;
}

CDealTime& CDealTime::operator=(const time_t& timesrc)
{
	m_time = timesrc;
	return *this;
}

CDealTime  CDealTime::operator+(const CDealTime& tm)
{
	return CDealTime(m_time + tm.m_time);
}

CDealTime  CDealTime::operator+(const time_t tm)
{
	return CDealTime(m_time + tm);
}

CDealTime  CDealTime::operator-(const CDealTime& tm)
{
	return CDealTime(m_time - tm.m_time);
}

CDealTime  CDealTime::operator-(const time_t tm)
{
	return CDealTime(m_time - tm);
}

CDealTime& CDealTime::operator+=(const CDealTime& tm)
{
	m_time += tm.m_time;
	return *this;
}

CDealTime& CDealTime::operator+=(const time_t tm)
{
	m_time += tm;
	return *this;
}

CDealTime& CDealTime::operator-=(const CDealTime& tm)
{
	m_time -= tm.m_time;
	return *this;
}

CDealTime& CDealTime::operator-=(const time_t tm)
{
	m_time -= tm;
	return *this;
}

int CDealTime::operator==(const CDealTime& tm) const
{
	return (m_time == tm.m_time);
}

int	CDealTime::operator!=(const CDealTime& time ) const
{
	return(m_time != time.m_time);
}

int CDealTime::operator>(const CDealTime& time ) const
{
	return (m_time>time.m_time);
}

int CDealTime::operator>=(const CDealTime& time ) const
{
	return (m_time>=time.m_time);
}
int CDealTime::operator<(const CDealTime& time ) const
{
	return (m_time<time.m_time);
}

int CDealTime::operator<=(const CDealTime& time ) const
{
	return (m_time<=time.m_time);
}