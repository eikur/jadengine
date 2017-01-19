#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL/include/SDL.h"

class Timer {

public:
	Timer() :m_started(false), m_ticks_start(0), m_ticks_stop(0){};
	~Timer() {};

	void Start(){
		m_ticks_start = SDL_GetTicks();
		m_ticks_stop = 0;
		m_started = true;
	}

	Uint32 Read() {
		if (m_started)
			return SDL_GetTicks() - m_ticks_start;
		else
			return m_ticks_stop - m_ticks_start;
	}

	Uint32 Stop() {
		if (m_started)
		{
			m_ticks_stop = SDL_GetTicks();
			m_started = false;
		}
		return Read();
	}


private:
	bool m_started = false;
	Uint32 m_ticks_start = 0;
	Uint32 m_ticks_stop = 0;
};

#endif

