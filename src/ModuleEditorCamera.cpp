#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"


ModuleEditorCamera::ModuleEditorCamera()
{

}

ModuleEditorCamera::~ModuleEditorCamera()
{

}

bool ModuleEditorCamera::Init()
{
	bool ret = true;

	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		MYLOG("ModuleEditorCamera: Unable to load configuration from file\n");
		ret = false;
	}
	else
	{
		float near_plane_distance = 0.1f;
		float far_plane_distance = 100.0f;
		
		// Set vertical Field-of-view (parameter angle in degrees)
		SetFOV(60.0f);
		SetPlaneDistances(near_plane_distance, far_plane_distance);
		
		frustum.SetFrame(float3::zero, -float3::unitZ, float3::unitY);
		frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	}

	return ret;
}

bool ModuleEditorCamera::Start()
{
	return true;
}

update_status ModuleEditorCamera::Update(float dt)
{
	float3 front = frustum.Front();
	float3 up = frustum.Up();
	float3 right = frustum.WorldRight();
	
	float mod = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT ? m_advance_speed_modifier : (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT ? m_advance_speed_modifier : 1.0f);

	Quat rotation = Quat::identity;
	float angle_up_z = RadToDeg(up.AngleBetween(float3::unitZ));
	
	iPoint mouse_motion = App->input->GetMouseMotion();
	int mouse_wheel = App->input->GetMouseWheel();

	if (mouse_wheel != 0)
		Position(frustum.Pos() + front*(float)mouse_wheel * m_mouse_wheel_speed* mod * dt);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (mouse_motion.y != 0)
			Position(frustum.Pos() + up* (float) mouse_motion.y * m_mouse_speed * mod* dt);
		if (mouse_motion.x != 0)
			Position(frustum.Pos() - right*(float)mouse_motion.x * m_mouse_speed *  mod * dt);
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (mouse_motion.y != 0 )
		{
			if (mouse_motion.y > 0)
				rotation = rotation.RotateAxisAngle(right, -DegToRad(m_rotation_speed)*dt*m_rotation_speed_modifier);
			else
				rotation = rotation.RotateAxisAngle(right, DegToRad(m_rotation_speed)*dt*m_rotation_speed_modifier);
			Orientation(rotation.Mul(front), rotation.Mul(up));
		}
		rotation = Quat::identity;
		if (mouse_motion.x != 0  )
		{
			if (mouse_motion.x > 0)
				rotation = rotation.RotateY(-DegToRad(m_rotation_speed)*dt*m_rotation_speed_modifier);
			else
				rotation = rotation.RotateY(DegToRad(m_rotation_speed)*dt*m_rotation_speed_modifier);
			Orientation(rotation.Mul(front), rotation.Mul(up));
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			Position(frustum.Pos() + front*m_advance_speed * mod * dt);
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			Position(frustum.Pos() - front*m_advance_speed * mod * dt);
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
			Position(frustum.Pos() + up*m_advance_speed * mod * dt);
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
			Position(frustum.Pos() - up*m_advance_speed * mod * dt);
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			Position(frustum.Pos() + right*m_advance_speed * mod * dt);
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			Position(frustum.Pos() - right*m_advance_speed * mod * dt);
	}
	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

float4x4 ModuleEditorCamera::GetViewMatrix() const
{
	float3x4 fvm = frustum.ViewMatrix();
	float4x4 tmp = float4x4::identity;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			tmp[i][j] = fvm[i][j];
	
	// change from row-major to column major
	return tmp.Transposed();
}

float4x4 ModuleEditorCamera::GetProjectionMatrix() const
{
	float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();
	return matrix;
}

void ModuleEditorCamera::SetFOV(float vertical_fov)
{
	float aspect_ratio = (float)App->window->m_screen_width / (float)App->window->m_screen_height;
	frustum.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), aspect_ratio);
}

void ModuleEditorCamera::SetAspectRatio(float aspect_ratio)
{
	float horizontal_fov = 2.0f * atanf(tanf(frustum.VerticalFov() / 2.0f) * aspect_ratio);
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleEditorCamera::SetPlaneDistances(float near_plane_distance, float far_plane_distance)
{
	frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);
}

void ModuleEditorCamera::Position(float3 pos)
{
	frustum.SetPos(pos);
}

void ModuleEditorCamera::Orientation(float3 front, float3 up)
{
	/*
	frustum.SetFront(front);
	frustum.SetUp(up);
	*/
	frustum.SetFrame(frustum.Pos(), front, up);
	//Calling this function recomputes the cached world matrix of this Frustum. As a micro-optimization, prefer this function 
	//over the individual SetPos/SetFront/SetUp functions if you need to do a batch of two or more changes, to avoid redundant recomputation of the world matrix.
}

void ModuleEditorCamera::LookAt(float3 look_at)
{
	float3 new_front = look_at - frustum.Pos();
	new_front.Normalize();
	float3 translation = new_front - frustum.Front();
	Orientation(new_front, frustum.Up() + translation);
}
//-----------------------------------------------------
bool ModuleEditorCamera::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;

	m_screen_width = (int)json_object_dotget_number(json_object(root_value), "window.screen_width");
	m_screen_height = (int)json_object_dotget_number(json_object(root_value), "window.screen_height");
	m_vsync = (json_object_dotget_boolean(json_object(root_value), "window.vsync") != 0) ? true : false;

	m_speed = (int)json_object_dotget_number(json_object(root_value), "renderer.camera.speed");

	json_value_free(root_value);

	if (m_screen_width == 0 || m_screen_height == 0)
		return false;
	else
		return true;
}
