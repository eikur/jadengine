#include "Globals.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "CubePrimitive.h"

CubePrimitive::CubePrimitive(const char* name, GameObject* parent, bool active) : GameObject(name, parent, active)
{
	CreateCubeComponents();
}

CubePrimitive::~CubePrimitive()
{

}

void CubePrimitive::CreateCubeComponents()
{
	CreateComponent(Component::componentType::TRANSFORM);
	ComponentMesh *comp_mesh = (ComponentMesh*)CreateComponent(Component::componentType::MESH);
	ComponentMaterial *comp_material = (ComponentMaterial*)CreateComponent(Component::componentType::MATERIAL); 
	float3 vertices[24] = {
		// front
		{-0.5f, 0.5f, 0.5f },		//A
		{ -0.5f, -0.5f, 0.5f },		//B
		{ 0.5f, -0.5f, 0.5f },		//C
		{ 0.5f, 0.5f, 0.5f },			//D 
		// up
		{ -0.5f, 0.5f, -0.5f },		//G
		{ -0.5f, 0.5f, 0.5f },		//A
		{ 0.5f, 0.5f, 0.5f },			//D 
		{ 0.5f, 0.5f, -0.5f },		//H
		// back
		{ 0.5f, 0.5f, -0.5f },		//H
		{ 0.5f, -0.5f, -0.5f },		//F
		{ -0.5f, -0.5f, -0.5f },		//E
		{ -0.5f, 0.5f, -0.5f },		//G
		// bottom
		{ 0.5f, -0.5f, -0.5f },		//F
		{ 0.5f, -0.5f, 0.5f },		//C
		{ -0.5f, -0.5f, 0.5f },		//B
		{ -0.5f, -0.5f, -0.5f },		//E
		// left
		{ -0.5f, 0.5f, -0.5f },		//G
		{ -0.5f, -0.5f, -0.5f },		//E
		{ -0.5f, -0.5f, 0.5f },		//B
		{ -0.5f, 0.5f, 0.5f },		//A
		// right
		{ 0.5f, 0.5f, 0.5f },			//D 
		{ 0.5f, -0.5f, 0.5f },		//C
		{ 0.5f, -0.5f, -0.5f },		//F
		{ 0.5f, 0.5f, -0.5f }			//H
	};

	unsigned indices[36] = {
		0, 1, 2, 2, 3, 0,		// front
		4, 5, 6, 6, 7, 4,		// up
		8, 9, 10, 10, 11, 8,	// back
		12, 13, 14, 14, 15, 12,	// bottom
		16, 17, 18, 18, 19, 16,	// left
		20, 21, 22, 22, 23, 20	// right
	};

	//
	float2 tex_coords[24] = {
		{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f }
	};
	comp_mesh->LoadVertices(vertices, 24, indices, 36, tex_coords, 24);
	comp_mesh->SetComponentMaterial(comp_material);
	float3 ambient = { 1.0f, 0, 0 };
	comp_material->SetMaterial(nullptr,false, &ambient);

	// Add physics
	CreateComponent(Component::componentType::PHYSICS);
}