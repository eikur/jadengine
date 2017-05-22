
#include "Material.h"

Material::Material(GLuint text_id)
	: m_texture_id(text_id)
{
}

Material::~Material()
{

}

void Material::SetColor(float4 color, COLOR_COMPONENT component)
{
	m_color[component] = color;
}

void Material::SetShininess(float shininess)
{
	m_shininess = shininess;
}

const GLuint Material::GetTexture() const
{
	return m_texture_id;
}

const GLfloat* Material::GetColorComponent(COLOR_COMPONENT c) const
{
	GLfloat color[] = { m_color[c].x, m_color[c].y, m_color[c].z, m_color[c].w };
	return color;
}

const float Material::GetShininess() const
{
	return m_shininess;
}

void Material::SetShader(const char* new_shader)
{
	if (strcmp("", new_shader) != 0)
	{
		shader_name = new_shader;
		has_shader = true;
	}
	else
	{
		has_shader = false;
	}
}