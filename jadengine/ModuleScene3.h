#ifndef __MODULESCENE3_H__
#define __MODULESCENE3_H__

#include "Module.h"
#include "Point.h"

class Animation;
struct SDL_Texture;
class Player;

class ModuleScene3 : public Module 
{

public:

	ModuleScene3(bool active = true);
	~ModuleScene3();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

private:
	bool LoadConfigFromFile(const char* file_path);
	void HandleCollision(Collider* a, Collider* b);
	void CreateSceneTriggers();
	void DeleteSceneTriggers();
	void TriggerCollisionManagement(Collider *trigger);
	void PlaceSceneItems();

public:
	Player* player;

private:
	SDL_Texture* graphics = nullptr;
	
	SDL_Rect background_section;
	iPoint background_pos = { 0,0 };

	SDL_Rect middleground_section;
	iPoint middleground_pos = { 0,0 };

	SDL_Rect foreground_section;
	iPoint foreground_pos = {0, 0};

	Animation wave_sand;
	iPoint wave_sand_pos = { 0, 0 };

	Animation wave_splash;
	iPoint wave_splash_pos = { 0, 0 };

	std::string music_path = "";
	std::string boss_music_path = "";
	unsigned int fx_waves = 0;

	// scene triggers
	std::vector<Collider*> triggers;

	Collider *spawn1 = nullptr;
	Collider *spawn2 = nullptr;
	Collider *spawn3 = nullptr;
	Collider *spawn4 = nullptr;
	Collider *spawn5 = nullptr;
	Collider *spawn6 = nullptr;
	Collider *spawn7 = nullptr;
	Collider *spawn8 = nullptr;
	Collider *spawn9 = nullptr;
	Collider *spawn10 = nullptr;

	Collider *cam_lock1 = nullptr;
	Collider *cam_lock2 = nullptr;
	Collider *cam_lock3 = nullptr;
	Collider *cam_lock4 = nullptr;
	
	Collider *battle_zone1 = nullptr;
	Collider *battle_zone2 = nullptr;
	Collider *battle_zone3 = nullptr;
	Collider *battle_zone4 = nullptr;


};


#endif // __MODULESCENE3_H__
