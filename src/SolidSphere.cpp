#include "Globals.h"
#include "SolidSphere.h"
#include "Application.h"
#include "ModuleTextures.h"



SolidSphere::SolidSphere(float radius, unsigned int rings, unsigned int sectors)
: Primitive(Primitive::Types::SOLID_SPHERE)
{
	float const R = 1.0 / (float)(rings - 1);
	float const S = 1.0 / (float)(sectors - 1);
	unsigned int r, s;

	m_vertices.resize(rings * sectors);
	m_uv_coords.resize(rings * sectors);
	std::vector<float3>::iterator v = m_vertices.begin();
	std::vector<float2>::iterator u = m_uv_coords.begin();

	double pi_2 = M_PI / 2;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = (float)sin(-pi_2 + M_PI * r * R);
			float const x = (float)cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = (float)sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			u->x = s * S;
			u->y = r * R;

			u++;

			v->x = x * radius;
			v->y = y * radius;
			v->z = z * radius;

			v++;
		}
	}

	m_indices.resize(rings * sectors * 4);
	std::vector<short>::iterator i = m_indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = (short) r * sectors + s;	
		*i++ = (short)(r + 1) * sectors + s;
		*i++ = (short) (r + 1) * sectors + (s + 1);
		*i++ = (short)r * sectors + (s + 1);
	}

	// Create buffer and load vertex data
	glGenBuffers(1, &m_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float3), &m_vertices[0], GL_STATIC_DRAW);

	// Create a buffer for UV coordinates
	glGenBuffers(1, &m_uv_coord_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_uv_coord_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, m_uv_coords.size() * sizeof(float2), &m_uv_coords[0], GL_STATIC_DRAW);

	// Create a buffer for the indices
	glGenBuffers(1, &m_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(short), &m_indices[0], GL_STATIC_DRAW);
}

SolidSphere::~SolidSphere() {}

void SolidSphere::Draw()
{
	glPushMatrix();

	glScalef(m_scale.x, m_scale.y, m_scale.z);
	glRotatef(m_rotate_angle, m_rotate_vector.x, m_rotate_vector.y, m_rotate_vector.z);
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	App->textures->UseTexture2D(m_texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_uv_coord_buffer_id);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);

	// Draw the quads
	glDrawElements(GL_QUADS, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	App->textures->DontUseTexture2D();

	glPopMatrix();
}