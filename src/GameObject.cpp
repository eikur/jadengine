#include "Globals.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject(const char* name, GameObject* parent, bool active) : name(name), parent(parent), active(active)
{

}
GameObject::~GameObject()
{

}

bool  GameObject::Update( float dt )
{
	bool ret = true; 
	glPushMatrix(); 
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Update(dt);
	}
	
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Update(dt);
	}
	glPopMatrix();
	return ret; 
}

bool GameObject::CleanUp()
{
	MYLOG("GameObject %s: Removing children GameObjects", name);
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}
	children.clear(); 

	MYLOG("GameObject %s: Removing components", name);
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp(); 
		RELEASE(*it);
	}
	components.clear();

	return true; 
}

void GameObject::DrawSkeleton(float3 color)
{
	glColor3f(color.x, color.y, color.z);
	glPushMatrix();
	FindComponentByType(Component::componentType::TRANSFORM)->Update();

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		float3 line_end = (*it)->GetTransformPosition();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(line_end.x, line_end.y, line_end.z);
		glEnd();
		(*it)->DrawSkeleton();
	}

	glPopMatrix();
}

//-- GUI interface

void GameObject::OnEditor()
{
	ImGui::Checkbox(name.c_str(), &active );

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnEditor();
	}
}

void GameObject::OnHierarchy( int *ptr_id, ImGuiTreeNodeFlags node_flags, long int &selection_mask, int *selected_node, GameObject *& selected_gameobject)
{
	ImGuiTreeNodeFlags flags = node_flags | ((selection_mask & (1 << *ptr_id)) ? ImGuiTreeNodeFlags_Selected : 0);
	if (children.empty() == true)
	{
		ImGui::TreeNodeEx(ptr_id, flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", name.c_str());
		if (ImGui::IsItemClicked())
		{
			*selected_node = *ptr_id;
			selected_gameobject = this; 
		}
		(*ptr_id)++;
	}
	else
	{
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)ptr_id, flags , "%s", name.c_str());
		if (ImGui::IsItemClicked())
		{
			*selected_node = *ptr_id;
			selected_gameobject = this; 
		}
		(*ptr_id)++;
		if (node_open)
		{
			for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
			{
				(*it)->OnHierarchy(ptr_id, flags, selection_mask, selected_node, selected_gameobject);
			}
			ImGui::TreePop();
		}
	}
	
}

// --- GameObject management ---
void GameObject::AddGameObjectToChildren(GameObject* game_object)
{
	children.push_back(game_object); 
}

// --- Component management ---

Component* GameObject::CreateComponent(Component::componentType type)
{
	static_assert(Component::componentType::UNKNOWN == 3, "Component class code needs update");
	Component* ret = nullptr;
	
	switch (type)
	{
	case Component::componentType::TRANSFORM: ret = new ComponentTransform(this, true); break;
	case Component::componentType::MESH: ret = new ComponentMesh(this, true);  break;
	case Component::componentType::MATERIAL: ret = new ComponentMaterial(this, true);  break;
	case Component::componentType::UNKNOWN: 
	default:
		ret = nullptr; 
		break;
	}

	if (ret != nullptr)
	{
		components.push_back(ret);
	}
	return ret;
}

Component* GameObject::FindComponentByType(Component::componentType type)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
			return *it;
	}
	return nullptr; 
}

void GameObject::SetTransform( float3 new_pos, Quat new_rot, float3 new_scale)
{
	ComponentTransform *transform = (ComponentTransform*) FindComponentByType(Component::componentType::TRANSFORM);
	if (transform == nullptr)
	{
		transform = (ComponentTransform*) CreateComponent(Component::componentType::TRANSFORM);
	}
	transform->SetTransform(new_pos, new_rot, new_scale);
}

float3 GameObject::GetTransformPosition()
{
	ComponentTransform *transform = (ComponentTransform*)FindComponentByType(Component::componentType::TRANSFORM);
	if (transform == nullptr)
	{
		return float3::zero;
	}
	return transform->position;
}