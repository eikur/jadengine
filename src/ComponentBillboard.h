#ifndef __COMPONENT_BILLBOARD_H__
#define __COMPONENT_BILLBOARD_H__

#include <vector>
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
	std::vector<Billboard*> elements;

};


#endif
