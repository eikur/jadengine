#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "Primitive.h"


ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	MYLOG("Creating 3D renderer context");
	bool ret = true;
	m_glcontext = SDL_GL_CreateContext(App->window->m_window);

	if (m_glcontext == nullptr)
	{
		MYLOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		MYLOG("Glew library could not init %s\n", glewGetErrorString(error));
		ret = false;
	}
	else
		MYLOG("Using Glew %s", glewGetString(GLEW_VERSION));

	if (ret == true)
	{
		//get version info
		MYLOG("Vendor: %s", glGetString(GL_VENDOR));
		MYLOG("Renderer: %s", glGetString(GL_RENDERER));
		MYLOG("OpenGL version supported %s", glGetString(GL_VERSION));
		MYLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
			
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			MYLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize ModelView Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			MYLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_TEXTURE_2D);

		if (LoadConfigFromFile(CONFIG_FILE) == false)
		{
			MYLOG("Renderer: Unable to load configuration from file\n");
			ret = false;
		}
		else
		{
			camera.x = camera.y = 0;
			camera.w = m_screen_width * m_screen_size;
			camera.h = m_screen_height* m_screen_size;

			Uint32 flags = 0;
			if (m_vsync == true)
			{
				flags |= SDL_RENDERER_PRESENTVSYNC;
			}

			renderer = SDL_CreateRenderer(App->window->m_window, -1, flags);

			if (renderer == nullptr)
			{
				MYLOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				ret = false;
			}

			sprite = App->textures->Load(asset_file.c_str());
		}

		//Setup the viewport
		int width = m_screen_width * m_screen_size;
		int height = m_screen_height * m_screen_size;
		double ratio = (double)width / (double)height;
		//glViewport(0, height/2, width/2, height/2);

		//Perspective projection
		//gluPerspective(60.0, ratio, 0.0, 5.0);

		//Orthogonal projection
		glOrtho(-2.0 * ratio, 2.0 * ratio, -2.0, 2.0, -4.0, 4.0);
		//Isometric projection
		glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
		glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update( float dt )
{
	Primitive().DrawAxis();
	Primitive().DrawCube();

	return UPDATE_CONTINUE;
}


update_status ModuleRender::PostUpdate()
{
	++(App->frame_count);
	
	SDL_GL_SwapWindow(App->window->m_window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	MYLOG("Destroying renderer");

	SDL_GL_DeleteContext(m_glcontext);

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, bool flip)
{
	bool ret = true;
	SDL_Rect rect;
	SDL_RendererFlip rflip = (flip == true)? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE;

	rect.x = (int)(camera.x * speed) + x * m_screen_size;
	rect.y = (int)(camera.y * speed) + y * m_screen_size;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= m_screen_size;
	rect.h *= m_screen_size;

	if (SDL_RenderCopyEx(renderer, texture, section, &rect,0,NULL,rflip) != 0)
	{
		MYLOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * m_screen_size);
		rec.y = (int)(camera.y + rect.y * m_screen_size);
		rec.w *= m_screen_size;
		rec.h *= m_screen_size;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		MYLOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

//---------------------------------------------------------------

bool ModuleRender::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;

	//get the path to the asset
	if (json_object_dothas_value_of_type(json_object(root_value), "renderer.file", JSONString))
		asset_file = json_object_dotget_string(json_object(root_value), "renderer.file");
	if (asset_file == "") {
		json_value_free(root_value);
		return false;
	}

	m_screen_width = (int)json_object_dotget_number(json_object(root_value), "window.screen_width");
	m_screen_height = (int)json_object_dotget_number(json_object(root_value), "window.screen_height");
	m_screen_size = (int)json_object_dotget_number(json_object(root_value), "window.screen_size");
	m_vsync = (json_object_dotget_boolean(json_object(root_value), "window.vsync") != 0) ? true : false;

	m_limit_margin = (int)json_object_dotget_number(json_object(root_value), "renderer.camera.x_limit_margin");
	m_speed = (int)json_object_dotget_number(json_object(root_value), "renderer.camera.speed");
	
	json_value_free(root_value);
	
	if (m_screen_width == 0 || m_screen_height == 0 || m_screen_size == 0)
		return false;
	else
		return true;
}