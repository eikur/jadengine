#include "Globals.h"
#include "ModulePhysics.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

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
	return true;
}

update_status ModulePhysics::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	return true;
}