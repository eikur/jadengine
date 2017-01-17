#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL/include/SDL.h"
#include "Module.h"

class ModuleTimer : public Module{

public:
	ModuleTimer();
	~ModuleTimer();

	bool Start();
	update_status Update();

	void TimerStart();
	void TimerStop();
	void TimerPause();
	void TimerResume();

	Uint32 ElapsedTime();
	Uint32 DeltaTime();

private:
	Uint32 m_ticks_start = 0;
	Uint32 m_ticks_pause = 0;
	Uint32 m_ticks_last_update = 0;
	Uint32 m_delta_time = 0;

	bool m_started;
	bool m_paused;
	

};

#endif // __TIMER_H__
