#include "ImGui/imgui.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics(GameObject* parent, bool active) : Component(parent, PHYSICS, active)
{
	collider = App->physics->AddBox(1);
}

ComponentPhysics::~ComponentPhysics() {}

void ComponentPhysics::OnEditor()
{
	if (ImGui::CollapsingHeader("Physics Component"))
	{
		ImGui::Text("Here will be shown the shape, and size / radius of the primitive");
		
	}

}

