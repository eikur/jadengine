#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"

ComponentMaterial::ComponentMaterial(GameObject* parent, bool active) : Component(parent, MATERIAL, active)
{}

ComponentMaterial::~ComponentMaterial()
{}

bool ComponentMaterial::Update(float)
{
	return true;
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("Material properties");
	}
}
