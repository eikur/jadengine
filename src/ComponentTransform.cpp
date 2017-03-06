#include "ComponentTransform.h"
#include "ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent, bool active) : Component(parent, TRANSFORM, active)
{

}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		float pos[3] = { position.x, position.y, position.z };
		float scl[3] = { scale.x, scale.y, scale.z };
		float3 tmprot = rotation.ToEulerXYZ() * 180.0f / pi;
		float rot[3] = { tmprot.x, tmprot.y, tmprot.z };
		ImGui::InputFloat3("Position", pos);
		ImGui::InputFloat3("Rotation", rot);
		ImGui::InputFloat3("Scale", scl); 

		// if changes have been made, we store them
		position = { pos[0], pos[1], pos[2] };
		scale = { scl[0], scl[1], scl[2] };
		rotation = Quat::FromEulerXYX(rot[0]*pi/180.0f, rot[1] * pi / 180.0f, rot[2] * pi / 180.0f);
	}
}
