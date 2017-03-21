#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ComponentCamera.h"
#include "ModuleWindow.h"


ComponentCamera::ComponentCamera(GameObject* parent, bool active) : Component(parent, CAMERA, active)
{
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::CleanUp()
{
	return true;
}

void ComponentCamera::OnEditor()
{
}

void ComponentCamera::Init()
{
	float near_plane_distance = 0.1f;
	float far_plane_distance = 100.0f;

	// Set vertical Field-of-view (parameter angle in degrees)
	SetFOV(60.0f);
	SetPlaneDistances(near_plane_distance, far_plane_distance);

	frustum.SetFrame(float3::zero, -float3::unitZ, float3::unitY);
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
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
	float aspect_ratio = (float)App->window->m_screen_width / (float)App->window->m_screen_height;
	frustum.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), aspect_ratio);
}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	float horizontal_fov = 2.0f * atanf(tanf(frustum.VerticalFov() / 2.0f) * aspect_ratio);
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ComponentCamera::SetPlaneDistances(float near_plane_distance, float far_plane_distance)
{
	frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);
}

void ComponentCamera::Position(float3 pos)
{
	frustum.SetPos(pos);
}

void ComponentCamera::Orientation(float3 front, float3 up)
{
	frustum.SetFrame(frustum.Pos(), front, up);
	//Calling this function recomputes the cached world matrix of this Frustum. As a micro-optimization, prefer this function 
	//over the individual SetPos/SetFront/SetUp functions if you need to do a batch of two or more changes, to avoid redundant recomputation of the world matrix.
}

void ComponentCamera::LookAt(float3 look_at)
{
	float3 new_front = look_at - frustum.Pos();
	new_front.Normalize();
	float3 translation = new_front - frustum.Front();
	Orientation(new_front, frustum.Up() + translation);
}
