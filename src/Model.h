#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

struct aiScene;
class Mesh;

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
	int **indices;
	std::vector<Mesh*> m_meshes;

};

#endif /* _MODEL_H_*/