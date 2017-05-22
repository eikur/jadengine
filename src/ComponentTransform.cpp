#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"
#include "GameObject.h"

#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* parent, bool active) : Component(parent, TRANSFORM, active)
{}

ComponentTransform::~ComponentTransform()
{}

bool ComponentTransform::Update(float)
{
	if ( scale.Equals(last_scale) == false || rotation.Equals(last_rotation) == false || position.Equals(last_position) == false)
	{
		parent->UpdateBoundingBoxesRecursively();
		parent->UpdateCameraWorldTransform(); 
		last_position = position; 
		last_scale = scale;
		last_rotation = rotation;
	}

	glTranslatef(position.x, position.y, position.z);
	float3 euler_rot = rotation.ToEulerXYZ() * 180.0f / pi;
	glRotatef(euler_rot.x, 1, 0, 0);
	glRotatef(euler_rot.y, 0, 1, 0);
	glRotatef(euler_rot.z, 0, 0, 1);
	glScalef(scale.x, scale.y, scale.z);
	return true; 
}

bool ComponentTransform::CleanUp()
{
	return true; 
}

void ComponentTransform::OnEditor()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		float pos[3] = { position.x, position.y, position.z };
		float scl[3] = { scale.x, scale.y, scale.z };
		float3 tmprot = rotation.ToEulerXYZ() * 180.0f / pi;
		float rot[3] = { tmprot.x, tmprot.y, tmprot.z };

		ImGui::DragFloat3("Position", pos, 0.1f); 
		ImGui::DragFloat3("Rotation", rot, 2.0f); 
		ImGui::DragFloat3("Scale", scl, 0.1f); 

		position = { pos[0], pos[1], pos[2] };
		scale = { scl[0], scl[1], scl[2] };
		rotation = Quat::FromEulerXYZ(rot[0]*pi/180.0f, rot[1] * pi / 180.0f, rot[2] * pi / 180.0f);
	}
}

void ComponentTransform::SetTransform(float3 new_pos, Quat new_rot, float3 new_scale)
{
	position = new_pos; rotation = new_rot; 
	if (new_scale.Equals({0,0,0}) == false)
		scale = new_scale;
	parent->UpdateBoundingBoxesRecursively(); 

}

float4x4 ComponentTransform::GetTransform() {
	return float4x4::FromTRS(position, rotation, scale); 

}