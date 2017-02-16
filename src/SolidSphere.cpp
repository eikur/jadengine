#include "Globals.h"
#include "SolidSphere.h"



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

}

SolidSphere::~SolidSphere() {}

void SolidSphere::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}