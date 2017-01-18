#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleTimer.h"
#include "EntityManager.h"
#include "MathGeoLib/Geometry/AABB.h"
#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(bool active) : Module(active)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	MYLOG("Loading intro screen");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	if (App->manager->IsEnabled())
		App->manager->Disable();
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		MYLOG("Intro: crash during file load");
		return false;
	}
	App->audio->PlayMusic(music_path.c_str());
	elapsed_msec = 0;
	return true;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	MYLOG("Unloading space scene");

	App->textures->Unload(background);
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0,0,false);

	elapsed_msec += App->timer->DeltaTime();
	if (elapsed_msec <= blink_msg_msec)
		App->fonts->Print(110, 150, ModuleFonts::Fonts::hud_small, "PRESS ENTER");
	else if (elapsed_msec > 2* blink_msg_msec)
		elapsed_msec = 0;

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->scene3, this);
		App->audio->PlayFx(fx_start);
	}

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "intro.graphics_file", JSONString))
		background = App->textures->Load(json_object_dotget_string(root_object, "intro.graphics_file"));

	if (json_object_dothas_value_of_type(root_object, "intro.music_file", JSONString))
		music_path = json_object_dotget_string(root_object, "intro.music_file");
	
	if (json_object_dothas_value_of_type(root_object, "intro.fx_start", JSONString))
		fx_start = App->audio->LoadFx(json_object_dotget_string(root_object, "intro.fx_start"));

	blink_msg_msec = (int)json_object_dotget_number(root_object, "intro.blink_msg_msec");

	json_value_free(root_value);

	if (background == nullptr || music_path == "" )
		return false;
	else 
		return true;
}