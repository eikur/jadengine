#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<list>
#include "Module.h"
#include "Globals.h"

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	GLuint LoadTexture(const char* texture_path);
	void UnloadTexture(GLuint* texture_id);
	
	GLuint CreateCheckersTexture();

	void UseTexture2D(GLuint texture);
	void DontUseTexture2D();

private:
	std::list<GLuint*> textures;
};

#endif // __MODULETEXTURES_H__