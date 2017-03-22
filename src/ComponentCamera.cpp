#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ImGui/imgui.h"
#include "GameObject.h"

#include "ComponentCamera.h"


ComponentCamera::ComponentCamera(GameObject* parent, bool active) : Component(parent, CAMERA, active)
{
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update(float)
{

	return true;
}

bool ComponentCamera::CleanUp()
{
	return true;
}

void ComponentCamera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Checkbox("Enabled", &active); 
		ImGui::SameLine();
		ImGui::Checkbox("Draw Frustum", &draw_frustum); 
		ImGui::DragFloat("Near Plane", &near_plane_distance); 
		ImGui::DragFloat("Far Plane", &far_plane_distance); 
		SetPlaneDistances(near_plane_distance, far_plane_distance); 
		ImGui::DragFloat("Field Of View", &field_of_view,0.5f); 
		SetFOV(field_of_view); 
		ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.01f); 
		SetAspectRatio(aspect_ratio); 
	}
}

bool ComponentCamera::DebugDraw()
{
	if (draw_frustum)
		DrawFrustum();
	return true; 
}

bool ComponentCamera::Init()
{
	near_plane_distance = 0.1f;
	far_plane_distance = 100.0f;
	field_of_view = 60.0f;

	// Set vertical Field-of-view (parameter angle in degrees)
	SetFOV(field_of_view);
	SetPlaneDistances(near_plane_distance, far_plane_distance);

	frustum.SetFrame(float3::zero, -float3::unitZ, float3::unitY);
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	return true;
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	float4x4 fvm = frustum.ViewMatrix();
	// change from row-major to column major
	fvm.Transpose();
	return fvm;
}

float4x4 ComponentCamera::GetProjectionMatrix() const
{
	float4x4 pvm = frustum.ProjectionMatrix();
	pvm.Transpose();
	return pvm;
}

void ComponentCamera::SetFOV(float vertical_fov)
{
	float new_aspect = (float)App->window->m_screen_width / (float)App->window->m_screen_height;
	frustum.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), new_aspect);
	aspect_ratio = new_aspect;
}

void ComponentCamera::SetAspectRatio(float new_aspect_ratio)
{
	float horizontal_fov = 2.0f * atanf(tanf(frustum.VerticalFov() / 2.0f) * new_aspect_ratio);
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, new_aspect_ratio);
	aspect_ratio = new_aspect_ratio;
}

void ComponentCamera::SetPlaneDistances(float new_near, float new_far)
{
	near_plane_distance = new_near;
	far_plane_distance = new_far;
	frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);
}

void ComponentCamera::Position(float3 pos)
{
	frustum.SetPos(pos);
}

void ComponentCamera::Orientation(float3 front, float3 up)
{
	frustum.SetFrame(frustum.Pos(), front, up);
}

void ComponentCamera::LookAt(float3 look_at)
{
	float3 new_front = look_at - frustum.Pos();
	new_front.Normalize();
	float3 translation = new_front - frustum.Front();
	Orientation(new_front, frustum.Up() + translation);
}

void ComponentCamera::DrawFrustum() {
	float3 *corner_points = new  float3[8];
	frustum.GetCornerPoints(corner_points);
	
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

}

void ComponentCamera::UpdateFrustumTransform(float4x4 parent_world_transform)
{
	float3 pos, scl;
	Quat rot; 
	parent_world_transform.Decompose(pos, rot, scl); 
	frustum.SetPos(pos);
	Orientation(rot.Mul(-float3::unitZ), rot.Mul(float3::unitY)); 
}