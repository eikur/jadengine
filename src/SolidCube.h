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
	GLuint m_vao_id, m_vertex_buffer_id, m_index_buffer_id, m_uv_coord_buffer_id;
	GLfloat m_vertices[24*3];
	float2 m_uv_coord[24];
	GLubyte m_indices[36];
};

#endif // __SOLIDCUBE_H__