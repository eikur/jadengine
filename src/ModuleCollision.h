#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

enum colliderType {
	FIRST_TYPE = 0,
	SECOND_TYPE
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	colliderType type;

	Collider() {}
	Collider(SDL_Rect rectangle, colliderType t) :
		rect(rectangle), type(t)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision(bool start_enabled = true);
	~ModuleCollision();

	update_status PreUpdate( float dt = 0.0f);
	update_status Update( float dt = 0);

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, colliderType type);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	int collision_matrix[2][2] = { { 0, 1 },{ 1, 0 }};
	
};

#endif // __ModuleCollision_H__