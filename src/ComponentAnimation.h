#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMATION_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentAnimation : public Component
{
private:
	unsigned int animation_instance_id = 0; 
	int next_animation_instance_id = -1; 
	bool valid_animation_instance_id = false;

public:
	ComponentAnimation(GameObject* parent, bool active = true);
	~ComponentAnimation();

public:
	bool Update(float dt);
	bool CleanUp();
	void OnEditor();

	void SetAnimationInstanceID(unsigned int new_id);

	void GetTransform(const char* go_name, float3& position, Quat& rotation) const;

};

#endif
