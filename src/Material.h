#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

class Material
{
public:
	enum COLOR_COMPONENT {
		AMBIENT,
		DIFFUSE,
		EMISSIVE,
		SPECULAR,
		TRANSPARENT
	};
	Material(GLuint text_id);
	~Material();
	void SetColor(float4 color, COLOR_COMPONENT component);
	void SetShininess(float shininess);
	const GLuint GetTexture() const;
	const GLfloat* GetColorComponent(COLOR_COMPONENT c) const;
	const float GetShininess() const;

public:
	float4 m_color[5] = { float4::zero, float4::zero, float4::zero, 
		float4::zero, float4::zero };
	float m_shininess;
	GLuint m_texture_id;
};

#endif // __MATERIAL_H__