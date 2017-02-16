#ifndef __PRIMITIVEMANAGER_H__
#define __PRIMITIVEMANAGER_H__

#include "Primitive.h"
#include <list>

class PrimitiveManager
{
public:
	PrimitiveManager();
	~PrimitiveManager();
	Primitive* createPrimitive(Primitive::Types primitive_type);

private:
	std::list<Primitive*> m_primitives;
};

#endif // __PRIMITIVEMANAGER_H__