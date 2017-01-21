#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	
	MYLOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		MYLOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		MYLOG("Window: Unable to load configuration from file\n");
		ret = false;
	}
	else
	{
		int width = m_screen_width * m_screen_size;
		int height = m_screen_height * m_screen_size;

		Uint32 flags = SDL_WINDOW_SHOWN;

		if(m_fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(m_window == nullptr)
		{
			MYLOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			m_screen_surface = SDL_GetWindowSurface(m_window);
		}
	}

	return ret;
}

// Called every draw update
update_status ModuleWindow::Update()
{
	char *title1 = "JADEngine - running at ";
	char fps[10];
	char *title2 = " FPS";
	char title[50];

	int ret = snprintf(fps, sizeof fps, "%.2f", App->FPS);
	if (ret < 0 || ret >= 10) {
		MYLOG("Encoding error when updating the window title!");
		return UPDATE_ERROR;
	}
	strcpy_s(title, title1);
	strcat_s(title, fps);
	strcat_s(title, title2);

	SDL_SetWindowTitle(m_window, title);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	MYLOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Read parameters from config file
bool ModuleWindow::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;

	m_screen_width = (int) json_object_dotget_number(json_object(root_value), "window.screen_width");
	m_screen_height = (int)json_object_dotget_number(json_object(root_value), "window.screen_height");
	m_screen_size = (int)json_object_dotget_number(json_object(root_value), "window.screen_size");
	m_fullscreen = (json_object_dotget_boolean(json_object(root_value), "window.fullscreen") != 0) ? true : false;
	m_vsync = (json_object_dotget_boolean(json_object(root_value), "window.vsync") != 0) ? true : false;
	if (json_object_dothas_value_of_type(json_object(root_value), "window.title", JSONString))
		m_title = json_object_dotget_string(json_object(root_value), "window.title");
	
	json_value_free(root_value);
	
	if (m_screen_width == 0 || m_screen_height == 0 || m_screen_size == 0 || m_title == "")
		return false;
	else 
		return true;
}