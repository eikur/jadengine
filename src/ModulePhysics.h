#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include <vector>
#include "Module.h"

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class btVector3;
class btRigidBody;
class btCollisionShape; 

class ModulePhysics : public Module
{
public:
	ModulePhysics(bool active = true);
	~ModulePhysics();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	btVector3 GetGravity() const; 
	void SetGravity(const btVector3 &new_gravity); 

	btRigidBody* AddBody(float box_size); 

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collision_configuration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamics_world;

	std::vector<btCollisionShape*> shapes; 

};

#endif