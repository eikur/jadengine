#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

class Primitive {
public:
	Primitive() {}
	virtual ~Primitive() {}
	virtual void Draw() {}

};

#endif // __PRIMITIVE_H__