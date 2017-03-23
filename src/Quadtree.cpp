#include "GameObject.h"
#include "Globals.h"
#include "glew-2.0.0\include\GL\glew.h"

#include "Quadtree.h"

void Quadtree::Create(const AABB limits)
{
	root_node = new QuadtreeNode(limits); 
}

void Quadtree::Clear()
{
	root_node->Clear();
	delete root_node;
}

void Quadtree::Insert(GameObject* go_to_insert)
{
	root_node->Insert(go_to_insert); 

}

void Quadtree::Remove(GameObject* )
{

}

void Quadtree::DebugDraw()
{
	root_node->DrawDebug(); 
}

void Quadtree::QuadtreeNode::Insert(GameObject* go_to_insert)
{
//	if (boundary.Contains(go_to_insert->GetWorldPosition()))
	if (boundary.Contains(go_to_insert->GetLocalPosition()))
	{
		if (capacity < bucket_space)
		{
			objects.push_back(go_to_insert); 
			capacity++; 
		}
		else
		{
			if (children == nullptr)
			{
				children = CreateChildren(boundary); 
			}
			for (int i = 0; i < 4; ++i)
				children[i].Insert(go_to_insert); 
		}
	}
}

Quadtree::QuadtreeNode* Quadtree::QuadtreeNode::CreateChildren(AABB parent_AABB)
{
	QuadtreeNode* children = new QuadtreeNode[4];
	
	float3 min = { boundary.CenterPoint().x, boundary.MinY(), boundary.CenterPoint().z };
	float3 max = { boundary.MaxX(), boundary.MaxY(), boundary.MaxZ() };
	children[0].boundary = AABB(min, max); 
	children[0].bucket_space = bucket_space;

	
	min = { boundary.MinX(), boundary.MinY(), boundary.CenterPoint().z };
	max = { boundary.CenterPoint().x, boundary.MaxY(), boundary.MaxZ() };

	children[1].boundary = AABB(min, max); 
	children[1].bucket_space = bucket_space;

	min = { boundary.MinX(), boundary.MinY(), boundary.MinZ() };
	max = {boundary.CenterPoint().x, boundary.MaxY(), boundary.CenterPoint().z};
	children[2].boundary = AABB(min, max); 
	children[2].bucket_space = bucket_space;

	min = { boundary.CenterPoint().x, boundary.MinY(), boundary.MinZ() };
	max = {boundary.MaxX(), boundary.MaxY(), boundary.CenterPoint().z};
	children[3].boundary = AABB(min, max); 
	children[3].bucket_space = bucket_space;

	return children; 
}

void Quadtree::QuadtreeNode::DrawDebug()
{
	float3 *corner_points = new  float3[8];
	boundary.GetCornerPoints(corner_points);

	glColor3f(1.0f, 1.0f, 0.0f);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	for (int i = 0; i < 8; i++)
		glVertex3f(corner_points[i].x, corner_points[i].y, corner_points[i].z);
	for (int i = 0; i < 4; i++)
	{
		glVertex3f(corner_points[i].x, corner_points[i].y, corner_points[i].z);
		glVertex3f(corner_points[i + 4].x, corner_points[i + 4].y, corner_points[i + 4].z);
	}
	for (int i = 0; i < 2; i++)
	{
		glVertex3f(corner_points[i].x, corner_points[i].y, corner_points[i].z);
		glVertex3f(corner_points[i + 2].x, corner_points[i + 2].y, corner_points[i + 2].z);
	}
	for (int i = 4; i < 6; i++)
	{
		glVertex3f(corner_points[i].x, corner_points[i].y, corner_points[i].z);
		glVertex3f(corner_points[i + 2].x, corner_points[i + 2].y, corner_points[i + 2].z);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	delete corner_points;

	if (children != nullptr)
		for (int i = 0; i < 4; ++i)
			children[i].DrawDebug(); 
}

void Quadtree::QuadtreeNode::Clear()
{
	if (children != nullptr)
	{
		for (int i = 0; i < 4; ++i)
			children[i].Clear();
		delete[] children; 
	}
}