#ifndef __COMPONENT_PHYSICS_H__
#define __COMPONENT_PHYSICS_H__

#include "Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class ComponentPhysics : public Component, public btMotionState
{

public: 
	ComponentPhysics(GameObject* parent, bool active);
	~ComponentPhysics(); 

	void OnEditor();

	// btMotionState abstract methods to be defined
	void getWorldTransform(btTransform &worldTrans) const;
	void setWorldTransform(const btTransform &worldTrans);

	//handler to modify rigidbody from editor
	void UpdateRigidbodyTransform();

private:
	btRigidBody* collider = nullptr;
};
#endif // !__COMPONENT_PHYSICS_H__
