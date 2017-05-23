#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class Model;
class PrimitiveManager;
class Level;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate( float dt = 0.0f);
	update_status Update( float dt = 0.0f);
	update_status PostUpdate();
	bool CleanUp();

private:
	bool LoadConfigFromFile(const char* file_path);
	void LoadProjection();


public:
	SDL_Renderer* renderer = nullptr;
	SDL_GLContext m_glcontext = NULL;
	bool m_projection_changed = false;

private:
	int m_screen_height = 0;
	int m_screen_width = 0;
	bool m_vsync = true;
};

#endif // __MODULERENDER_H__