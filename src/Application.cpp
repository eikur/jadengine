#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleFonts.h"
#include "ModuleEditorCamera.h"
#include "ModuleEditorGUI.h"
#include "ModuleScene.h"
#include "ModuleAnimation.h"
#include "ModuleShaders.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "DevIL/include/IL/il.h"
#include "glew-2.0.0/include/GL/glew.h"

#ifdef _MSC_VER
#	ifdef _WIN64
#		ifdef _DEBUG
#			pragma comment( lib, "3rdparty/MathGeoLib/libx64/deb/MathGeoLib.lib" )
#		else // RELEASE
#			pragma comment( lib, "3rdparty/MathGeoLib/libx64/rel/MathGeoLib.lib" )
#		endif // _DEBUG
#	else // WIN32
#		ifdef _DEBUG
#			pragma comment( lib, "3rdparty/MathGeoLib/libx86/deb/MathGeoLib.lib" )
#			pragma comment( lib, "3rdparty/DevIL/libx86/rel/DevIL.lib")
#			pragma comment( lib, "3rdparty/DevIL/libx86/rel/ILU.lib")
#			pragma comment( lib, "3rdparty/DevIL/libx86/rel/ILUT.lib")
#			pragma comment( lib, "3rdparty/glew-2.0.0/libx86/deb/glew32d.lib" )
#			pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#			pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#			pragma comment (lib, "3rdparty/assimp/lib/assimp-vc140-mt.lib")
#		else // RELEASE
#			pragma comment( lib, "3rdparty/MathGeoLib/libx86/rel/MathGeoLib.lib" )
#			pragma comment( lib, "3rdparty/DevIL/libx86/rel/DevIL.lib")
#			pragma comment( lib, "3rdparty/DevIL/libx86/rel/ILU.lib")
#			pragma comment( lib, "3rdparty/DevIL/libx86/rel/ILUT.lib")
#			pragma comment( lib, "3rdparty/glew-2.0.0/libx86/rel/glew32.lib" )
#			pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#			pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#			pragma comment (lib, "3rdparty/assimp/lib/assimp-vc140-mt.lib")
#		endif // _DEBUG	
#	endif // _WIN64
#endif // _MSC_VER

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(textures = new ModuleTextures());
	modules.push_back(camera = new ModuleEditorCamera());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(animations = new ModuleAnimation());
	modules.push_back(shaders = new ModuleShaders()); 
	modules.push_back(scene = new ModuleScene());
	modules.push_back(gui = new ModuleEditorGUI());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(fonts = new ModuleFonts());

	// Modules to draw on top of game logic	
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(fade = new ModuleFadeToBlack());

	//textures = new ModuleTextures();
	AABB limits = AABB({ -10,-2,-10 }, { 10,2,10 });
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	ret = LoadConfigFromFile(CONFIG_FILE);
	msec_wait_fps_cap = 1000 / fps_cap;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not enabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();

	real_timer.Start();
	fps_refresh_timer.Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	game_timer.Measure();
	real_timer.Measure();
	fps_refresh_timer.Measure();

	// Pre-Update all modules
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	// Update all modules considering the game timer's delta
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(game_timer.Delta() / 1000000.0f);

	// Post update all modules 
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	// Fps calculation for application stats
	if (fps_refresh_timer.Read() > 250)
	{
		FPS = 1000.0f / (float)fps_refresh_timer.Delta();
		fps_refresh_timer.Start(); 
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

//-------------------------------------------------
bool Application::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	fps_cap = (Uint32)json_object_dotget_number(root_object, "application.fps_cap");

	json_value_free(root_value);
	return true;
}