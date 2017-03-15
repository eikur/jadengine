#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGui/imgui.h"

class ComponentTransform;

class GameObject
{
public:
	bool active = true;
	std::string name = "";
	ComponentTransform *transform = nullptr;

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

	void DrawSkeleton(float3 color = { 0.0f, 0.0f, 1.0f });

	void OnEditor(); 
	void OnHierarchy(int *ptr_id, ImGuiTreeNodeFlags node_flags, int *selected_node, GameObject *& selected_gameobject);
	
	const std::string& GetName() const;

	void AddGameObjectToChildren(GameObject* game_object);

	Component* CreateComponent(Component::componentType type);

	void SetTransform(float3 new_pos, Quat new_rot, float3 new_scale);
	float3 GetTransformPosition() const;

private:
	Component* FindComponentByType(Component::componentType type) const;
};

#endif