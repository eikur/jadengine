#include "Globals.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GameObject* parent, bool active) : Component(parent, PARTICLES, active)
{}

ParticleSystem::~ParticleSystem()
{}

bool ParticleSystem::Init()
{
	return true;
}

bool ParticleSystem::Update(float dt)
{
	return true;
}

bool ParticleSystem::CleanUp()
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(text_coords);
	RELEASE_ARRAY(colors);
	RELEASE_ARRAY(indices);
}

void ParticleSystem::Draw()
{

}