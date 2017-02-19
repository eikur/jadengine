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
	Primitive* Scale(float3 v) { m_scale = v; return this; }
	Primitive* Translate(float3 v) { m_pos = v; return this; }
	Primitive* Rotate(float angle, float3 v) 
	{ 
		m_rotate_angle = angle; 
		m_rotate_vector = v;
		return this; 
	}
	Primitive* ApplyTexture(const GLuint& texture_id)
	{
		m_texture_id = texture_id;
		return this;
	}

protected:
	Types m_type = UNKNOWN;
	float3 m_scale = float3::one;
	float3 m_pos = float3::zero;
	float m_rotate_angle = 0.0f;
	float3 m_rotate_vector = float3::zero;
	GLuint m_texture_id = 0;
};

#endif // __PRIMITIVE_H__