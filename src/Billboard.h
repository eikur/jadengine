#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "ComponentCamera.h"


class Billboard {

public:
	Billboard(ComponentCamera *camera = nullptr, float3 position = { 0,0,0 }, float width = 1.0f, float height = 1.0f);
	~Billboard();

	public void ComputeQuad();

private: 
	ComponentCamera *camera = nullptr; 

	float3 position = { 0,0,0 };
	float width = 0.0f; 
	float height = 0.0f;

#endif