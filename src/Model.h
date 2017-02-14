#ifndef __MODEL_H__
#define __MODEL_H__

struct aiScene;

class Model
{
	const aiScene *scene = nullptr;

public:

	Model();
	~Model();

	void Load(const char *file);
	void Clear();
	void Draw();
};

#endif /* _MODEL_H_*/