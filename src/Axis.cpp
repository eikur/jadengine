#include "Axis.h"
#include "glew-2.0.0/include/GL/glew.h"
#include "Globals.h"

Axis::Axis()
{
}

Axis::~Axis()
{
}

void Axis::Draw()
{
	glLineWidth(3.0f);
	glBegin(GL_LINES);

	// X axis - red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	// Y axis - green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	// Z axis - blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glEnd();
	glLineWidth(1.0f);
}
