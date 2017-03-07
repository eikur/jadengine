#include <assimp/include/scene.h>

#include "Application.h"
#include "ModuleTextures.h"

#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"


#include "ComponentMaterial.h"


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

void ComponentMaterial::LoadMaterial(aiMaterial *ai_mat, const char* asset_path)
{
	GLuint texture_id = 0;
	if (ai_mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString path; 
		path.Set(asset_path); 
		aiString texture_file;
		if (ai_mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture_file) == AI_SUCCESS) {
			path.Append(texture_file.C_Str()); 
			texture_id = App->textures->LoadTexture(path.C_Str());
		}
	}
	material = new Material(texture_id);
	aiColor3D color(0.0f, 0.0f, 0.0f);
	float shininess = 1.0f;
	float shine_strength = 1.0f;

	if (ai_mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
		material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::AMBIENT);
	if (ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::DIFFUSE);
	if (ai_mat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
		material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::EMISSIVE);
	if (ai_mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
		material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::SPECULAR);
	if (ai_mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color) == AI_SUCCESS)
		material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::TRANSPARENT);

	if (ai_mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
		material->SetShininess(shininess * 128.0f);

}