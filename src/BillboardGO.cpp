#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "BillboardGO.h"

BillboardGO::BillboardGO(const char* texture_path, float width, float height, GameObject* parent, bool active): GameObject("Billboard", parent, active )
{
	CreateComponent(Component::componentType::TRANSFORM); 
	ComponentMesh *comp_mesh =  (ComponentMesh*) CreateComponent(Component::componentType::MESH);
	ComponentMaterial *comp_material = (ComponentMaterial*)CreateComponent(Component::componentType::MATERIAL); 
	CreateComponent(Component::componentType::BILLBOARD); 

	float3 vertices[4] = { { -width/2, -height/2, 0.0f }, { width / 2, -height / 2, 0.0f}, { width / 2, height / 2, 0.0f}, { -width / 2, height / 2, 0.0f} };
	unsigned indices[6] = { 0, 1, 2, 0, 2, 3};
	float2 tex_coords[4] = { 	{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f }	};

	comp_mesh->LoadVertices(vertices, 4, indices, 6, tex_coords, 4);
	comp_mesh->SetComponentMaterial(comp_material);
	float3 ambient = { 1.0f, 1.0f, 1.0f };
	comp_material->SetMaterial(texture_path, true, &ambient);
}

BillboardGO::~BillboardGO()
{

}
