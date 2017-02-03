#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__
#include "MathGeoLib/include/MathGeoLib.h"

class Primitive {
private:
	unsigned m_buffer_id;
	float3 *m_cube_vertices;

public:
	~Primitive();
	void DrawAxis();
	void DrawCilinder();
	void DrawCube();
	void DrawPlane();
	void DrawSphere();

};

#endif // __PRIMITIVE_H__