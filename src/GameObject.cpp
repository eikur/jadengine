#include "Globals.h"

#include "GameObject.h"
#include "Component.h"


bool  GameObject::Update()
{
	bool ret = true; 
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		ret = (*it)->Update();
	}
	return ret; 
}

bool GameObject::CleanUp()
{
	MYLOG("GameObject %s: Removing components", name);
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp(); 
		delete *it;
	}
	components.clear();

	return true; 
}


Component* GameObject::CreateComponent(componentType type)
{
	static_assert(Component::componentType::UNKNOWN == 2, "Component class code needs update");
	Component* ret = nullptr;
	
	switch (type)
	{
	case Component::componentType::TRANSFORM: /*ret = new TransformComponent();*/ break;
	case Component::componentType::CAMERA: /*ret = new CameraComponent(); */break;
	case Component::componentType::UNKNOWN: ret = nullptr; break;
	}

	if (ret != nullptr)
	{
		components.push_back(ret);
	}
	return ret;
}

Component* GameObject::FindComponentByType(componentType type)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
			return *it;
	}
	return nullptr; 
}