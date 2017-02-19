#include "SolidCube.h"
#include "glew-2.0.0/include/GL/glew.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"

/*

   G__________H
   /|        /|
 A/_|______D/ |
  | |E______|_|F   
  | /       | /
  |/________|/
 B          C

*/
SolidCube::SolidCube() 
	: Primitive(Primitive::Types::SOLID_CUBE), 
	m_vertices{
	// front
	 -0.5f, 0.5f, 0.5f ,		//A
	 -0.5f, -0.5f, 0.5f ,		//B
	 0.5f, -0.5f, 0.5f ,		//C
	 0.5f, 0.5f, 0.5f ,			//D 
	// up
	 -0.5f, 0.5f, -0.5f ,		//G
	 -0.5f, 0.5f, 0.5f ,		//A
	 0.5f, 0.5f, 0.5f ,			//D 
	 0.5f, 0.5f, -0.5f ,		//H
	// back
	 0.5f, 0.5f, -0.5f ,		//H
	 0.5f, -0.5f, -0.5f ,		//F
	 -0.5f, -0.5f, -0.5f ,		//E
	 -0.5f, 0.5f, -0.5f ,		//G
	// bottom
	 0.5f, -0.5f, -0.5f ,		//F
	 0.5f, -0.5f, 0.5f ,		//C
	 -0.5f, -0.5f, 0.5f ,		//B
	 -0.5f, -0.5f, -0.5f ,		//E
	// left
	 -0.5f, 0.5f, -0.5f ,		//G
	 -0.5f, -0.5f, -0.5f ,		//E
	 -0.5f, -0.5f, 0.5f ,		//B
	 -0.5f, 0.5f, 0.5f ,		//A
	// right
	 0.5f, 0.5f, 0.5f ,			//D 
	 0.5f, -0.5f, 0.5f ,		//C
	 0.5f, -0.5f, -0.5f ,		//F
	 0.5f, 0.5f, -0.5f 			//H
	}, m_uv_coord {
	{ 0.0f, 1.0f },	{ 0.0f, 0.0f },	{ 1.0f, 0.0f },	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },	{ 0.0f, 0.0f },	{ 1.0f, 0.0f },	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },	{ 0.0f, 0.0f },	{ 1.0f, 0.0f },	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },	{ 0.0f, 0.0f },	{ 1.0f, 0.0f },	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },	{ 0.0f, 0.0f },	{ 1.0f, 0.0f },	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },	{ 0.0f, 0.0f },	{ 1.0f, 0.0f },	{ 1.0f, 1.0f },
	}, m_indices{
	0, 1, 2, 2, 3, 0,		// front
	4, 5, 6, 6, 7, 4,		// up
	8, 9, 10, 10, 11, 8,	// back
	12, 13, 14, 14, 15, 12,	// bottom
	16, 17, 18, 18, 19, 16,	// left
	20, 21, 22, 22, 23, 20	// right
	}

{
	// Create buffer and load vertex data
	glGenBuffers(1, &m_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), m_vertices, GL_STATIC_DRAW);

	// Create a buffer for UV coordinates
	glGenBuffers(1, &m_uv_coord_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_uv_coord_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(GLfloat), m_uv_coord, GL_STATIC_DRAW);

	// Create a buffer for the indices
	glGenBuffers(1, &m_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLbyte), m_indices, GL_STATIC_DRAW);
}

SolidCube::~SolidCube()
{
}

void SolidCube::Draw()
{
	glPushMatrix();

	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glScalef(m_scale.x, m_scale.y, m_scale.z);
	glRotatef(m_rotate_angle, m_rotate_vector.x, m_rotate_vector.y, m_rotate_vector.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	App->textures->UseTexture2D(m_texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, m_uv_coord_buffer_id);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);

	// Draw the triangles
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	App->textures->DontUseTexture2D();

	glPopMatrix();
}
