#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>
#include "parson/parson.h"
#include "glew-2.0.0\include\GL\glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Application;
class Entity;
struct Collider;

class Module
{
public:

	Module(bool active = true) : active(active)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return active;
	}

	 bool Enable()
	{
		if(active == false)
			return active = Start();

		return true;
	}

	 bool Disable()
	 {
		 if(active == true)
			 return active = !CleanUp();

		 return true;
	 }

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate( float = 0.0f)
	//virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update( float = 0.0f)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool LoadConfigFromFile(const char* )
	{
		return true;
	}
	// Callbacks ---
	virtual void HandleCollision(Collider*, Collider*)
	{}

private:
	bool active = true;
};

#endif // __MODULE_H__