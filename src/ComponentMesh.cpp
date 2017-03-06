#include "ComponentMesh.h"
#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"

ComponentMesh::ComponentMesh(GameObject* parent, bool active) : Component(parent, MESH, active)
{
}

ComponentMesh::~ComponentMesh()
{}

bool ComponentMesh::Update(float)
{
	return true;
}

void ComponentMesh::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Mesh properties");
	}
}

//----- 
void ComponentMesh::LoadMesh(aiMesh *ai_mesh, Material *material)
{
	if (mesh != nullptr)
		delete mesh; 
	mesh = new Mesh(ai_mesh, material); 
}