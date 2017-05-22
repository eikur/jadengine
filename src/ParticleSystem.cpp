#include "Globals.h"
#include "ParticleSystem.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"

ParticleSystem::ParticleSystem(GameObject* parent, bool active) 
/*	unsigned max_particles, const float2& emit_size, unsigned falling_time,
	float falling_height, const char* texture_file, const float2& psize) */
: Component(parent, PARTICLES, active)
{
/*	m_emit_area = emit_size;
	m_falling_time = falling_time;
	m_falling_height = falling_height;
	m_texture = App->textures->LoadTexture(texture_file);*/
}

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
	RELEASE_ARRAY(m_vertices);
	RELEASE_ARRAY(m_text_coords);
	RELEASE_ARRAY(m_colors);
	RELEASE_ARRAY(m_indices);

	return true;
}

void ParticleSystem::Draw()
{

}

void ParticleSystem::OnEditor()
{
	if (ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::Text("Emit area: %u", m_falling_time);
	}
}