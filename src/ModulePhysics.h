#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"

class ModulePhysics : public Module
{
public:
	ModulePhysics(bool active = true);
	~ModulePhysics();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();
};

#endif