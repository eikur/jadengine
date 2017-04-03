#ifndef __COMPONENT_BILLBOARD_H__
#define __COMPONENT_BILLBOARD_H__

class GameObject;
#include "Component.h"

class ComponentBillboard : public Component
{
public: 
	ComponentBillboard(GameObject* parent = nullptr, bool active = true);
	~ComponentBillboard(); 

	bool Update(float dt); 
	void OnEditor(); 

};


#endif
