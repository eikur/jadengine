#ifndef __CHICKEN_H__
#define __CHICKEN_H__

#include "Entity.h"

class Chicken : public Entity {

public:
	Chicken();
	~Chicken();

	bool Init();
	bool Update(unsigned int msec_elapsed, const bool upd_logic);
	void DecreaseHealth(int amount);

private:
	bool LoadFromConfigFile(const char* file_path);
};

#endif 
