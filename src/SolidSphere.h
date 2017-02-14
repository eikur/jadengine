#ifndef __SOLIDSPHERE_H__
#define __SOLIDSPHERE_H__

#include <vector>

#include "Primitive.h"

class SolidSphere : public Primitive
{
public:
	SolidSphere(float radius, unsigned int rings, unsigned int sectors);
	~SolidSphere();
	void Draw();

private:
	std::vector<float3> vertices;
	std::vector<short> indices;
};


#endif
