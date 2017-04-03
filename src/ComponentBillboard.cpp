#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include "Billboard.h"

#include "ComponentBillboard.h"

ComponentBillboard::ComponentBillboard(GameObject* parent, bool active) : Component(parent, Component::componentType::BILLBOARD, active)
{

}

ComponentBillboard::~ComponentBillboard()
{

}

bool ComponentBillboard::Update(float)
{
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			grid[i][j]->Update(); 
	return true; 
}

void ComponentBillboard::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard Quad"))
	{
		ImGui::Text("m = %d", m); 
		ImGui::Text("n = %d", n); 
	}
}


void ComponentBillboard::Configure(int m_rows, int n_cols, const char* texture)
{
	m = m_rows; 
	n = n_cols; 

	grid = new Billboard**[m];
	for (int i = 0; i < m; i++)
		grid[i] = new Billboard*[n];

	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			grid[i][j] = new Billboard(App->camera->GetCameraComponent(), { (float)i,0,(float)j }, 1.0f, 1.0f, texture); 
}

bool ComponentBillboard::CleanUp()
{
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			delete grid[i][j];
	for (int i = 0; i < m; ++i)
		delete grid[i];
	delete grid;
	
	return true; 
}