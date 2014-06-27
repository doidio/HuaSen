#ifndef CAESAR_DEALTIME_H__DEF
#define CAESAR_DEALTIME_H__DEF
#include <time.h>
//this is a class based on second,that can handle time operater
class CDealTime
{
public:
	CDealTime();																//use current time
	CDealTime(time_t tm);														//use time_t time
	CDealTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);	//use specif time
	virtual ~CDealTime();

public:	//
	static CDealTime GetLocalTime();

public:								//for get value
	time_t	GetTime() const;		//gain time_t value
	int		GetYear() const;		//gain year
	int		GetMonth() const;		//gain month
	int		GetDay() const;			//gain day
	int		GetHour() const;		//gain hour
	int		GetMinute() const;		//gain minute
	int		GetSecond() const;		//gain second
	int		GetDayOfWeek() const;	//gain the current day of week(from 1 to 7)
	int		GetDayOfMonth() const;	//gain the last days of current month
	int		GetDayOfYear() const;	//gain the last days of current year(from 1.1)

public:								//for operator
	CDealTime& operator=(const CDealTime& timesrc);
	CDealTime& operator=(const time_t& timesrc);
	CDealTime  operator+(const CDealTime& tm);
	CDealTime  operator+(const time_t tm);
	CDealTime  operator-(const CDealTime& tm);
	CDealTime  operator-(const time_t tm);
	CDealTime& operator+=(const CDealTime& tm);
	CDealTime& operator+=(const time_t tm);
	CDealTime& operator-=(const CDealTime& tm);
	CDealTime& operator-=(const time_t tm);
	int		   operator==(const CDealTime& tm) const;
	int		   operator!=(const CDealTime& time ) const;
	int		   operator>(const CDealTime& time ) const;
	int		   operator>=(const CDealTime& time ) const;
	int		   operator<(const CDealTime& time ) const;
	int		   operator<=(const CDealTime& time ) const;
	
protected:
	time_t m_time;
};

#endif