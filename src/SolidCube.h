#ifndef __SOLIDCUBE_H__
#define __SOLIDCUBE_H__

#include "Primitive.h"

class SolidCube : public Primitive
{
public:
	SolidCube();
	~SolidCube();

	void Draw();
private:
	unsigned m_id;
	float3 m_vertices[36];
};

#endif // __SOLIDCUBE_H__