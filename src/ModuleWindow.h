#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();
	update_status Update();

	bool LoadConfigFromFile(const char* file_path);

public:
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_screen_surface = nullptr;
	int m_screen_width = 0;

private:
	
	int m_screen_height = 0;
	int m_screen_size = 0;
	bool m_fullscreen = false;
	bool m_vsync = true;
	std::string m_title = "";

};

#endif // __MODULEWINDOW_H__