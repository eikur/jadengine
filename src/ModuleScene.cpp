#include "Globals.h"
#include "Application.h"

#include "Level.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleInput.h"	// for anim blending
#include "ModuleAnimation.h" // for anim blending
#include "ModuleEditorCamera.h" // for camera go
#include "Quadtree.h" // for quadtree
#include "CubePrimitive.h" // for testing quadtree
#include "ComponentBillboard.h" // for testing billboard structure

#include "brofiler/Brofiler.h"

#include "ModuleScene.h"
#include <algorithm>


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

 /* go = lvl->Load("assets/street/", "Street.obj");
  if (go != nullptr)
	  game_objects.push_back(go);*/
//	go = lvl->Load("assets/Batman/", "Batman.obj");
	go = lvl->Load("assets/ArmyPilot/", "ArmyPilot.dae");
	if (go != nullptr)
		game_objects.push_back(go);

	game_objects.push_back(App->camera->CreateCameraGameObject("Sample Camera"));

	// create quadtree
	quadtree = new Quadtree(); 
	AABB limits = AABB({ -11,-2,-11 }, { 11,2,11 });
	quadtree->Create(limits); 
	
	go = new GameObject("Billboard structure"); 
	go->CreateComponent(Component::componentType::TRANSFORM); 
	ComponentBillboard *bbcomp = (ComponentBillboard*) go->CreateComponent(Component::componentType::BILLBOARD); 
	bbcomp->Configure(4, 5, "graphics/billboardgrass.png"); 
	game_objects.push_back(go); 

	// Init Game Objects in the scene
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->Init();
	}

	return true; 
}

update_status ModuleScene::Update(float dt)
{
	BROFILER_CATEGORY("ModuleInput", Profiler::Color::Green)
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->Update(dt);
	}
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->DebugDraw();
	}
//	go->DrawSkeleton();
	if (blend_animation == false && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		int new_anim = App->animations->BlendTo(0, "Run_Forwards", 600);	// very improvable code
		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		{
			if ((*it)->active == true)
				(*it)->SetNextAnimationID(new_anim);
		}
	}
	
	// add new go to quadtree and scene
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		GameObject* cube = new CubePrimitive("cube", { float(float(rand() % 2000) / 100 - 10),0,float(rand() % 20 - 10) }, 1);
	//	cube->SetLocalTransform({float(float(rand()%2000) /100 -10),0,float(rand()%20-10)}, Quat::identity, float3::one);
		game_objects.push_back(cube);
		quadtree->Insert(cube);
	}
	quadtree->DebugDraw(); 



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
	quadtree->Clear(); 
	RELEASE(quadtree); 



	return true;
}

GameObject* ModuleScene::BoneToGameObjMapping(const char* name, const std::vector<GameObject*>& gos) const
{
	std::vector<GameObject*>::const_iterator it = std::find_if(gos.begin(), gos.end(),
		[name](const GameObject* go) { return strcmp(go->name.c_str(), name) == 0; });

	if (it != gos.end()) {
		return *it;
	}

	for (size_t i = 0; i < gos.size(); ++i) {
		GameObject* go = BoneToGameObjMapping(name, gos[i]->GetChildren());
		if (go)	return go;
	}

	return nullptr;
}

/*GameObject* ModuleScene::FindGameObject(const char *name)
{
	std::vector<GameObject*>::iterator it = std::find_if(game_objects.begin(), game_objects.end(),
		[name](const GameObject* e) { return strcmp(e->name.c_str(), name) == 0; });

	if (it != game_objects.end()) {
		return *it;
	}
	else {
		for (size_t i = 0; i < game_objects.size(); ++i)
		{
			it = std::find_if(game_objects[i]->GetChildren().begin(), game_objects[i]->GetChildren().end(),
				[name](const GameObject* e) { return strcmp(e->name.c_str(), name) == 0; });
			if (it != game_objects[i]->GetChildren().end()) {
				return *it;
			}
		}
	}

	return nullptr;
}*/