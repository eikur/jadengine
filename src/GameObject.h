#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>

class Component; 
enum componentType;

class GameObject
{
public:
	bool active = true;
	std::string name = "";

private:
	std::vector<Component*> components;

public: 
	GameObject(const char* name, bool active = true);
	~GameObject();

	void Enable() { active = true; }
	void Disable() { active = false; }
	bool Update( float dt);
	bool CleanUp(); 

	Component* CreateComponent(componentType type);
	Component* FindComponentByType(componentType type);
};

#endif