//#include "Bullet/include/btBulletDynamicsCommon.h"
#include "glew-2.0.0/include/GL/glew.h"
#include "PhysicsDebugDrawer.h"


void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	// direct method
	glBegin(GL_LINES); 
	glColor3fv(color); 
	glVertex3fv(from); 
	glVertex3fv(to); 
	glEnd(); 
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
	btScalar distance, int lifeTime, const btVector3& color)
{}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode; // test thoroughly
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mode; 
}
