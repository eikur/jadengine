#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__
#include "MathGeoLib/include/MathGeoLib.h"

class Primitive {
public:
	Primitive() {}
	virtual ~Primitive() {}
	virtual void Draw() {}

};

#endif // __PRIMITIVE_H__