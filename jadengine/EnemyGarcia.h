#ifndef __ENEMYGARCIA_H__
#define __ENEMYGARCIA_H__

#include "Entity.h"

class EnemyGarcia: public Entity {

public:
	EnemyGarcia( Entity* target = nullptr);
	~EnemyGarcia();

	bool Init();
	bool Update(unsigned int msec_elapsed, const bool upd_logic = false);

private:
	void CleanUp();

	void UpdateAIDestinationPoint(AIState new_state);
	iPoint SpeedTowardsPoint(iPoint to_point) const; 
	bool InEnemyActionQueue() const;

	bool LoadFromConfigFile(const char* file_path);

private:
	Entity* target = nullptr;

};

#endif // __ENEMYGARCIA_H__