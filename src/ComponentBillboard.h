#ifndef __COMPONENT_BILLBOARD_H__
#define __COMPONENT_BILLBOARD_H__

#include "Component.h"

class Billboard;


class ComponentBillboard : public Component
{
public: 
	ComponentBillboard(GameObject* parent = nullptr, bool active = true);
	~ComponentBillboard(); 

	void Configure(int m, int n, const char* texture); 

	bool Update(float dt); 
	void OnEditor(); 
	bool CleanUp(); 

private:
	int m = 0; 
	int n = 0; 

	Billboard ***grid = nullptr; 

};


#endif
