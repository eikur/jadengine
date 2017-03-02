#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject; 

class Component
{

public: 
	enum componentType{ DEFAULT, TRANSFORM, CAMERA };

private: 
	bool active = true; 
	componentType type = DEFAULT;
	GameObject* parent = nullptr;

public: 
	virtual void Enable() { active = true;  }
	virtual void Update() {}
	virtual void Disable() { active = false;  }
};

#endif
