#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleAnimation.h"

#include "ImGui/imgui.h"

#include "ComponentAnimation.h"

ComponentAnimation::ComponentAnimation(GameObject* parent, bool active) : Component(parent, ANIMATION, active)
{}

ComponentAnimation::~ComponentAnimation()
{}

bool ComponentAnimation::Update(float)
{	
 return true;
}

bool ComponentAnimation::CleanUp()
{
	App->animations->Stop(animation_instance_id); 
	return true;
}

void ComponentAnimation::OnEditor()
{
	if (ImGui::CollapsingHeader("Animation"))
	{
		ImGui::Text("Animation Instance ID: %d", animation_instance_id);
	}
}

void ComponentAnimation::SetAnimationInstanceID(unsigned int new_id)
{
	animation_instance_id = new_id;
	valid_animation_instance_id = true;
}

void ComponentAnimation::GetTransform(const char* go_name, float3& position, Quat& rotation) const
{
	App->animations->GetTransform(animation_instance_id, go_name, position, rotation);
}