#include "Globals.h"
#include "ParticleSystem.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"
ParticleSystem::ParticleSystem(GameObject* parent, bool active)
/* unsigned max_particles, const float2& emit_size, unsigned falling_time,
float falling_height, const char* texture_file, const float2& psize) */
	: Component(parent, PARTICLES, active)
{
	/* m_emit_area = emit_size;
	m_falling_time = falling_time;
	m_falling_height = falling_height;
	m_texture = App->textures->LoadTexture(texture_file);*/
}
ParticleSystem::~ParticleSystem()
{}
bool ParticleSystem::Init()
{
	m_emit_area = { 5.0f, 3.0f };
	m_falling_height = 10.0f;
	m_falling_time = 5;
	m_particle_size = { 1.0f, 1.0f };
	m_max_particles = 50;
	// init particle pool
	m_particles.reserve(m_max_particles);
	for (size_t i = 0; i < m_max_particles; ++i) {
		Particle p;
		p.lifetime = m_falling_time;
		p.velocity.x = 0;
		p.velocity.y = (float)m_falling_time / (float)m_max_particles;
		p.velocity.z = 0;
		p.position = float3::zero; // to be set
		m_particles.push_back(p);
	}
	return true;
}
bool ParticleSystem::Update(float dt)
{
	glBegin(GL_POINTS);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();
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