#include "Globals.h"
#include "ModuleTimer.h"


ModuleTimer::ModuleTimer() : Module(true), m_ticks_start(0), m_ticks_pause(0), m_started(false), m_paused(false)
{}

ModuleTimer::~ModuleTimer() {}

bool ModuleTimer::Start()
{
	TimerStart();
	return true;
}

update_status ModuleTimer::Update()
{
	m_delta_time = ElapsedTime() - m_ticks_last_update;
	m_ticks_last_update = ElapsedTime();
	return UPDATE_CONTINUE;
}


void ModuleTimer::TimerStart() {
	m_started = true;
	m_paused = false;

	m_ticks_start = SDL_GetTicks();
	m_ticks_pause = 0;
}

void ModuleTimer::TimerStop() {
	m_started = false;
	m_paused = false;

	m_ticks_start = 0;
	m_ticks_pause = 0;
}

void ModuleTimer::TimerPause() {
	if (m_started && !m_paused)
	{
		m_paused = true;

		m_ticks_pause = SDL_GetTicks() - m_ticks_start;
		m_ticks_start = 0;
	}
}

void ModuleTimer::TimerResume() {
	if (m_started && m_paused)
	{
		m_paused = false;
		m_ticks_start = SDL_GetTicks() - m_ticks_pause;
		m_ticks_pause = 0;
	}
}

Uint32 ModuleTimer::ElapsedTime()
{
	Uint32 elapsed_time = 0;

	if (m_started)
	{
		if (m_paused)
			elapsed_time = m_ticks_pause;
		else
			elapsed_time = SDL_GetTicks() - m_ticks_start;
	}
	return elapsed_time;
}

Uint32 ModuleTimer::DeltaTime()
{
	return m_delta_time;
}