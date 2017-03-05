#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject; 

class Component
{

public: 
	enum componentType{ TRANSFORM, CAMERA, UNKNOWN };
	bool active = true;
	componentType type = UNKNOWN;

private: 
	GameObject* parent = nullptr;

public: 

	Component(GameObject* parent, componentType type = UNKNOWN, bool active = true);
	void Enable() { active = true;  }
	void Disable() { active = false; }

	virtual bool Update( float dt = 0.0f ) = 0;
	virtual bool CleanUp() = 0; 
	virtual void OnEditor() = 0; 
	
};

#endif
