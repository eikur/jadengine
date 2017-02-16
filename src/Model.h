#ifndef __MODEL_H__
#define __MODEL_H__

struct aiScene;

class Model
{

public:

	Model();
	~Model();

	bool Load(const char *file);
	void Clear();
	void Draw();

private:
	const aiScene *scene = nullptr;
	int **index;


};

#endif /* _MODEL_H_*/