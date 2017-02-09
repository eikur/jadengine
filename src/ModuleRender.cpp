#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "SolidCube.h"
#include "Grid.h"
#include "Axis.h"
#include "ModuleEditorCamera.h"


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


		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
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

			sprite = App->textures->Load(asset_file.c_str());
		}

		//Setup the viewport
		double ratio = (double)m_screen_width / (double)m_screen_height;
		glViewport(0, 0, m_screen_width, m_screen_height);

		//Orthographic projection
		/*glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-2.0 * ratio, 2.0 * ratio, -2.0, 2.0, -4.0, 4.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();*/

		//Perspective projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//gluPerspective(120.0, ratio, 1.0, 10.0);

		glLoadMatrixf(App->camera->GetProjectionMatrix().ptr());
		//	glTranslatef(0.0f, 0.0f, -2.0f);
		//	glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
		//	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// checker image
		GLubyte checkImage[64][64][4];

		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkImage[i][j][0] = (GLubyte)c;
				checkImage[i][j][1] = (GLubyte)c;
				checkImage[i][j][2] = (GLubyte)c;
				checkImage[i][j][3] = (GLubyte)255;
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &ImageName);

		glBindTexture(GL_TEXTURE_2D, ImageName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	}
	return ret;
}
bool ModuleRender::Start()
{
	bool ret = true;
	m_cube = new SolidCube();
	m_axis = new Axis();
	m_grid = new Grid();

	// aqui pasa la magia
	return ret;
}

update_status ModuleRender::PreUpdate()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	m_grid->Draw();
	m_axis->Draw();

//	m_cube->Draw();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	/*
	glBegin(GL_QUADS);

		glTexCoord2d(0.0f, 0.0f); 	glVertex3f(-0.5f, -0.5f, 0.5f);	//A
		glTexCoord2d(1.0f, 0.0f);  	glVertex3f(0.5f, -0.5f, 0.5f);	//B
		glTexCoord2d(1.0f, 1.0f);  	glVertex3f(0.5f, 0.5f, 0.5f);	//D
		glTexCoord2d(0.0f, 1.0f);  	glVertex3f(-0.5f, 0.5f, 0.5f);	//C

		glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
		glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);	//F
		glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H
		glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);	//D

		glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//C
		glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);	//D
		glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H
		glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G

		glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
		glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);	//A
		glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
		glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);	//F

		glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);	//F
		glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
		glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G
		glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H

		glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
		glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);	//A
		glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//C
		glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G
	glEnd();
	*/
	glBegin(GL_TRIANGLES);


	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);	//A
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//C

	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//c
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);	//D

	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);	//F
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H

	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);	//D

	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);	//D
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G

	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);	//D
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//C

	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);	//A
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//C

	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);	//C
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G

	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);	//A
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E		
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B


	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);	//B
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);	//F

	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);	//F
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H

	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);	//H
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	//E
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);	//G
	glEnd();



	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update( float dt )
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

	RELEASE(m_cube);
	RELEASE(m_axis);
	RELEASE(m_grid);

	return true;
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