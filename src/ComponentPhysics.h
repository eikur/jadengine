#ifndef __COMPONENT_PHYSICS_H__
#define __COMPONENT_PHYSICS_H__

#include "Component.h"

class ComponentPhysics : public Component
{


public: 
	ComponentPhysics(GameObject* parent, bool active);
	~ComponentPhysics(); 

	void OnEditor();
};
#endif // !__COMPONENT_PHYSICS_H__
