#ifndef __TIMERUSEC_H__
#define __TIMERUSEC_H__

#include "SDL/include/SDL.h"

class TimerUSec
{
public:
	void Start();
	void Measure();
	float Read() const;
	float Delta() const;
	void Stop();

private:
	Uint64 m_timer_start = 0;
	Uint64 m_previous_read = 0;
	Uint64 m_last_read = 0;
	bool m_stop = true;
	static const Uint64 m_frequency;
};


#endif