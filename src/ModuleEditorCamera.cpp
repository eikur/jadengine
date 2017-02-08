#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"


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
		float vertical_FOV = DegToRad(60.0f);
		float aspect_ratio = (float)m_screen_width / (float)m_screen_height;
		float horizontal_FOV = 2.0f * atanf(tanf(vertical_FOV / 2.0f) * aspect_ratio);
		float near_plane_distance = 1.0f;
		float far_plane_distance = 10.0f;

		frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
		frustum.SetPerspective(horizontal_FOV, vertical_FOV);
		frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);

		frustum.SetPos(float3::zero);
		frustum.SetFront(float3::unitZ);
		frustum.SetUp(float3::unitY);
		
	}

	return ret;
}

bool ModuleEditorCamera::Start()
{
	return true;
}

update_status ModuleEditorCamera::Update( float dt )
{
	float3 advance_direction = frustum.Front() - frustum.Pos();
	advance_direction.Normalize();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{ 

		frustum.SetPos(frustum.Pos() + advance_direction*m_advance_speed*dt);
		MYLOG("Advancing!")
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		frustum.SetPos(frustum.Pos() -advance_direction*m_advance_speed*dt);
		MYLOG("Receding!");
	}

	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

float3x4 ModuleEditorCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

float4x4 ModuleEditorCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

bool ModuleEditorCamera::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;

	m_screen_width = (int)json_object_dotget_number(json_object(root_value), "window.screen_width");
	m_screen_height = (int)json_object_dotget_number(json_object(root_value), "window.screen_height");
	m_screen_size = (int)json_object_dotget_number(json_object(root_value), "window.screen_size");
	m_vsync = (json_object_dotget_boolean(json_object(root_value), "window.vsync") != 0) ? true : false;

	m_limit_margin = (int)json_object_dotget_number(json_object(root_value), "renderer.camera.x_limit_margin");
	m_speed = (int)json_object_dotget_number(json_object(root_value), "renderer.camera.speed");

	json_value_free(root_value);

	if (m_screen_width == 0 || m_screen_height == 0 || m_screen_size == 0)
		return false;
	else
		return true;
}
