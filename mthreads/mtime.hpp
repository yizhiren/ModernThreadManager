#ifndef _M_TIME_H_
#define _M_TIME_H_

#include <sys/time.h>

#define ONE_SECOND_IN_USECS (1000 * 1000)

class  MTime  
{
public:
	MTime();
	MTime(long sec);
	MTime(long sec, long usec);
	MTime(const timeval &tv);
	MTime(double sec);
	
	void set(long sec, long usec);
	void set(const timeval &tv);
	void set(double sec);

	long getSec() const ;
	long getUsec() const ;

	void setByTotalMsec(long milliseconds);
	long getTotalInMsec() const;

	void operator += (const MTime &right);
	void operator -= (const MTime &right);

	friend  MTime operator + (const MTime &left, const MTime &right);
	friend  MTime operator - (const MTime &left, const MTime &right);
	friend  bool operator < (const MTime &left, const MTime &right);
	friend  bool operator > (const MTime &left, const MTime &right);
	friend  bool operator <= (const MTime &left, const MTime &right);
	friend  bool operator >= (const MTime &left, const MTime &right);
	friend  bool operator == (const MTime &left, const MTime &right);
	friend  bool operator != (const MTime &left, const MTime &right);

	static MTime getTimeOfDay();
	static const MTime mtZero;
	
private:
	void normalize();
	
	long m_sec;
	long m_usec;
};


// inline functions
inline MTime::MTime()
	: m_sec(0)
	, m_usec(0)
{
}

inline MTime::MTime(long sec)
	: m_sec(sec)
	, m_usec(0)
{
}

inline MTime::MTime(long sec, long usec)
{
	set(sec, usec);
}

inline MTime::MTime(const timeval &tv)
{
	set(tv);
}

inline MTime::MTime(double sec)
{
	set(sec);
}

inline void MTime::set(long sec, long usec)
{
	m_sec = sec;
	m_usec = usec;
	normalize();
}

inline void MTime::set(const timeval &tv)
{
	m_sec = tv.tv_sec;
	m_usec = tv.tv_usec;
	normalize();
}

inline void MTime::set(double sec)
{
	long l = (long)sec;
	m_sec = l;
	m_usec = (long)((sec - (double)l) * ONE_SECOND_IN_USECS);
	normalize();
}

inline void MTime::setByTotalMsec(long milliseconds)
{
	m_sec = milliseconds / 1000;
	m_usec = (milliseconds - (m_sec * 1000)) * 1000;
}

inline long MTime::getSec() const 
{
	return m_sec;
}

inline long MTime::getUsec() const 
{
	return m_usec;
}

inline long MTime::getTotalInMsec() const
{
	return m_sec * 1000 + m_usec / 1000;
}


inline MTime MTime::getTimeOfDay()
{
	timeval tvCur;
	::gettimeofday(&tvCur, NULL);
	return MTime(tvCur);
}


inline bool operator > (const MTime &left, const MTime &right)
{
	if (left.getSec() > right.getSec())
		return true;
	else if (left.getSec() == right.getSec() && left.getUsec() > right.getUsec())
		return true;
	else
		return false;
}

inline bool operator >= (const MTime &left, const MTime &right)
{
	if (left.getSec() > right.getSec())
		return true;
	else if (left.getSec() == right.getSec() && left.getUsec() >= right.getUsec())
		return true;
	else
		return false;
}

inline bool operator < (const MTime &left, const MTime &right)
{
	return !(left >= right);
}

inline bool operator <= (const MTime &left, const MTime &right)
{
	return right >= left;
}

inline bool operator == (const MTime &left, const MTime &right)
{
	return left.getSec() == right.getSec() && 
		   left.getUsec() == right.getUsec();
}

inline bool operator != (const MTime &left, const MTime &right)
{
	return !(left == right);
}

inline void MTime::operator += (const MTime &right)
{
	m_sec = getSec() + right.getSec();
	m_usec = getUsec() + right.getUsec();
	normalize();
}

inline void MTime::operator -= (const MTime &right)
{
	m_sec = getSec() - right.getSec();
	m_usec = getUsec() - right.getUsec();
	normalize();
}

inline MTime operator + 
(const MTime &left, const MTime &right)
{
	return MTime(left.getSec() + right.getSec(), 
					  left.getUsec() + right.getUsec());
}

inline MTime operator - 
(const MTime &left, const MTime &right)
{
	return MTime(left.getSec() - right.getSec(), 
					  left.getUsec() - right.getUsec());
}


void MTime::normalize()
{
	if (m_usec >= ONE_SECOND_IN_USECS) {
		do {
			m_sec++;
			m_usec -= ONE_SECOND_IN_USECS;
		}while (m_usec >= ONE_SECOND_IN_USECS);
	}
	else if (m_usec <= -ONE_SECOND_IN_USECS) {
		do {
			m_sec--;
			m_usec += ONE_SECOND_IN_USECS;
		}while (m_usec <= -ONE_SECOND_IN_USECS);
	}

	// reduce |m_sec|
	if (m_sec >= 1 && m_usec < 0) {
		m_sec--;
		m_usec += ONE_SECOND_IN_USECS;
	}
	else if (m_sec < 0 && m_usec > 0) {
		m_sec++;
		m_usec -= ONE_SECOND_IN_USECS;
	}
}


#endif
