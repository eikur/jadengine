#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGui/imgui.h"

class ComponentTransform;
class ComponentMesh;
class ComponentAnimation;

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
	bool Init();
	bool Update( float dt);
	bool CleanUp(); 

	bool DebugDraw(); 

	void DrawSkeleton(float3 color = { 0.0f, 0.0f, 1.0f }) const;

	void OnEditor(); 
	int OnHierarchy(int id, ImGuiTreeNodeFlags node_flags, GameObject *& selected_gameobject);
	
	const std::string& GetName() const;

	void AddGameObjectToChildren(GameObject* game_object);

	Component* CreateComponent(Component::componentType type);
	const std::vector<GameObject*>& GetChildren() const;

	// transform component handling methods
	float4x4 GetWorldTransformMatrix() const;
	float4x4 GetLocalTransformMatrix() const; 
	void SetLocalTransform(float3 new_pos, Quat new_rot, float3 new_scale);
	float3 GetLocalPosition() const;
	float3 GetWorldPosition() const; 

	//mesh component handling methods
	void UpdateBoundingBoxesRecursively();
	void GetAllMeshComponents(std::vector<ComponentMesh*> &meshes) const;

	//animation component handling methods
	void SetNextAnimationID(int next_id);
	
	//camera component handling methods
	void UpdateCameraWorldTransform();

private:
	Component* FindComponentByType(Component::componentType type) const;
};

#endif