#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include "GameObject.h"

class Billboard : public GameObject
{

public: 
	Billboard(const char* texture_path = nullptr, float width = 1.0f, float height = 1.0f, GameObject* parent = nullptr, bool active = true);
	~Billboard(); 

	bool DebugDraw(); 

private:
	void ComputeQuad(); 
	void Draw(); 
		

private: 
	bool debug_mode = false; 
};

#endif