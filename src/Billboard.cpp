#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "Billboard.h"

Billboard::Billboard(const char* texture_path, float width, float height, GameObject* parent, bool active): GameObject("Billboard", parent, active )
{
	CreateComponent(Component::componentType::TRANSFORM); 
	ComponentMesh *comp_mesh =  (ComponentMesh*) CreateComponent(Component::componentType::MESH);
	ComponentMaterial *comp_material = (ComponentMaterial*)CreateComponent(Component::componentType::MATERIAL); 

	float3 vertices[4] = { { -width/2, -height/2, 0.0f }, { width / 2, -height / 2, 0.0f}, { width / 2, height / 2, 0.0f}, { -width / 2, height / 2, 0.0f} };
	unsigned indices[6] = { 0, 1, 2, 0, 2, 3};
	float2 tex_coords[4] = { 	{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f }	};

	comp_mesh->LoadVertices(vertices, 4, indices, 6, tex_coords, 4);
	comp_mesh->SetComponentMaterial(comp_material);
	float3 ambient = { 1.0f, 1.0f, 1.0f };
	comp_material->SetMaterial(texture_path, true, &ambient);
}

Billboard::~Billboard()
{

}

void Billboard::ComputeQuad()
{
	ComponentCamera *camera = App->camera->GetCameraComponent();


	
}

bool Billboard::DebugDraw()
{
	if (debug_mode == true)
		MYLOG("DEBUG"); 
	return true; 
}