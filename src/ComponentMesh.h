#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
class GameObject; 

class ComponentMesh : public Component
{

public:
	ComponentMesh(GameObject* parent, bool active);
	~ComponentMesh();

public:
	bool Update(float dt);
	void OnEditor();
};

#endif