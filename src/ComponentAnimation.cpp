#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleAnimation.h"

#include "ImGui/imgui.h"

#include "ComponentAnimation.h"

ComponentAnimation::ComponentAnimation(GameObject* parent, bool active) : Component(parent, ANIMATION, active)
{}

ComponentAnimation::~ComponentAnimation()
{}

bool ComponentAnimation::Update(float)
{	
	if (animation_instance_id != -1)
	{
		if (App->animations->GetTransform(animation_instance_id, parent->GetName().c_str(), parent->transform->position, parent->transform->rotation) == false)
		{
			App->animations->Stop(animation_instance_id);
			if (next_animation_instance_id != -1)
				App->animations->GetTransform(animation_instance_id, parent->GetName().c_str(), parent->transform->position, parent->transform->rotation);
			animation_instance_id = next_animation_instance_id;
			next_animation_instance_id = -1; 
		}
	}

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
		ImGui::Text("Next animation Instance ID: %d", next_animation_instance_id); 
	}
}

void ComponentAnimation::SetAnimationInstanceID(int new_id)
{
	animation_instance_id = new_id;
}

void ComponentAnimation::SetNextAnimationInstanceID(int next_id)
{
	next_animation_instance_id = next_id;
}

void ComponentAnimation::GetTransform(const char* go_name, float3& position, Quat& rotation) const
{
	App->animations->GetTransform(animation_instance_id, go_name, position, rotation);
}