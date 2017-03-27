#include "ImGui/imgui.h"
#include "ComponentBillboardQuad.h"

ComponentBillboardQuad::ComponentBillboardQuad(GameObject* parent, bool active) : Component(parent, Component::componentType::BILLBOARD_QUAD, active)
{

}

ComponentBillboardQuad::~ComponentBillboardQuad()
{

}

bool ComponentBillboardQuad::Update(float dt)
{
	return true; 
}

void ComponentBillboardQuad::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard Quad"))
	{
		ImGui::Text("This component orients the billboard"); 
	}
}