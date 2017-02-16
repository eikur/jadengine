#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

class Primitive {
public:
	enum Types
	{
		AXIS,
		GRID,
		SOLID_CUBE,
		SOLID_SPHERE,
		UNKNOWN
	};
	Primitive(Types primitive_type)
	: m_type(primitive_type) {}
	virtual ~Primitive() {}
	virtual void Draw() {}
	virtual void Scale(float3 v) 
	{ 
		m_scale = v;
	}
protected:
	Types m_type = UNKNOWN;
	float3 m_scale = float3::one;
};

#endif // __PRIMITIVE_H__