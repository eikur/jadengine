#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "ComponentBillboardQuad.h"

ComponentBillboardQuad::ComponentBillboardQuad(GameObject* parent, bool active) : Component(parent, Component::componentType::BILLBOARD_QUAD, active)
{

}

ComponentBillboardQuad::~ComponentBillboardQuad()
{

}

bool ComponentBillboardQuad::Update(float dt)
{
	ComponentCamera *camera = App->camera->GetCameraComponent(); 
	// rotate here the go

	return true; 
}

void ComponentBillboardQuad::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard Quad"))
	{
		ImGui::Text("This component orients the billboard"); 
	}
}