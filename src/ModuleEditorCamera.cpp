#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "brofiler/Brofiler.h"

#include "ModuleEditorCamera.h"
#include "ParticleSystem.h"

ModuleEditorCamera::ModuleEditorCamera()
{
	CreateEditorCamera(); 
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
		m_camera_component->Init();
	}

	return ret;
}

bool ModuleEditorCamera::Start()
{
	return true;
}

update_status ModuleEditorCamera::Update(float dt)
{
	BROFILER_CATEGORY("ModuleEditorCamera", Profiler::Color::Green)
	m_camera_gameobject->Update(dt); 

	/* EDITOR CONTROLS*/
	float3 front = m_camera_component->frustum.Front();
	float3 up = m_camera_component->frustum.Up();
	float3 right = m_camera_component->frustum.WorldRight();

	
	float mod = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT ? m_advance_speed_modifier : (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT ? m_advance_speed_modifier : 1.0f);

	iPoint mouse_motion = App->input->GetMouseMotion();
	int mouse_wheel = App->input->GetMouseWheel();

	if (mouse_wheel != 0)
		m_transform->position += front*(float)mouse_wheel * m_mouse_wheel_speed* mod * fixed_dt;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (mouse_motion.y != 0)
			m_transform->position += up* (float) mouse_motion.y * m_mouse_speed * mod* fixed_dt;
		if (mouse_motion.x != 0)
			m_transform->position -= right*(float)mouse_motion.x * m_mouse_speed *  mod * fixed_dt;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (mouse_motion.IsZero() == false)
		{
			Quat rotation = Quat::identity;

			rotation = rotation.RotateAxisAngle(right, -DegToRad(m_rotation_speed)*fixed_dt*mouse_motion.y);
			front = rotation.Mul(front);	
			up = rotation.Mul(up);
			
			rotation = rotation.RotateY(-DegToRad(m_rotation_speed)*fixed_dt*mouse_motion.x);
			front = rotation.Mul(front); 
			up = rotation.Mul(up); 
			
			m_camera_component->Orientation(front, up);
			m_transform->rotation = m_transform->rotation.LookAt(-float3::unitZ, front, float3::unitY, up);
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			m_transform->position += front*m_advance_speed * mod * fixed_dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			m_transform->position -= front*m_advance_speed * mod * fixed_dt;
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
			m_transform->position += up*m_advance_speed * mod * fixed_dt;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
			m_transform->position -= up*m_advance_speed * mod * fixed_dt;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			m_transform->position += right*m_advance_speed * mod * fixed_dt;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			m_transform->position -= right*m_advance_speed * mod * fixed_dt;
	}

	return UPDATE_CONTINUE;
}

ComponentCamera* ModuleEditorCamera::GetCameraComponent() const
{
	return m_camera_component;
}

bool ModuleEditorCamera::CleanUp()
{
	m_camera_gameobject->CleanUp(); 
	RELEASE(m_camera_gameobject);
	return true;
}

GameObject* ModuleEditorCamera::CreateCameraGameObject(const std::string& name) const
{
	GameObject *new_cam_go = new GameObject(name.c_str()); 
	if (new_cam_go == nullptr)
		return nullptr;
	new_cam_go->CreateComponent(Component::componentType::TRANSFORM); 
	new_cam_go->CreateComponent(Component::componentType::CAMERA)->Init(); 
	new_cam_go->CreateComponent(Component::componentType::PARTICLES);

	return new_cam_go; 
}

void ModuleEditorCamera::CreateEditorCamera()
{
	m_camera_gameobject = new GameObject("Editor Camera");
	m_transform = (ComponentTransform*) m_camera_gameobject->CreateComponent(Component::componentType::TRANSFORM);
	m_camera_component = (ComponentCamera*)m_camera_gameobject->CreateComponent(Component::componentType::CAMERA);
	m_particle_system = (ParticleSystem*)m_camera_gameobject->CreateComponent(Component::componentType::PARTICLES);
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
