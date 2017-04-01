#include "TimerUSec.h"
#include "Globals.h"

void TimerUSec::Start()
{
	m_timer_start = SDL_GetPerformanceCounter();
	m_previous_read = m_timer_start;
	m_last_read = m_timer_start;
	m_stop = false;
}

void TimerUSec::Measure()
{
	m_previous_read = m_last_read;
	if (!m_stop)
		m_last_read = SDL_GetPerformanceCounter() - m_timer_start;
}

float TimerUSec::Read() const
{
	return (float)m_last_read * 1000000.0f / (float)m_frequency ;
}

float TimerUSec::Delta() const
{
	return ((float)m_last_read - (float)m_previous_read) * 1000000.0f / (float)m_frequency;
}

void TimerUSec::Stop()
{
	if (!m_stop)
		m_stop = true;
}

const Uint64 TimerUSec::m_frequency = SDL_GetPerformanceFrequency();
