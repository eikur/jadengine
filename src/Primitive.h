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
private:
	Types m_type = UNKNOWN;

};

#endif // __PRIMITIVE_H__