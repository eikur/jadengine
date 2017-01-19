#include "Timer2.h"

void Timer2::Start()
{
	m_timer_start = SDL_GetPerformanceCounter();
	m_stop = false;
	m_frequency = SDL_GetPerformanceFrequency();
}

float Timer2::Read()
{
	if (!m_stop)
	{
		m_timer_count = SDL_GetPerformanceCounter() - m_timer_start;
	}
	else
	{
		m_timer_count = m_timestamp - m_timer_start;
	}

	return m_timer_count/ m_frequency * 1000000;
}

void Timer2::Stop()
{
	if (!m_stop)
	{
		m_stop = true;
		m_timestamp = SDL_GetPerformanceCounter();
	}
}

