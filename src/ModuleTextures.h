#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include <string>
#include <map>

#include "Module.h"


class ModuleTextures : public Module
{
	struct LessString
	{
		bool operator()(const std::string left, const std::string right) const
		{
			return ::strcmp(left.c_str(), right.c_str()) < 0;
		}
	};

	typedef std::map<std::string, unsigned, LessString> TextureList;

	TextureList textures;

public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	GLuint LoadTexture(const std::string& texture_path);
	void UnloadTexture(GLuint* texture_id);
	
	GLuint CreateCheckersTexture();

	void UseTexture2D(GLuint texture);
	void DontUseTexture2D();

};

#endif // __MODULETEXTURES_H__