#include "SolidCube.h"
#include "glew-2.0.0/include/GL/glew.h"
#include "Globals.h"

SolidCube::SolidCube() 
: Primitive(Primitive::Types::SOLID_CUBE), m_vertices { // Cube front side
			{ -0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { -0.5f, 0.5f, 0.5f },
			{ 0.5f, -0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { -0.5f, 0.5f, 0.5f },
			// Cube upper side
			{ -0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { -0.5f, 0.5f, -0.5f },
			{ 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f },
			// Cube right side
			{ 0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f },
			// Cube left side
			{ -0.5f, 0.5f, 0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f },
			{ -0.5f, 0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f },
			// Cube bottom side
			{ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, 0.5f },
			{ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, 0.5f },
			// Cube back side
			{ -0.5f, 0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f },
			{ -0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }}
{
}

SolidCube::~SolidCube()
{
}

void SolidCube::Draw()
{
	//glGenBuffers(1, (GLuint*)(&m_id));
	//glBindBuffer(GL_ARRAY_BUFFER, m_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * 3, m_vertices, GL_STATIC_DRAW);
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_vertices);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/
	//glTranslatef(3.0f, 0.0f, 0.0f);
	glPushMatrix();

	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glScalef(m_scale.x, m_scale.y, m_scale.z);
	glRotatef(m_rotate_angle, m_rotate_vector.x, m_rotate_vector.y, m_rotate_vector.z);

	glBegin(GL_TRIANGLES);
	
	glVertex3f(-0.5f, -0.5f, 0.5f);	//A
	glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glVertex3f(-0.5f, 0.5f, 0.5f);	//C

	glVertex3f(-0.5f, 0.5f, 0.5f);	//c
	glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glVertex3f(0.5f, 0.5f, 0.5f);	//D

	glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glVertex3f(0.5f, -0.5f, -0.5f);	//F
	glVertex3f(0.5f, 0.5f, -0.5f);	//H

	glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glVertex3f(0.5f, 0.5f, -0.5f);	//H
	glVertex3f(0.5f, 0.5f, 0.5f);	//D

	glVertex3f(0.5f, 0.5f, 0.5f);	//D
	glVertex3f(0.5f, 0.5f, -0.5f);	//H
	glVertex3f(-0.5f, 0.5f, -0.5f);	//G

	glVertex3f(0.5f, 0.5f, 0.5f);	//D
	glVertex3f(-0.5f, 0.5f, -0.5f);	//G
	glVertex3f(-0.5f, 0.5f, 0.5f);	//C

	glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glVertex3f(-0.5f, -0.5f, 0.5f);	//A
	glVertex3f(-0.5f, 0.5f, 0.5f);	//C

	glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glVertex3f(-0.5f, 0.5f, 0.5f);	//C
	glVertex3f(-0.5f, 0.5f, -0.5f);	//G

	glVertex3f(-0.5f, -0.5f, 0.5f);	//A
	glVertex3f(-0.5f, -0.5f, -0.5f);	//E		
	glVertex3f(0.5f, -0.5f, 0.5f);	//B

	glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glVertex3f(0.5f, -0.5f, -0.5f);	//F

	glVertex3f(0.5f, -0.5f, -0.5f);	//F
	glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glVertex3f(0.5f, 0.5f, -0.5f);	//H

	glVertex3f(0.5f, 0.5f, -0.5f);	//H
	glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glVertex3f(-0.5f, 0.5f, -0.5f);	//G
	glEnd();

	glPopMatrix();
}
