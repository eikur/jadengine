#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"

ComponentMaterial::ComponentMaterial(GameObject* parent, GLuint texture_id, bool active) : Component(parent, MATERIAL, active)
{
	material = new Material(texture_id);
}

ComponentMaterial::~ComponentMaterial()
{}

bool ComponentMaterial::Update(float)
{
	return true;
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Checkbox("Enabled", &active);

		float ambient[4] = { material->m_color[Material::AMBIENT].x, material->m_color[Material::AMBIENT].y, material->m_color[Material::AMBIENT].z, material->m_color[Material::AMBIENT].w };
		ImGui::DragFloat4("Ambient", ambient, 0.005f, 0.0f, 1.0f);
		material->m_color[Material::AMBIENT] = { ambient[0], ambient[1], ambient[2], ambient[3] };

		ImGui::Text("More material variables coming soon");
	}
}
