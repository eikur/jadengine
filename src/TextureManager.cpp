//#include <assimp/include/postprocess.h>
//#include <assimp/include/scene.h>

#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager() {}

unsigned int TextureManager::Load( const aiString& file )
{}

void TextureManager::Clear()
{

 }

TextureManager* TextureManager::GetInstance()
{
	return new TextureManager();
}