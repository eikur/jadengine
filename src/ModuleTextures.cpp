#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/devil_cpp_wrapper.hpp"
#include "DevIL/include/IL/ilut.h"


#ifdef _MSC_VER
# pragma comment( lib, "3rdparty/DevIL/libx86/rel/DevIL.lib")
#endif

using namespace std;

ModuleTextures::ModuleTextures() : Module(true)
{

}

// Destructor
ModuleTextures::~ModuleTextures()
{

}

bool ModuleTextures::Init()
{
	MYLOG("Init Image library");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	//	ilutEnable(ILUT_OPENGL_CONV);
	return true;
}

bool ModuleTextures::CleanUp()
{
	MYLOG("Freeing textures");
	for (std::map<std::string, unsigned, LessString>::iterator it = textures.begin(); it != textures.end(); )
	{
		glDeleteTextures(1, &(*it).second);
		textures.erase(it);
		it = textures.begin();
	}
	textures.clear();
	return true;
}

GLuint ModuleTextures::LoadTexture(const std::string& texture_path)
{
	ILuint image_id;
	GLuint texture_id;
	
	std::map<std::string, unsigned, LessString>::iterator it; 
	it = textures.find(texture_path);
	if (it != textures.end())
	{
		return (*it).second; 
	}
	else
	{
		ilGenImages(1, &image_id);
		ilBindImage(image_id);

		if (ilLoadImage(texture_path.c_str()) == IL_FALSE)
		{
			MYLOG("Texture could not be loaded. Error: %s", iluErrorString(ilGetError()));
			ilDeleteImages(1, &image_id);
			return 0;
		}
		else
		{
			// bind to OpenGL
			glGenTextures(1, &texture_id);
			textures[texture_path] = texture_id;

			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}
			
			int channels = ilGetInteger(IL_IMAGE_CHANNELS);
			if (channels == 3)
			{
				ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
			}
			else if (channels == 4)
			{
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			}

			ILubyte* data = ilGetData();
			int width = ilGetInteger(IL_IMAGE_WIDTH);
			int height = ilGetInteger(IL_IMAGE_HEIGHT);

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width,
				height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data);

			ilDeleteImages(1, &image_id);
			return texture_id;
		}
	}

}

GLuint ModuleTextures::CreateCheckersTexture()
{
	GLuint texture_id;
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
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	return texture_id;
}

void ModuleTextures::UseTexture2D(GLuint texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void ModuleTextures::DontUseTexture2D()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleTextures::UnloadTexture(GLuint *texture_id)
{
	for (std::map<std::string, unsigned, LessString>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if ((*it).second == *texture_id)
		{
			glDeleteTextures(1, &(*it).second);
			textures.erase(it);
			break;
		}
	}
	
}