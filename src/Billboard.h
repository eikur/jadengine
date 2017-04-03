#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

class ComponentCamera;

class Billboard {

	public:
		Billboard(ComponentCamera *camera = nullptr, float3 position = { 0,0,0 }, float width = 1.0f, float height = 1.0f, const char* texture = nullptr);
		~Billboard();

		void Update(); 

private:
		void ComputeQuad();
		void Draw(); 

	private:
		ComponentCamera *camera = nullptr;

		float3 position = { 0,0,0 };
		float width = 0.0f;
		float height = 0.0f;
		
		float3 up = { 0,1,0 };
		float3 right = { 0,0,0 };

		float3 *vertices = nullptr; 

		int texture_id = -1;
};

#endif