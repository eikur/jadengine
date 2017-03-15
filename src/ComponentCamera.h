#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{

public:
	ComponentCamera(GameObject* parent, bool active = true);
	~ComponentCamera();

	bool CleanUp();
	void OnEditor();

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
};

#endif // __COMPONENTCAMERA_H__