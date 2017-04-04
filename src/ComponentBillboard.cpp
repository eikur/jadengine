#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"

#include "ComponentBillboard.h"

ComponentBillboard::ComponentBillboard(GameObject* parent, bool active) : Component(parent, Component::componentType::BILLBOARD, active)
{

}

ComponentBillboard::~ComponentBillboard()
{

}

bool ComponentBillboard::Update(float)
{
	elements.sort(billboardDistanceComparator());
	
	for (std::list<Billboard*>::iterator it = elements.begin(); it != elements.end(); ++it )
		(*it)->Update();

	return true; 
}

void ComponentBillboard::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard Quad"))
	{
		ImGui::Checkbox("Enabled", &active); 
		ImGui::Text("Quad elements are updated here"); 
	}
}


void ComponentBillboard::Configure(int m_rows, int n_cols, const char* texture)
{
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < n_cols; ++j)
			elements.push_back(new Billboard(App->camera->GetCameraComponent(), { (float)i,0,(float)j }, 1.0f, 1.0f, texture));
}

bool ComponentBillboard::CleanUp()
{
	for (std::list<Billboard*>::iterator it = elements.begin(); it != elements.end(); )
	{
		delete (*it);
		elements.erase(it);
		it = elements.begin();
	}
	
	elements.clear();
	return true; 
}