#ifndef __MODULEEDITORCAMERA_H__
#define __MODULEEDITORCAMERA_H__

#include "Module.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ModuleEditorCamera : public Module
{

public:

	ModuleEditorCamera();

	// Destructor
	virtual ~ModuleEditorCamera();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	update_status Update( float dt = 0.0f);

	// Called before quitting
	bool CleanUp();

	// Load configuration from file
	bool LoadConfigFromFile(const char* file_path);

	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;
	void SetFOV(float vertical_fov);
	void SetAspectRatio(float aspect_ratio);
	void SetPlaneDistances(float near_plane_distance, float far_plane_distance);
	void Position(float3 pos);
	void Orientation(float3 front, float3 up);
	void LookAt(float3 look_at);

private:
	Frustum frustum;

	int m_screen_height = 0;
	int m_screen_width = 0;
	int m_speed = 0;
	bool m_vsync = true;

	float m_advance_speed = 3.0f;
	float m_advance_speed_modifier = 3.0f;

	float m_mouse_speed = 0.5f;
	
	float m_rotation_speed = 10.0f;
	float m_rotation_speed_modifier = 5.0f;
};

#endif // __MODULEEDITORCAMERA_H__