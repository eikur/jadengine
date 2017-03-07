#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGui/imgui.h"

class GameObject
{
public:
	bool active = true;
	std::string name = "";

private:
	GameObject* parent = nullptr; 
	std::vector<Component*> components;
	std::vector<GameObject*> children;

public: 
	GameObject(const char* name, GameObject *parent = nullptr, bool active = true);
	~GameObject();

	void Enable() { active = true; }
	void Disable() { active = false; }
	bool Update( float dt);
	bool CleanUp(); 

	void OnEditor(); 
	void OnHierarchy(int *ptr_id, ImGuiTreeNodeFlags node_flags,long int &selection_mask, int *selected_node, GameObject *& selected_gameobject);
	
	void AddGameObjectToChildren(GameObject* game_object);
	bool HasChildren(); 

	Component* CreateComponent(Component::componentType type);
	Component* FindComponentByType(Component::componentType type);

	void SetTransform(float3 new_pos, Quat new_rot, float3 new_scale);
};

#endif