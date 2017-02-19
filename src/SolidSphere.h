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
	GLuint m_vao_id, m_vertex_buffer_id, m_index_buffer_id, m_uv_coord_buffer_id;
	std::vector<float3> vertices;
	std::vector<short> indices;
};


#endif
