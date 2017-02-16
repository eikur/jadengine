#include "PrimitiveManager.h"
#include "Globals.h"
#include "Axis.h"
#include "Grid.h"
#include "SolidCube.h"
#include "SolidSphere.h"

PrimitiveManager::PrimitiveManager()
{
}

PrimitiveManager::~PrimitiveManager()
{
	for (std::list<Primitive*>::iterator it = m_primitives.begin(); it != m_primitives.end(); ++it)
	{
		RELEASE(*it);
	}
}

Primitive* PrimitiveManager::createPrimitive(Primitive::Types primitive_type)
{
	static_assert(Primitive::Types::UNKNOWN == 4, "Code needs update");
	Primitive* ret = nullptr;
	switch (primitive_type) {
	case Primitive::Types::AXIS: ret = new Axis(); break;
	case Primitive::Types::GRID: ret = new Grid(); break;
	case Primitive::Types::SOLID_CUBE: ret = new SolidCube(); break;
	case Primitive::Types::SOLID_SPHERE: ret = new SolidSphere(0.5f, 10, 10); break;
	}
}