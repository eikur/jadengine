#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleFonts.h"

using namespace std;

Application::Application()
{
	timer.Start();
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(fonts = new ModuleFonts());

	// Modules to draw on top of game logic	
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(fade = new ModuleFadeToBlack());
	MYLOG("****** APP CONSTRUCTOR TIME: %.2f usec", timer.Read());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	timer.Start();
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not enabled
	MYLOG("****** APP INIT TIME: %.2f usec", timer.Read());
	timer.Start();
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}
	MYLOG("****** APP START TIME: %.2f usec", timer.Read());
	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	timer.Start();
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();
	MYLOG("****** APP CLEANUP TIME: %.2f usec", timer.Read());
	return ret;
}
