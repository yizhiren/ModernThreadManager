#include "mtime.hpp"

const MTime mtZero;

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

