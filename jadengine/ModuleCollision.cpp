
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision(Module* entities_report_to, Module* scene_cols_report_to) : entities_report_to(entities_report_to), scene_cols_report_to(scene_cols_report_to)
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

update_status ModuleCollision::Update()
{
	for (list<Collider*>::const_iterator it = colliders.cbegin(); it != colliders.cend();++it )
	{
		for (list<Collider*>::const_iterator it2 = it; it2 != colliders.cend();++it2)	
		{
			if ((*it)->CheckCollision((*it2)->rect) == true && (*it) != (*it2))
			{
				if (collision_matrix[(*it)->type][(*it2)->type] == 1)
				{
					if ((*it)->type != colliderType::SCENE_TRIGGER && (*it2)->type != colliderType::SCENE_TRIGGER )
						entities_report_to->HandleCollision((*it), (*it2));
					else
						scene_cols_report_to->HandleCollision((*it), (*it2));
					
				}
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
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		if ((*it)->type == colliderType::PLAYER_ATTACK)
			App->renderer->DrawQuad((*it)->rect, 0, 0, 255, 120);
		else if ((*it)->type == colliderType::PLAYER)
			App->renderer->DrawQuad((*it)->rect, 0, 0, 255, 80);
		else if ((*it)->type == colliderType::ENEMY)
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
		else if ((*it)->type == colliderType::ENEMY_ATTACK)
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 120);
		else if ((*it)->type == colliderType::ITEMS)
			App->renderer->DrawQuad((*it)->rect, 0, 255, 0, 80);
		else 
			App->renderer->DrawQuad((*it)->rect, 255, 0, 255, 40);

	}
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, colliderType type, Entity* parent)
{
	Collider* ret = new Collider(rect, type, parent);

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
