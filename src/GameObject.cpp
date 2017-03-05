#include "Globals.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ImGui/imgui.h"

GameObject::GameObject(const char* name, bool active) : name(name), active(active)
{

}
GameObject::~GameObject()
{

}


bool  GameObject::Update( float dt )
{
	bool ret = true; 
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Update( dt );
	}
	return ret; 
}

bool GameObject::CleanUp()
{
	MYLOG("GameObject %s: Removing components", name);
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		delete *it;
	}
	components.clear();

	return true; 
}

void GameObject::OnEditor()
{
	ImGui::Checkbox(name.c_str(), &active );

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnEditor();
	}
}


// --- Component management ---

Component* GameObject::CreateComponent(Component::componentType type)
{
	static_assert(Component::componentType::UNKNOWN == 3, "Component class code needs update");
	Component* ret = nullptr;
	
	switch (type)
	{
	case Component::componentType::TRANSFORM: ret = new TransformComponent(this, true); break;
	case Component::componentType::MESH: /*ret = new CameraComponent(); */break;
	case Component::componentType::MATERIAL: /*ret = new CameraComponent(); */break;
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