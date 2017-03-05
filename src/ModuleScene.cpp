#include "Globals.h"
#include "Application.h"

#include "GameObject.h"
#include "ModuleScene.h"

ModuleScene::ModuleScene(bool active) : Module(active){}

ModuleScene::~ModuleScene() {}

bool ModuleScene::Init()
{
	// test game object creation here
	game_objects.push_back(new GameObject("First"));
	game_objects.push_back(new GameObject("Second"));
	game_objects.push_back(new GameObject("Third"));
	return true; 
}

update_status ModuleScene::Update(float dt)
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->Update(dt);
	}
	return UPDATE_CONTINUE;
}
bool ModuleScene::CleanUp()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->CleanUp(); 
	}
	game_objects.clear(); 
	return true;
}