#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "TimerUSec.h"
#include "TimerMSec.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleParticles;
class ModuleFonts;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleFadeToBlack* fade = nullptr;
	ModuleCollision* collision = nullptr;
	ModuleParticles* particles = nullptr;
	ModuleFonts* fonts = nullptr;
	Uint32 frame_count = 0;
	Uint32 last_frame_count = 0;
	float avgFPS = 0.0f;
	float FPS = 0.0f;
private:
	TimerUSec timer;
	std::list<Module*> modules;
	TimerMSec game_timer, update_timer;
};

extern Application* App;

#endif // __APPLICATION_CPP__