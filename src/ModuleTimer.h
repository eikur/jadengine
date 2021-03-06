#ifndef __MODULETIMER_H__
#define __MODULETIMER_H__

#include "SDL/include/SDL.h"
#include "Module.h"

class ModuleTimer : public Module{

public:
	ModuleTimer();
	~ModuleTimer();

	bool Start();
	update_status Update( float dt = 0.0f);

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

	bool m_started = false;
	bool m_paused = false;
	

};

#endif // __MODULETIMER_H__
