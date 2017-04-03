#ifndef __BILLBOARDGO_H__
#define __BILLBOARDGO_H__

#include "GameObject.h"

class BillboardGO : public GameObject
{

public: 
	BillboardGO(const char* texture_path = nullptr, float width = 1.0f, float height = 1.0f, GameObject* parent = nullptr, bool active = true);
	~BillboardGO(); 
	
};

#endif