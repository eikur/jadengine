#ifndef __COMPONENT_PHYSICS_H__
#define __COMPONENT_PHYSICS_H__

#include "Component.h"
class btRigidBody;

class ComponentPhysics : public Component
{


public: 
	ComponentPhysics(GameObject* parent, bool active);
	~ComponentPhysics(); 

	void OnEditor();

private:
	btRigidBody* collider = nullptr;
};
#endif // !__COMPONENT_PHYSICS_H__
