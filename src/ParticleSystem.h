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
	ParticleSystem(GameObject* parent, bool active = true);
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
	unsigned falling_time = 0;
	unsigned accum_elapsed = 0;
	float falling_height = 0.0f;
	unsigned texture = 0;

	float3* vertices = nullptr;
	float2* text_coords = nullptr;
	aiColor4D* colors = nullptr;
	unsigned* indices = nullptr;
};

#endif // __PARTICLE_SYSTEM_H__
