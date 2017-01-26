#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"


ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	MYLOG("Creating Renderer context");
	bool ret = true;

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

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update( float dt )
{
	// Draw a sprite in the middle of the screen as reference
	int w, h;
	SDL_QueryTexture(sprite, NULL, NULL, &w, &h);
	Blit(sprite, m_screen_width / 2 - w/2, m_screen_height / 2 - h/2, NULL);

	//move camera

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->renderer->camera.x += m_speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->renderer->camera.x -= m_speed * dt;

	return UPDATE_CONTINUE;
}


update_status ModuleRender::PostUpdate()
{
	//SDL_RenderPresent(renderer);
	SDL_GL_SwapWindow(App->window->m_window);
	++(App->frame_count);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	MYLOG("Destroying renderer");

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