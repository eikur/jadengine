#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMATION_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentAnimation : public Component
{
private:
	int animation_instance_id = -1; 
	int next_animation_instance_id = -1; 

public:
	ComponentAnimation(GameObject* parent, bool active = true);
	~ComponentAnimation();

public:
	bool Update(float dt);
	bool CleanUp();
	void OnEditor();

	void SetAnimationInstanceID( int new_id);
	void SetNextAnimationInstanceID(int next_id);

	void GetTransform(const char* go_name, float3& position, Quat& rotation) const;

};

#endif
