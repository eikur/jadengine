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
class ModuleAnimation;
class ModulePhysics;
class PhysicsDebugDrawer;
class ProgramManager;

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
	ModuleAnimation* animations = nullptr; 
	ModulePhysics* physics = nullptr;
	PhysicsDebugDrawer *debug_drawer = nullptr;

	TimerUSec game_timer;
	TimerUSec real_timer;
	
	TimerMSec fps_refresh_timer;
	
	ProgramManager *shaders = nullptr;
	GLuint program; 
	


	Uint32 frame_count = 0;
	float FPS = 0.0f;

private:
	std::list<Module*> modules;

	Uint32 last_frame_count = 0;
	Uint32 fps_cap = 0;
	Uint32 msec_wait_fps_cap = 0;

};

extern Application* App;

#endif // __APPLICATION_CPP__