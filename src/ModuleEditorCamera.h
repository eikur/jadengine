#ifndef __MODULEEDITORCAMERA_H__
#define __MODULEEDITORCAMERA_H__

#include "Module.h"
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject;
class ComponentTransform; 
class ComponentCamera;
class ParticleSystem;

class ModuleEditorCamera : public Module
{

public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	update_status Update( float dt = 0.0f);
	GameObject* CreateCameraGameObject(const std::string& name = "Unnamed") const;
	ComponentCamera* GetCameraComponent() const;

	// Called before quitting
	bool CleanUp();

private:
	// Load configuration from file
	void CreateEditorCamera();
	bool LoadConfigFromFile(const char* file_path);

private:
	GameObject* m_camera_gameobject = nullptr;
	ComponentTransform* m_transform = nullptr; 
	ComponentCamera* m_camera_component = nullptr;
	ParticleSystem* m_particle_system = nullptr;

	float fixed_dt = 0.02f;

	int m_screen_height = 0;
	int m_screen_width = 0;
	int m_speed = 0;
	bool m_vsync = true;

	float m_advance_speed = 3.0f;
	float m_advance_speed_modifier = 3.0f;

	float m_mouse_speed = 0.5f;
	float m_mouse_wheel_speed = 10.0f;
	
	float m_rotation_speed = 10.0f;
	float m_rotation_speed_modifier = 5.0f;
};

#endif // __MODULEEDITORCAMERA_H__