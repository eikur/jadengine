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

	// Called before quitting
	bool CleanUp();

	// Load configuration from file
	bool LoadConfigFromFile(const char* file_path);

	float4x4 GetProjectionMatrix() const;
	float3x4 GetViewMatrix() const;

private:
	Frustum frustum;

	int m_screen_height = 0;
	int m_screen_size = 0;
	int m_screen_width = 0;
	int m_speed = 0;
	bool m_vsync = true;

	int m_limit_margin = 0;
};

#endif // __MODULEEDITORCAMERA_H__