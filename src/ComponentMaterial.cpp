#include <assimp/include/scene.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ProgramManager.h"

#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"


#include "ComponentMaterial.h"


ComponentMaterial::ComponentMaterial(GameObject* parent, GLuint texture_id, bool active) : Component(parent, MATERIAL, active)
{
//	material = new Material(texture_id);
}

ComponentMaterial::~ComponentMaterial()
{

}

bool ComponentMaterial::Update(float)	
{
	return true; 
}

bool ComponentMaterial::CleanUp()
{
	RELEASE(material); 
	return true; 
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Checkbox("Enabled", &this->active);
		float ambient[4] = { material->m_color[Material::AMBIENT].x, material->m_color[Material::AMBIENT].y, material->m_color[Material::AMBIENT].z, material->m_color[Material::AMBIENT].w };
		float diffuse[4] = { material->m_color[Material::DIFFUSE].x, material->m_color[Material::DIFFUSE].y, material->m_color[Material::DIFFUSE].z, material->m_color[Material::DIFFUSE].w };
		float emissive[4] = { material->m_color[Material::EMISSIVE].x, material->m_color[Material::EMISSIVE].y, material->m_color[Material::EMISSIVE].z, material->m_color[Material::EMISSIVE].w };
		float specular[4] = { material->m_color[Material::SPECULAR].x, material->m_color[Material::SPECULAR].y, material->m_color[Material::SPECULAR].z, material->m_color[Material::SPECULAR].w };
		float transparent[4] = { material->m_color[Material::TRANSPARENT].x, material->m_color[Material::TRANSPARENT].y, material->m_color[Material::TRANSPARENT].z, material->m_color[Material::TRANSPARENT].w };
		float shininess = material->m_shininess;
		char* buffer = new char[50];
		strcpy(buffer, material->shader_name.c_str()); 
		

		ImGui::DragFloat4("Ambient", ambient, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat4("Diffuse", diffuse, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat4("Emissive", emissive, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat4("Specular", specular, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat4("Transparent", transparent, 0.005f, 0.0f, 1.0f);		
		ImGui::DragFloat("Shininess", &shininess);
		ImGui::InputText("Shader", buffer, 50);
		
		material->m_color[Material::AMBIENT] = { ambient[0], ambient[1], ambient[2], ambient[3] };
		material->m_color[Material::DIFFUSE] = { diffuse[0], diffuse[1], diffuse[2], diffuse[3] };
		material->m_color[Material::EMISSIVE] = { emissive[0], emissive[1], emissive[2], emissive[3] };
		material->m_color[Material::SPECULAR] = { specular[0], specular[1], specular[2], specular[3] };
		material->m_color[Material::TRANSPARENT] = { transparent[0], transparent[1], transparent[2], transparent[3] };
		material->m_shininess = shininess;
		material->SetShader(buffer);

		delete buffer;
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
			MYLOG("findme: texture path: %s", path.C_Str());
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

void ComponentMaterial::SetMaterial(const char* asset_path, bool has_alpha, float3 *ambient, float3* diffuse, float3* emissive, float3* specular, float3* transparent, float shininess)
{
	int texture_id = -1; 
	if (asset_path != nullptr)
		texture_id = App->textures->LoadTexture(asset_path);
	else
		texture_id = App->textures->CreateCheckersTexture(); 

	material = new Material(texture_id); 
	material->has_alpha = has_alpha;
	if (ambient != nullptr)
		material->SetColor({ ambient->x, ambient->y, ambient->z, 1.0f }, Material::COLOR_COMPONENT::AMBIENT); 
	if (diffuse != nullptr)
		material->SetColor({ diffuse->x, diffuse->y, diffuse->z, 1.0f }, Material::COLOR_COMPONENT::DIFFUSE);
}

Material* ComponentMaterial::GetMaterial()
{
	return active ? material : nullptr;
}

