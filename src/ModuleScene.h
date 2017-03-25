#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include <vector>

class GameObject;
class Level;
class Quadtree;

class ModuleScene : public Module
{

public:
	ModuleScene(bool active = true);
	~ModuleScene();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();
	GameObject* FindGameObject(const char* name, const std::vector<GameObject*>& gameobjects) const;

public:
	std::vector<GameObject*> game_objects;
private:
	Level *lvl = nullptr;
	GameObject *go = nullptr;

	Quadtree *quadtree = nullptr; 

	bool blend_animation = false; 
};

#endif