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
		float near_plane_distance = 1.0f;
		float far_plane_distance = 100.0f;
		
		// Set vertical Field-of-view (parameter angle in degrees)
		SetFOV(60.0f);
		SetPlaneDistances(near_plane_distance, far_plane_distance);
		
		float3 frustum_pos = float3(0.0f, 2.0f, 5.0f);
		float3 frustum_front = float3(0.0f, 0.0f, -1.0f);
		float3 frustum_up = float3::unitY;
		frustum.SetFrame(frustum_pos, frustum_front, frustum_up);
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
	Quat rotation = Quat::identity;

	float3 advance_move = float3::zero;
	float mod = 0.0f;

	// pitch rotation
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		rotation.RotateY(DegToRad(m_rotation_speed));
		glRotatef(m_rotation_speed, 0.0f, 1.0f, 0.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		glRotatef(-m_rotation_speed, 0.0f, 1.0f, 0.0f);
		rotation.RotateY(DegToRad(m_rotation_speed));
	}



// translation
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		mod = m_advance_speed_modifier;
	else
		mod = 1.0f;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		advance_move += frustum.Front() * m_advance_speed * mod * dt;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		advance_move -= frustum.Front() * m_advance_speed * mod * dt;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		advance_move += frustum.Up() * m_advance_speed * mod * dt;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		advance_move -= frustum.Up() * m_advance_speed * mod * dt;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		advance_move += frustum.WorldRight() * m_advance_speed * mod * dt;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		advance_move -= frustum.WorldRight() * m_advance_speed * mod * dt;

	if (advance_move.Equals(float3::zero) == false)
	{
		//glMatrixMode(GL_MODELVIEW);
		glMatrixMode(GL_PROJECTION);
		glTranslatef(advance_move.x, advance_move.y, advance_move.z);
		frustum.Translate(advance_move);
	}

	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

float4x4 ModuleEditorCamera::GetViewMatrix() const
{
	float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();
	return matrix;
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
	frustum.SetFront(front);
	frustum.SetUp(up);
}

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
