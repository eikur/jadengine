#ifndef __PHYSICSDEBUGDRAWER_H__
#define __PHYSICSDEBUGDRAWER_H__
#include "Bullet/include/btBulletDynamicsCommon.h"


class PhysicsDebugDrawer : public btIDebugDraw
{
public:
	PhysicsDebugDrawer() {}
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
		btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;
public:
	DebugDrawModes mode;

};

#endif // !__PHYSICSDEBUGDRAWER_H__
