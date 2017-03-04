#include "GameObject.h"
#include "Component.h"

void GameObject::Update()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Update();
	}
}

Component* GameObject::CreateComponent(componentType type)
{
	// TODO - To be fixed
	// Just to be able to compile
	return nullptr;
}