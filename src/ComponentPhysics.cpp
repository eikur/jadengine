#include "ImGui/imgui.h"
#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics(GameObject* parent, bool active) : Component(parent, PHYSICS, active)
{

}

ComponentPhysics::~ComponentPhysics() {}

void ComponentPhysics::OnEditor()
{
	if (ImGui::CollapsingHeader("Physics Component"))
		ImGui::Text("Here will be shown the shape, and size / radius of the primitive"); 

}

