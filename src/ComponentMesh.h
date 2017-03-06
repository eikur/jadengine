#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "Mesh.h"
class GameObject; 

class ComponentMesh : public Component
{

private: 
	Mesh *mesh; 

public:
	ComponentMesh(GameObject* parent, bool active = true);
	~ComponentMesh();

public:
	bool Update(float dt);
	void OnEditor();

	void LoadMesh(aiMesh *ai_mesh, Material *material); 
};

#endif