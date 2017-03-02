#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>

class Component; 
enum componentType;

class GameObject
{
	void Update();
	Component* CreateComponent(componentType type);

private:
	bool active = true; 
	std::string name = "";
	std::vector<Component*> components;
};

#endif