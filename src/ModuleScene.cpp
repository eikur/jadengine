#include "Globals.h"
#include "Application.h"

#include "Level.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleInput.h"	// for anim blending
#include "ModuleAnimation.h" // for anim blending

#include "ModuleScene.h"


ModuleScene::ModuleScene(bool active) : Module(active){}

ModuleScene::~ModuleScene() {

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		RELEASE(*it);
	game_objects.clear();
}

bool ModuleScene::Init()
{
	// test game object creation here
	lvl = new Level();

  go = lvl->Load("assets/street/", "Street.obj");
  if (go != nullptr)
	  game_objects.push_back(go);
//	go = lvl->Load("assets/Batman/", "Batman.obj");
	go = lvl->Load("assets/ArmyPilot/", "ArmyPilot.dae");
	if (go != nullptr)
		game_objects.push_back(go);
	return true; 
}

update_status ModuleScene::Update(float dt)
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->Update(dt);
	}
	go->DrawSkeleton();
	if (blend_animation == false && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		int new_anim = App->animations->BlendTo(0, "Run_Forwards", 1000);	// very improvable code
		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		{
			if ((*it)->active == true)
				(*it)->SetNextAnimationID(new_anim);
		}
	}
	return UPDATE_CONTINUE;
}
bool ModuleScene::CleanUp()
{
	RELEASE(lvl);

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->CleanUp(); 
	}

//	game_objects.clear(); 
	return true;
}