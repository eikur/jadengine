#ifndef __CUBE_PRIMITIVE_H__
#define __CUBE_PRIMITIVE_H__

#include "GameObject.h"

class CubePrimitive : public GameObject
{
public:
	CubePrimitive(const char* name, float3 position = { 0,0,0 }, float side = 1.0f, GameObject* parent = nullptr, bool active = true);
	~CubePrimitive(); 

private:
	void CreateCube( float3 position, float side); 
	
};

#endif