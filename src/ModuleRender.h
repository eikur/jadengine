#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class SolidSphere;
class SolidCube;
class Axis;
class Grid;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update( float dt = 0.0f);
	update_status PostUpdate();
	bool CleanUp();

private:
	bool LoadConfigFromFile(const char* file_path);
	void LoadProjection();


public:
	SDL_Renderer* renderer = nullptr;
	SDL_GLContext m_glcontext;
	SDL_Rect camera;
	bool m_projection_changed = false;

private:
	SDL_Texture* sprite = nullptr;
	std::string asset_file = "";
	int m_speed = 0;

	int m_screen_height = 0;
	int m_screen_width = 0;
	bool m_vsync = true;
	
	SolidSphere *m_sphere;
	SolidCube *m_cube;
	Grid *m_grid;
	Axis *m_axis;

	GLuint ImageName = 0;

};

#endif // __MODULERENDER_H__