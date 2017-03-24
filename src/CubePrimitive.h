#ifndef __CUBE_PRIMITIVE_H__
#define __CUBE_PRIMITIVE_H__

#include "GameObject.h"

class CubePrimitive : public GameObject
{
public:
	CubePrimitive(const char* name, GameObject* parent = nullptr, bool active= true);
	~CubePrimitive(); 
private:
	void CreateCubeComponents(); 


	
};
#endif