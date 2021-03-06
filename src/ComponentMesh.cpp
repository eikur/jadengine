#include "Globals.h"
#include "GameObject.h"
#include "ComponentMaterial.h"

#include "ImGui/imgui.h"
#include "glew-2.0.0\include\GL\glew.h"

#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(GameObject* parent, bool active) : Component(parent, MESH, active)
{
}

ComponentMesh::~ComponentMesh()
{
	
}

bool ComponentMesh::Init()
{
	mesh->Init();
	return true;
}

bool ComponentMesh::Update(float)
{
	if (component_material != nullptr)
		mesh->SetMaterial(component_material->GetMaterial());

	if (shown_in_active_camera)
	{
		mesh->Update();
		mesh->Draw();
	}

	return true;
}

bool ComponentMesh::CleanUp()
{
	RELEASE(mesh); 
	return true; 
}

void ComponentMesh::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Checkbox("Enabled", &this->active);
		ImGui::SameLine();
		ImGui::Checkbox("Draw AABB", &show_bounding_box);
		float bx[2] = { (float)bounding_box.MinX(), (float)bounding_box.MaxX() };
		float by[2] = { bounding_box.MinY(), bounding_box.MaxY() };
		float bz[2] = { bounding_box.MinZ(), bounding_box.MaxZ() };
		ImGui::DragFloat2("AABB.x", bx);
		ImGui::DragFloat2("AABB.y", by);
		ImGui::DragFloat2("AABB.z", bz);
	}
}

//----- 

void ComponentMesh::SetComponentMaterial(ComponentMaterial *comp_mat)
{
	component_material = comp_mat;
}
void ComponentMesh::LoadMesh(aiMesh *ai_mesh, Material *material)
{
	if (mesh == nullptr)
	{
		mesh = new Mesh(ai_mesh, material);
		parent->UpdateBoundingBoxesRecursively();
	}
}

void ComponentMesh::LoadVertices(float3 *vertices, unsigned num_vertices, unsigned *indices, unsigned num_indices, float2 *tex_coords, unsigned num_tex_coords)
{
	if (mesh == nullptr)
	{
		mesh = new Mesh(vertices, num_vertices, indices, num_indices, tex_coords, num_tex_coords);
		parent->UpdateBoundingBoxesRecursively(); 
	}
}

void ComponentMesh::ShowBoundingBox()
{
	glColor3f(0.0f, 1.0f, 0.0f); 
	glDisable(GL_LIGHTING);

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

	glEnable(GL_LIGHTING); 
	glColor3f(1.0f, 1.0f, 1.0f);
}

void ComponentMesh::UpdateBoundingBox( float4x4 parent_world_transform)
{
	bounding_box.SetNegativeInfinity();
	
	float3* world_vertices = new float3[mesh->num_vertices];
	for (int i = 0; i < mesh->num_vertices; i++)
		world_vertices[i] = (parent_world_transform * mesh->vertices[i].ToPos4()).Float3Part();

	bounding_box.Enclose((float3*)world_vertices, mesh->num_vertices); 
	
	delete world_vertices;
}

AABB ComponentMesh::GetBoundingBox() const
{
	return bounding_box;
}



bool ComponentMesh::DebugDraw()
{
	if (show_bounding_box)
		ShowBoundingBox(); 
	return true; 
}