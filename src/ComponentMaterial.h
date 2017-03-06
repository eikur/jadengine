#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
class GameObject;

class ComponentMaterial : public Component
{

public:
	ComponentMaterial(GameObject* parent, bool active);
	~ComponentMaterial();

public:
	bool Update(float dt);
	void OnEditor();
};

#endif