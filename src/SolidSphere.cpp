#include "Globals.h"
#include "SolidSphere.h"
#include "Application.h"
#include "ModuleTextures.h"



SolidSphere::SolidSphere(float radius, unsigned int rings, unsigned int sectors)
: Primitive(Primitive::Types::SOLID_SPHERE)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	unsigned int r, s;

	vertices.resize(rings * sectors);
	std::vector<float3>::iterator v = vertices.begin();

	double pi_2 = M_PI / 2;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-pi_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			v->x = x * radius;
			v->y = y * radius;
			v->z = z * radius;

			v++;
		}
	}

	indices.resize(rings * sectors * 4);
	std::vector<short>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}

	// Create buffer and load vertex data
	glGenBuffers(1, &m_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), &vertices[0], GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	// Create a buffer for UV coordinates
/*	glGenBuffers(1, &m_uv_coord_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_uv_coord_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(GLfloat), m_uv_coord, GL_STATIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);*/

	// Create a buffer for the indices
	glGenBuffers(1, &m_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(short), &indices[0], GL_STATIC_DRAW);
}

SolidSphere::~SolidSphere() {}

void SolidSphere::Draw()
{
	glPushMatrix();

	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glScalef(m_scale.x, m_scale.y, m_scale.z);
	glRotatef(m_rotate_angle, m_rotate_vector.x, m_rotate_vector.y, m_rotate_vector.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	App->textures->UseTexture2D(m_texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);

	// Draw the triangles
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}