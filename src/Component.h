#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject; 

class Component
{

public: 
	enum componentType{ TRANSFORM, MESH, MATERIAL, ANIMATION, CAMERA, BILLBOARD, PARTICLES, PHYSICS, UNKNOWN };
	bool active = true;
	componentType type = UNKNOWN;

protected: 
	GameObject* parent = nullptr;

public: 
	Component(GameObject* parent, componentType type = UNKNOWN, bool active = true);
	virtual ~Component();

	void Enable() { active = true;  }
	void Disable() { active = false; }

	virtual bool Init();
	virtual bool Update( float dt = 0.0f );
	virtual bool CleanUp();

	virtual bool DebugDraw();

	virtual void OnEditor() = 0; 
	
};

#endif
