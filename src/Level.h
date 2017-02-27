#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

struct aiScene;
struct aiNode;

class Level
{
	struct Node
	{
		std::string name;
		float3 position = float3(0, 0, 0);
		Quat rotation = Quat(1, 0, 0, 0);
		std::vector<unsigned> meshes;
		Node* parent = nullptr;
		std::vector<Node*> children;

		Node* FindNodeInChildren(const char* name);
	};

	struct Mesh
	{
		unsigned material = 0;
		float3* vertices = nullptr;
		float3* tex_coords = nullptr;
		float3* normals = nullptr;
		unsigned int num_vertices = 0;
		unsigned* indices;
		unsigned int num_indices = 0;
	};

	struct Material
	{
		float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
		float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
		float4 spectular = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float shiness = 0.0f;
		unsigned int texture = 0;
	};

private:
	const aiScene *scene = nullptr;
	Node* root = nullptr;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;

public:

	Level();
	~Level();

	bool Load(const char *path, const char* file);
	void Clear();
	void Draw();

	unsigned int GetNumMeshes() const { return meshes.size(); }
	unsigned int GetNumMaterials() const { return materials.size(); }

	Mesh& GetMesh(unsigned int index) { return meshes[index]; }
	const Mesh& GetMesh(unsigned int index) const { return meshes[index]; }

	Material& GetMaterial(unsigned index) { return materials[index]; }
	const Material& GetMaterial(unsigned index) const { return materials[index]; }

	Node* GetRootNode() { return root; }
	const Node* GetRootNode() const { return root; }

	Node* FindNode(const char* name);
	void LinkNode(Node* node, Node* new_parent);

	Node* CopyaiNode(aiNode* origin);

};

#endif /* __LEVEL_H__*/