#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"

struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool active = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	bool LoadConfigFromFile(const char *file_path);

private:
	SDL_Texture* background = nullptr;
	std::string music_path = "";
	unsigned int fx_start = 0;

	uint blink_msg_msec = 0;
	uint elapsed_msec = 0;
};

#endif // __MODULESCENEINTRO_H__