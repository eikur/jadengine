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
class ModuleEditorCamera;
class ModuleEditorGUI;
class ModuleScene;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	bool LoadConfigFromFile(const char* file_path);

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
	ModuleEditorCamera* camera = nullptr;
	ModuleEditorGUI* gui = nullptr;
	ModuleScene* scene = nullptr; 

	Uint32 frame_count = 0;
	float FPS = 0.0f;

private:
	std::list<Module*> modules;

	TimerUSec game_timer;
	TimerMSec update_timer;
	Uint32 last_frame_count = 0;
	Uint32 fps_cap = 0;
	Uint32 msec_wait_fps_cap = 0;
	float avgFPS = 0.0f;
	float last_update_usec;
	float last_update_start;
};

extern Application* App;

#endif // __APPLICATION_CPP__