#ifndef __TIMERMSEC_H__
#define __TIMERMSEC_H__

#include "SDL/include/SDL.h"

class TimerMSec {

public:
	void Start(){
		m_ticks_start = SDL_GetTicks();
		m_ticks_previous = m_ticks_start;
		m_ticks_last = m_ticks_start;
		m_stop = false;
	}

	void Measure() {
		m_ticks_previous = m_ticks_last;
		if (!m_stop)
			m_ticks_last = SDL_GetTicks() - m_ticks_start;
	}

	Uint32 Read() const {
		return m_ticks_last;
	}

	Uint32 Delta() const {
		return m_ticks_last - m_ticks_previous;
	}

	Uint32 Stop() {
		if (!m_stop)
			m_stop = true;
	}

private:
	bool m_stop = true;
	Uint32 m_ticks_start = 0;
	Uint32 m_ticks_previous = 0;
	Uint32 m_ticks_last = 0;
};

#endif

