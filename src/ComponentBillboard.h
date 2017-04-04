#ifndef __COMPONENT_BILLBOARD_H__
#define __COMPONENT_BILLBOARD_H__

#include <list>
#include "Component.h"
#include "Billboard.h"


class ComponentBillboard : public Component
{
	struct billboardDistanceComparator {
		bool operator() (const Billboard* left, const Billboard* right) const {
			return left->GetDistanceToCamera() < right->GetDistanceToCamera();
		}
	};



public: 
	ComponentBillboard(GameObject* parent = nullptr, bool active = true);
	~ComponentBillboard(); 

	void Configure(int m, int n, const char* texture); 

	bool Update(float dt); 
	void OnEditor(); 
	bool CleanUp(); 

private:
	std::list<Billboard*> elements;

};


#endif
