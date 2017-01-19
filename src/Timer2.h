#ifndef __TIMER2_H__
#define __TIMER2_H__

#include "SDL/include/SDL.h"

class Timer2
{
public:
	void Start();
	Uint64 Read();
	void Stop();

private:
	Uint64 m_timestamp;
	Uint64 m_timer_count;
	Uint64 m_timer_start;
	bool m_stop;
};




#endif