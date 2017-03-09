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

public:
	Mesh();
	Mesh(aiMesh* mesh, Material* material);
	~Mesh();
	void Draw();

	void SetMaterial(Material* new_mat);


public:
	float3 *vertices = nullptr;
	int num_vertices = 0;

private:
	GLuint m_num_elements = 0;
	GLuint m_vbo[4] = { 0 };
	//GLuint m_texture_id = 0;
	Material* m_material = nullptr;
};

#endif // __MESH_H__
