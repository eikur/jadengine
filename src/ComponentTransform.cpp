#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"

ComponentTransform::ComponentTransform(GameObject* parent, bool active) : Component(parent, TRANSFORM, active)
{}

ComponentTransform::~ComponentTransform()
{}

bool ComponentTransform::Update(float)
{
	glPushMatrix();
	glScalef(scale.x, scale.y, scale.z);
	float3 euler_rot = rotation.ToEulerXYZ() * 180.0f / pi;
	glRotatef(euler_rot.x, 1, 0, 0);
	glRotatef(euler_rot.y, 0, 1, 0);
	glRotatef(euler_rot.z, 0, 0, 1);
	glTranslatef(position.x, position.y, position.z); 
	glPopMatrix();
	return true; 
}

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

		position = { pos[0], pos[1], pos[2] };
		scale = { scl[0], scl[1], scl[2] };
		rotation = Quat::FromEulerXYZ(rot[0]*pi/180.0f, rot[1] * pi / 180.0f, rot[2] * pi / 180.0f);
	}
}
