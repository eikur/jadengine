#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "Component.h"
#include "Billboard.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "assimp\include\color4.h"

class ParticleSystem : public Component
{
public:
	struct Particle
	{
		float3 position;
		float3 velocity;
		unsigned lifetime;
	};
	typedef std::vector<Billboard> BillboardList;
	typedef std::vector<Particle> ParticlePool;
	typedef std::vector<unsigned> ParticleList;
	ParticleSystem(GameObject* parent = nullptr, bool active = true);
	~ParticleSystem();

	bool Init();
	bool Update(float dt);
	bool CleanUp();
	void Draw();
	void OnEditor();

private:
	ParticlePool m_particles;
	ParticleList m_alive;
	ParticleList m_dead;
	BillboardList m_billboards;

	float2 m_emit_area;
	unsigned m_falling_time = 0;
	unsigned m_accum_elapsed = 0;
	float m_falling_height = 0.0f;
	unsigned m_texture = 0;

	float3* m_vertices = nullptr;
	float2* m_text_coords = nullptr;
	aiColor4D* m_colors = nullptr;
	unsigned* m_indices = nullptr;
};

#endif // __PARTICLE_SYSTEM_H__
