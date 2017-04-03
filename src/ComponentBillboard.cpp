#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "ComponentBillboard.h"

ComponentBillboard::ComponentBillboard(GameObject* parent, bool active) : Component(parent, Component::componentType::BILLBOARD, active)
{

}

ComponentBillboard::~ComponentBillboard()
{

}

bool ComponentBillboard::Update(float)
{
	float3 camera_pos = App->camera->GetCameraComponent()->frustum.Pos(); 
	float3 right = (parent->transform->position - camera_pos).Cross(float3::unitY).Normalized(); 
	parent->transform->rotation = parent->transform->rotation.LookAt(float3::unitZ, right.Cross(float3::unitY), float3::unitY, float3::unitY);

	return true; 
}

void ComponentBillboard::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard Quad"))
	{
		ImGui::Text("This component orients the billboard"); 
	}
}