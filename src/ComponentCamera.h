#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{

public:
	ComponentCamera(GameObject* parent, bool active = true);
	~ComponentCamera();

	bool Init();
	bool Update( float dt); 
	bool CleanUp();
	void OnEditor();
	
	bool Debug(); 

	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;
	void SetFOV(float vertical_fov);
	void SetAspectRatio(float aspect_ratio);
	void SetPlaneDistances(float near_plane_distance, float far_plane_distance);
	void Position(float3 pos);
	void Orientation(float3 front, float3 up);
	void LookAt(float3 look_at);

private:
	void DrawFrustum(); 

public:
	Frustum frustum;

private:
	float near_plane_distance = 0.1f;
	float far_plane_distance = 100.0f;
	float field_of_view = 60.0f;
	float aspect_ratio = 1.78f;

	bool draw_frustum = false;
};

#endif // __COMPONENTCAMERA_H__