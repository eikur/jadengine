#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "Mesh.h"
class GameObject; 
class ComponentMaterial;

class ComponentMesh : public Component
{
public:
	bool shown_in_active_camera = true;

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

	bool DebugDraw();

	void SetComponentMaterial(ComponentMaterial* comp_mat);
	void LoadMesh(aiMesh *ai_mesh, Material *material); 
	void LoadVertices(float3 *vertices, unsigned num_vertices, unsigned *indices, unsigned num_indices, float2 *tex_coords, unsigned num_tex_coords); 

	void ShowBoundingBox();
	void UpdateBoundingBox( float4x4 parent_world_transform); 
	AABB GetBoundingBox() const;
};

#endif