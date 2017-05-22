#include "Globals.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#include "brofiler/Brofiler.h"

#ifdef _DEBUG
#	pragma comment( lib, "3rdparty/Bullet/libx86/BulletDynamics_debug.lib" )
#	pragma comment( lib, "3rdparty/Bullet/libx86/BulletCollision_debug.lib" )
#	pragma comment( lib, "3rdparty/Bullet/libx86/LinearMath_debug.lib" )
#else // RELEASE
#	pragma comment( lib, "3rdparty/Bullet/libx86/BulletDynamics.lib" )
#	pragma comment( lib, "3rdparty/Bullet/libx86/BulletCollision.lib" )
#	pragma comment( lib, "3rdparty/Bullet/libx86/LinearMath.lib" )
#endif // _MSC_VER

ModulePhysics::ModulePhysics(bool active) : Module(active) 
{
}

ModulePhysics::~ModulePhysics()
{

}

bool ModulePhysics::Init()
{
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
	dynamics_world->setGravity(btVector3(0, -10, 0));

	//get debug drawer from application
	dynamics_world->setDebugDrawer((btIDebugDraw*) App->debug_drawer);

	return true;
}

update_status ModulePhysics::PreUpdate(float dt)
{
	BROFILER_CATEGORY("ModulePhysics", Profiler::Color::Green)
	if (dt >= 0.005)
		dynamics_world->stepSimulation(dt, 15);
	dynamics_world->debugDrawWorld();
	return UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	for (std::vector<btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); )
	{
		RELEASE(*it);
		shapes.erase(it);
		it = shapes.begin();
	}

	for (std::vector<btRigidBody*>::iterator it = rigidbodies.begin(); it != rigidbodies.end();)
	{
		
		dynamics_world->removeRigidBody(*it);
		RELEASE(*it); 
		rigidbodies.erase(it);
		it = rigidbodies.begin(); 
	}

	RELEASE(dynamics_world);
	RELEASE(solver);
	RELEASE(dispatcher);
	RELEASE(collision_configuration);
	RELEASE(broadphase);

	return true;
}

btVector3 ModulePhysics::GetGravity() const
{
	return dynamics_world->getGravity(); 
}


void ModulePhysics::SetGravity(const btVector3 &new_gravity)
{
	dynamics_world->setGravity(new_gravity); 


}

btRigidBody* ModulePhysics::AddBox(float box_size, btMotionState* ms)
{
	// to improve, not working properly yet
	float mass = 1.0f; // 0.0 creates a static or immutable body. Consider passing it as a parameter
	btVector3 box_extents(box_size, box_size, box_size );
	btCollisionShape* colShape = new btBoxShape(box_extents); 
	shapes.push_back(colShape); 

	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (mass != 0.0f)
		colShape->calculateLocalInertia(mass, localInertia); 

	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ms, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo); 

	rigidbodies.push_back(body);

	dynamics_world->addRigidBody(body);
	return body; 
}