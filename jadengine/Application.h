#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleParticles;
class ModuleFonts;
class ModuleTimer;
class ModuleSceneIntro;
class ModuleScene3;
class EntityManager;

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
	ModuleTimer* timer = nullptr;

	// Game modules ---
	ModuleSceneIntro* intro = nullptr;
	ModuleScene3* scene3 = nullptr;
	EntityManager* manager = nullptr;


private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__