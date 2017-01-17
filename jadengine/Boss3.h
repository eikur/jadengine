#ifndef __BOSS3_H__
#define __BOSS3_H__

#include "Entity.h"

class Boss3 : public Entity {

public:
	Boss3(Entity* target = nullptr);
	~Boss3();

	bool Init();
	bool Update(unsigned int msec_elapsed, const bool upd_logic = false);

private:
	void CleanUp();

	void UpdateAIDestinationPoint(AIState curr_state);
	iPoint SpeedTowardsPoint(iPoint to_point) const;
	bool InEnemyActionQueue() const;

	bool LoadFromConfigFile(const char* file_path);

private:
	Entity* target = nullptr;

};

#endif __BOSS3_H__