#include "Globals.h"
#include "ModulePhysics.h"

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