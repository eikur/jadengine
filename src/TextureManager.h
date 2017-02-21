#ifndef __TEXTURE_MANAGER__
#define __TEXTURE_MANAGER__

#include <cstring>
#include <map>
#include <assimp/include/cimport.h>

class TextureManager
{
	struct LessString
	{
		bool operator()(const aiString& left, const aiString& right) const
		{
			return ::strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map<aiString, unsigned, LessString> TextureList;

	TextureList textures;
	static std::auto_ptr<TextureManager> instance;

public: 

	TextureManager();
	~TextureManager();

	unsigned Load(const aiString& file);
	void Clear();

	static  TextureManager* GetInstance();
};
#endif // !__TEXTURE_MANAGER__
