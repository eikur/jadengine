#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<list>
//#include "Globals.h"
//#include <assimp/include/types.h>
#include <string>
#include <map>


class ModuleTextures 
{
	struct LessString
	{
		bool operator()(const std::string left, const std::string right)
		{
			return ::strcmp(left.c_str(), right.c_str()) < 0;
		}
	};

	typedef std::map<std::string, unsigned, LessString> TextureList;

	TextureList textures2;
	static std::auto_ptr<ModuleTextures> instance;

public:
	ModuleTextures();
	~ModuleTextures();

	GLuint LoadTexture(std::string texture_path);
	void UnloadTexture(GLuint* texture_id);
	
	GLuint CreateCheckersTexture();

	void UseTexture2D(GLuint texture);
	void DontUseTexture2D();

private:
	std::list<GLuint*> textures;

	static ModuleTextures* GetInstance();
};

#endif // __MODULETEXTURES_H__