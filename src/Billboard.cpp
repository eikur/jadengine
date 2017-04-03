#include "Application.h"
#include "ModuleTextures.h"
#include "ComponentCamera.h"

#include "Billboard.h"

Billboard::Billboard(ComponentCamera *camera, float3 position, float width, float height, const char* texture) : camera(camera), position(position), width(width), height(height) {
	vertices = new float3[4];
	if (texture != nullptr)
		texture_id = App->textures->LoadTexture(texture); 

}


Billboard::~Billboard() {
	delete vertices;
}
void Billboard::Update()
{
	ComputeQuad(); 
	Draw(); 
}


void Billboard::ComputeQuad()
{
	right = (position - camera->GetPosition()).Cross(up).Normalized();
	vertices[0] = position - up*height - right*width; 
	vertices[1] = position - up*height + right*width;
	vertices[2] = position + up*height + right*width; 
	vertices[3] = position + up*height - right*width; 
}

void Billboard::Draw()
{
	glEnable(GL_BLEND);

	App->textures->UseTexture2D(texture_id);
	
	glBegin(GL_QUADS); 
	glTexCoord2f(0, 0); 
	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
	glTexCoord2f(1, 0);
	glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	glTexCoord2f(1, 1); 
	glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
	glTexCoord2f(0, 1); 
	glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	glEnd(); 
	
	App->textures->DontUseTexture2D(); 

	glDisable(GL_BLEND);
}