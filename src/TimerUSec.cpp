#include "TimerUSec.h"

void TimerUSec::Start()
{
	m_timer_start = SDL_GetPerformanceCounter();
	m_stop = false;
}

float TimerUSec::Read()
{
	if (!m_stop)
	{
		m_timer_count = SDL_GetPerformanceCounter() - m_timer_start;
	}
	else
	{
		m_timer_count = m_timestamp - m_timer_start;
	}

	return ((float) m_timer_count)/((float) m_frequency) * 1000000;
}

void TimerUSec::Stop()
{
	if (!m_stop)
	{
		m_stop = true;
		m_timestamp = SDL_GetPerformanceCounter();
	}
}

Uint64 TimerUSec::m_frequency = SDL_GetPerformanceFrequency();

