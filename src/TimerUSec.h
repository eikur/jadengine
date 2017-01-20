#ifndef __TIMERUSEC_H__
#define __TIMERUSEC_H__

#include "SDL/include/SDL.h"

class TimerUSec
{
public:
	
	void Start();
	float Read();
	void Stop();

private:
	Uint64 m_timestamp = 0;
	Uint64 m_timer_count = 0;
	Uint64 m_timer_start = 0;
	bool m_stop = true;
	static Uint64 m_frequency;
};




#endif