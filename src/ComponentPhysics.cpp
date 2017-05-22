#include "ImGui/imgui.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "GameObject.h"

#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics(GameObject* parent, bool active) : Component(parent, PHYSICS, active), btMotionState()
{
	collider = App->physics->AddBox(1, this);
}

ComponentPhysics::~ComponentPhysics() {}

void ComponentPhysics::OnEditor()
{
	if (ImGui::CollapsingHeader("Physics Component"))
	{
		ImGui::Text("Here will be shown the shape, and size / radius of the primitive");
		
	}

}

void ComponentPhysics::getWorldTransform(btTransform &worldTrans) const
{}



void ComponentPhysics::setWorldTransform(const btTransform &worldTrans) {
}

