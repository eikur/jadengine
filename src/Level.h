#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

class Mesh;
class Material;
class GameObject; 

class Level
{
public:
	struct Node
	{
		std::string name;
		float3 position = float3(0, 0, 0);
		Quat rotation = Quat(1, 0, 0, 0);
		std::vector<unsigned> mesh_ids;
		Node* parent = nullptr;
		std::vector<Node*> children;
	};

private:
	const aiScene *scene = nullptr;
	Node* root = nullptr;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

public:

	Level();
	~Level();

	GameObject* Load(const char *path, const char* file);
	void Clear();
	void Draw();

	unsigned int GetNumMeshes() const { return meshes.size(); }
	unsigned int GetNumMaterials() const { return materials.size(); }

	Mesh& GetMesh(unsigned int index) { return *meshes[index]; }
	const Mesh& GetMesh(unsigned int index) const { return *meshes[index]; }

	Material& GetMaterial(unsigned index) { return *materials[index]; }
	const Material& GetMaterial(unsigned index) const { return *materials[index]; }

	Node* GetRootNode() { return root; }
	const Node* GetRootNode() const { return root; }

	Node* FindNode(const char* name);
	Node* CheckNode(const char* name, Node* node);
	void LinkNode(Node* node, Node* new_parent);

	void LoadNode(const char* asset_path, const aiNode* node, Node* parent);
	void DrawNode(const Node* origin);

	GameObject* CreateGameObject(const char* path, const aiNode* origin, GameObject* parent);

};

#endif /* __LEVEL_H__*/