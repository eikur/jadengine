#ifndef __MODULEEDITORGUI_H__
#define __MODULEEDITORGUI_H__

#include "Module.h"


class ModuleEditorGUI : public Module
{
public: 
	ModuleEditorGUI( bool active = true);
	~ModuleEditorGUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();

};

#endif