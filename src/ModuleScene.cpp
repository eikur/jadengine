#include "Globals.h"
#include "Application.h"

#include "GameObject.h"
#include "Component.h"
#include "ModuleScene.h"

ModuleScene::ModuleScene(bool active) : Module(active){}

ModuleScene::~ModuleScene() {}

bool ModuleScene::Init()
{
	// test game object creation here
	GameObject *tmp = new GameObject("First");
	tmp->CreateComponent(Component::componentType::TRANSFORM);
	game_objects.push_back(tmp);

	return true; 
}

update_status ModuleScene::Update(float dt)
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->active == true)
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