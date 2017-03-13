#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "Material.h"

class GameObject;
struct aiMaterial;

class ComponentMaterial : public Component
{
private:
	Material *material = nullptr;

public:
	ComponentMaterial(GameObject* parent, GLuint texture_id = 0, bool active = true);
	~ComponentMaterial();

public:
	bool Update(float dt);
	bool CleanUp(); 
	void OnEditor();

	void LoadMaterial(aiMaterial *ai_mat, const char* path);

	Material* GetMaterial();
};

#endif