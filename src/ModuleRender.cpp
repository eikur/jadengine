#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleEditorCamera.h"
#include "Model.h"
#include "PrimitiveManager.h"
#include "Primitive.h"
#include "Level.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	MYLOG("Creating primitive manager");
	m_primitives = new PrimitiveManager();

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
		error = glGetError();
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


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat light_position[] = { 0.25f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (LoadConfigFromFile(CONFIG_FILE) == false)
		{
			MYLOG("Renderer: Unable to load configuration from file\n");
			ret = false;
		}
		else
		{
			camera.x = camera.y = 0;
			camera.w = m_screen_width;
			camera.h = m_screen_height;

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
		}

		//Perspective projection
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(App->camera->GetProjectionMatrix().ptr());

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(App->camera->GetViewMatrix().ptr());

		// Texture load
		m_checkers_texture = App->textures->CreateCheckersTexture();
		m_lenna_texture = App->textures->LoadTexture("graphics/Lenna.png");

		//lvl = new Level();
		//lvl->Load("assets/street/", "Street.obj");
		//Level::Node* node = lvl->FindNode("g Line002");

	}
	return ret;
}
bool ModuleRender::Start()
{
	bool ret = true;
	
	m_primitives->createPrimitive(Primitive::Types::AXIS);
	m_primitives->createPrimitive(Primitive::Types::GRID);
	
	m_primitives->createPrimitive(Primitive::Types::SOLID_CUBE)
		->Translate(float3(1.0, 1.0f, 1.0f))
		->Scale(float3(0.1f, 0.1f, 0.1f))
		->ApplyTexture(m_checkers_texture);
	m_primitives->createPrimitive(Primitive::Types::SOLID_CUBE)
		->Translate(float3(-1.0, 1.0f, 1.0f))
		->Scale(float3(0.1f, 0.1f, 0.1f))
		->ApplyTexture(m_checkers_texture);
	m_primitives->createPrimitive(Primitive::Types::SOLID_CUBE)
		->Translate(float3(0.0, 1.0f, -2.0f))
		->Scale(float3(2.0f, 2.0f, 2.0f))
		->Rotate(45.0f, float3(0.0f, 1.0f, 0.0f))
		->ApplyTexture(m_lenna_texture);
	m_primitives->createPrimitive(Primitive::Types::SOLID_SPHERE)
		->Translate(float3(0.0, 1.0f, 2.0f))
		->ApplyTexture(m_checkers_texture);

	m_model = new Model();
	//if (m_model->Load("Assets/Batman/", "Batman.obj") == false)
	if (m_model->Load("Assets/Magneto/", "magnetto2.fbx") == false)
		RELEASE(m_model);

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	glViewport(0, 0, App->window->m_screen_width, App->window->m_screen_height);
	if (m_projection_changed)
		LoadProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(App->camera->GetViewMatrix().ptr());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	m_primitives->DrawAllPrimitives();
	
//	if (m_model != nullptr)
//		m_model->Draw();
	//lvl->Draw();

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update( float )
{
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

	RELEASE(m_primitives);
	RELEASE(m_model);

	return true;
}

void ModuleRender::LoadProjection()
{
	//Perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->GetProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_projection_changed = false;
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
	m_vsync = (json_object_dotget_boolean(json_object(root_value), "window.vsync") != 0) ? true : false;

	m_speed = (int)json_object_dotget_number(json_object(root_value), "renderer.camera.speed");
	
	json_value_free(root_value);
	
	if (m_screen_width == 0 || m_screen_height == 0)
		return false;
	else
		return true;
}