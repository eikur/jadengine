#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

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

	void OnEditor(); 

	Component* CreateComponent(Component::componentType type);
	Component* FindComponentByType(Component::componentType type);

	void SetTranform(float3 new_pos, Quat new_rot, float3 new_scale);
};

#endif