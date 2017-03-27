#ifndef __COMPONENT_BILLBOARD_QUAD_H__
#define __COMPONENT_BILLBOARD_QUAD_H__

class GameObject;
#include "Component.h"

class ComponentBillboardQuad : public Component
{
public: 
	ComponentBillboardQuad(GameObject* parent = nullptr, bool active = true);
	~ComponentBillboardQuad(); 

	bool Update(float dt); 
	void OnEditor(); 

};


#endif
