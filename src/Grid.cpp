#include "Grid.h"
#include "glew-2.0.0/include/GL/glew.h"
#include "Globals.h"

Grid::Grid()
: Primitive(Primitive::Types::GRID)
{
}

Grid::~Grid()
{
}

void Grid::Draw()
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
