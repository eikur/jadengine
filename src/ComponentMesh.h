#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "Mesh.h"
class GameObject; 
class ComponentMaterial;

class ComponentMesh : public Component
{

private: 
	AABB bounding_box;
	bool show_bounding_box = false;

	Mesh *mesh = nullptr;

	ComponentMaterial *component_material = nullptr;

public:
	ComponentMesh(GameObject* parent, bool active = true);
	~ComponentMesh();

public:
	bool Update(float dt);
	bool CleanUp(); 
	void OnEditor();

	bool Debug(); 

	void SetComponentMaterial(ComponentMaterial* comp_mat);
	void LoadMesh(aiMesh *ai_mesh, Material *material); 

	void ShowBoundingBox();
	void UpdateBoundingBox( float4x4 parent_world_transform); 
};

#endif