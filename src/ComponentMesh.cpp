#include "ComponentMesh.h"
#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"

ComponentMesh::ComponentMesh(GameObject* parent, bool active) : Component(parent, MESH, active)
{
}

ComponentMesh::~ComponentMesh()
{
	
}

bool ComponentMesh::Update(float)
{
	mesh->Draw(); 
	if (show_bounding_box)
		ShowBoundingBox();
	return true;
}

void ComponentMesh::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Checkbox("Enabled", &this->active);
		ImGui::SameLine();
		ImGui::Checkbox("Debug", &show_bounding_box);
		float bx[2] = { (float)bounding_box.MinX(), (float)bounding_box.MaxX() };
		float by[2] = { bounding_box.MinY(), bounding_box.MaxY() };
		float bz[2] = { bounding_box.MinZ(), bounding_box.MaxZ() };
		ImGui::DragFloat2("AABB.x", bx);
		ImGui::DragFloat2("AABB.y", by);
		ImGui::DragFloat2("AABB.z", bz);
	}
}

//----- 
void ComponentMesh::LoadMesh(aiMesh *ai_mesh, Material *material)
{
	if (mesh == nullptr)
	{
		mesh = new Mesh(ai_mesh, material);
		bounding_box.SetNegativeInfinity();
		bounding_box.Enclose((float3*)mesh->vertices, mesh->num_vertices);
	}
}

void ComponentMesh::ShowBoundingBox()
{
	glColor3f(0.0f, 1.0f, 0.0f); 
	glBegin(GL_LINES);
	for (int i = 0; i < 8; i++)
		glVertex3f(bounding_box.CornerPoint(i).x, bounding_box.CornerPoint(i).y, bounding_box.CornerPoint(i).z);
	for (int i = 0; i < 4; i++)
	{
		glVertex3f(bounding_box.CornerPoint(i).x, bounding_box.CornerPoint(i).y, bounding_box.CornerPoint(i).z);
		glVertex3f(bounding_box.CornerPoint(i+4).x, bounding_box.CornerPoint(i+4).y, bounding_box.CornerPoint(i+4).z);
	}
	for (int i = 0; i < 2; i++)
	{
		glVertex3f(bounding_box.CornerPoint(i).x, bounding_box.CornerPoint(i).y, bounding_box.CornerPoint(i).z);
		glVertex3f(bounding_box.CornerPoint(i + 2).x, bounding_box.CornerPoint(i + 2).y, bounding_box.CornerPoint(i + 2).z);
	}
	for (int i = 4; i < 6; i++)
	{
		glVertex3f(bounding_box.CornerPoint(i).x, bounding_box.CornerPoint(i).y, bounding_box.CornerPoint(i).z);
		glVertex3f(bounding_box.CornerPoint(i + 2).x, bounding_box.CornerPoint(i + 2).y, bounding_box.CornerPoint(i + 2).z);
	}

	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}