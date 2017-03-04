#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

struct aiMesh;
class Material;

class Mesh
{
public:
	enum BUFFERS {
		VERTEX_BUFFER,
		TEXCOORD_BUFFER,
		NORMAL_BUFFER,
		INDEX_BUFFER
	};
	Mesh(aiMesh* mesh, Material* material);
	~Mesh();
	void Draw();
private:
	GLuint m_num_elements;
	GLuint m_vbo[4] = { 0 };
	//GLuint m_texture_id = 0;
	Material* m_material;
};

#endif // __MESH_H__
