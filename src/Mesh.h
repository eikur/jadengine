#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

struct aiMesh;
class Material;
class GameObject;

struct Weight
{
	unsigned vertex = 0;
	float weight = 0.0f;
};

struct Bone
{
	std::string name;
	Weight* weights = nullptr;
	unsigned num_weights = 0;
	float4x4 bind;
	const GameObject *attached_to = nullptr;
};

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
	Bone* m_bones = nullptr;
	unsigned int m_num_bones = 0;
};

#endif // __MESH_H__
