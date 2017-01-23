#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFonts.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	MYLOG("Loading particles");
	//graphics = App->textures->Load("rtype/particles.png");
	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	MYLOG("Unloading particles");
//	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw particles 
update_status ModuleParticles::Update( float dt )
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		Particle* p = *it;

		if(p->Update( dt ) == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
		{
			App->renderer->Blit(graphics, (*it)->position.x, (*it)->position.y, &(*it)->animation->GetCurrentFrame(), 1.0f);
			++it;
		}
	}

	

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle&, int, int)
{
	// Method to create an instance of a prototype particle
}


// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	animation = new Animation();
	collider = new Collider();
}

Particle::Particle(const Particle& p)
{
	animation = new Animation(*p.animation);
	fx = p.fx;
	time_to_live = p.time_to_live;
	age = p.age;
	speed = p.speed;
	has_collider = p.has_collider;
	if (has_collider)
		collider = p.collider;
	
}

Particle::~Particle()
{
}

bool Particle::Update( float dt )
{
	// Return false if the particle must be destroyed

	//death by age
	++age;
	if(age >= time_to_live)	{
		to_delete = true;
		if (has_collider)
			collider->to_delete = true;
		return false;
	}
	//death by collision
	if (has_collider && collider->to_delete == true)
	{
		to_delete = true;
		return false;
	}
	//else
	position.x += speed;
	if (has_collider)
		collider->rect.x += speed;

	return true;
}
