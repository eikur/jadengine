#include "Billboard.h"

Billboard::Billboard(ComponentCamera *camera, float3 position, float width, float height) : camera(camera), position(position), width(width), height(height) {}
Billboard::~Billboard() {}

void Billboard::ComputeQuad()
{
	// 0 = bottom left, 1 = bottom right , 2 = top right, 3 = top left
	// drawing= 012 023
	float3 = position - camera; 

}