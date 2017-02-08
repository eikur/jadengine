#include "Primitive.h"
#include "glew-2.0.0/include/GL/glew.h"
#include "Globals.h"

Primitive::Primitive() {
	m_cube_vertices = new float3[36];
}


Primitive::~Primitive()
{
	RELEASE_ARRAY(m_cube_vertices);
}

void Primitive::DrawAxis()
{
	glLineWidth(10.0f);
	glBegin(GL_LINES);
	
	// X axis - red
	glColor3f(3.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	// Y axis - green
	glColor3f(0.0f, 3.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	// Z axis - blue
	glColor3f(0.0f, 0.0f, 3.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glEnd();
	glLineWidth(1.0f);
}

void Primitive::DrawCilinder()
{

}

void Primitive::DrawCube()
{
	// Cube front side - red
	m_cube_vertices[0] = { -0.5f, -0.5f, 0.5f };
	m_cube_vertices[1] = { 0.5f, -0.5f, 0.5f };
	m_cube_vertices[2] = { -0.5f, 0.5f, 0.5f };
	m_cube_vertices[3] = { 0.5f, -0.5f, 0.5f };
	m_cube_vertices[4] = { 0.5f, 0.5f, 0.5f };
	m_cube_vertices[5] = { -0.5f, 0.5f, 0.5f };
	// Cube upper side - green
	m_cube_vertices[6] = { -0.5f, 0.5f, 0.5f };
	m_cube_vertices[7] = { 0.5f, 0.5f, 0.5f };
	m_cube_vertices[8] = { -0.5f, 0.5f, -0.5f };
	m_cube_vertices[9] = { 0.5f, 0.5f, 0.5f };
	m_cube_vertices[10] = { 0.5f, 0.5f, -0.5f };
	m_cube_vertices[11] = { -0.5f, 0.5f, -0.5f };
	// Cube right side - blue
	m_cube_vertices[12] = { 0.5f, -0.5f, 0.5f };
	m_cube_vertices[13] = { 0.5f, -0.5f, -0.5f };
	m_cube_vertices[14] = { 0.5f, 0.5f, 0.5f };
	m_cube_vertices[15] = { 0.5f, 0.5f, 0.5f };
	m_cube_vertices[16] = { 0.5f, -0.5f, -0.5f };
	m_cube_vertices[17] = { 0.5f, 0.5f, -0.5f };
	// Cube left side - yellow
	m_cube_vertices[18] = { -0.5f, 0.5f, 0.5f };
	m_cube_vertices[19] = { -0.5f, -0.5f, -0.5f };
	m_cube_vertices[20] = { -0.5f, -0.5f, 0.5f };
	m_cube_vertices[21] = { -0.5f, 0.5f, -0.5f };
	m_cube_vertices[22] = { -0.5f, -0.5f, -0.5f };
	m_cube_vertices[23] = { -0.5f, 0.5f, 0.5f };
	// Cube bottom side - light blue
	m_cube_vertices[24] = { -0.5f, -0.5f, -0.5f };
	m_cube_vertices[25] = { 0.5f, -0.5f, 0.5f };
	m_cube_vertices[26] = { -0.5f, -0.5f, 0.5f };
	m_cube_vertices[27] = { -0.5f, -0.5f, -0.5f };
	m_cube_vertices[28] = { 0.5f, -0.5f, -0.5f };
	m_cube_vertices[29] = { 0.5f, -0.5f, 0.5f };
	// Cube back side - purple
	m_cube_vertices[30] = { -0.5f, 0.5f, -0.5f };
	m_cube_vertices[31] = { 0.5f, -0.5f, -0.5f };
	m_cube_vertices[32] = { -0.5f, -0.5f, -0.5f };
	m_cube_vertices[33] = { -0.5f, 0.5f, -0.5f };
	m_cube_vertices[34] = { 0.5f, 0.5f, -0.5f };
	m_cube_vertices[35] = { 0.5f, -0.5f, -0.5f };

	glGenBuffers(1, (GLuint*)(&m_cube_id));
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, m_cube_vertices, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void Primitive::DrawPlane()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	int max = 100;
	for (int i = -max; i <= max; ++i) {
		glVertex3f(i, 0, -max);
		glVertex3f(i, 0, max);
		glVertex3f(-max, 0, i);
		glVertex3f(max, 0, i);
	}
	glEnd();
}

void Primitive::DrawSphere()
{

}
