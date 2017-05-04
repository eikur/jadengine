
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include "brofiler/Brofiler.h"

using namespace std;

ModuleCollision::ModuleCollision(bool start_enabled) : Module(start_enabled)
{
	
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update( float )
{
	BROFILER_CATEGORY("ModuleCollision", Profiler::Color::Green)

	for (list<Collider*>::const_iterator it = colliders.cbegin(); it != colliders.cend();++it )
	{
		for (list<Collider*>::const_iterator it2 = it; it2 != colliders.cend();++it2)	
		{
			if ((*it)->CheckCollision((*it2)->rect) == true && (*it) != (*it2))
			{
				// Do action if there is collisions
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
/*	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 0, 0, 255, 80);*/
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	MYLOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, colliderType type)
{
	Collider* ret = new Collider(rect, type);

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool collision;
	if ((r.y + r.h <= rect.y))
		collision = false;
	else if (r.y >= (rect.y + rect.h))
		collision = false;
	else if ((r.x + r.w) <= rect.x)
		collision = false;
	else if (r.x >= (rect.x + rect.w))
		collision = false;
	else
		collision = true;
	return collision;

}
