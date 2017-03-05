#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include <vector>
class GameObject;

class ModuleScene : public Module
{

public:
	ModuleScene(bool active = true);
	~ModuleScene();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

public:
	std::vector<GameObject*> game_objects;

};

#endif