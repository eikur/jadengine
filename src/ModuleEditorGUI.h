#ifndef __MODULEEDITORGUI_H__
#define __MODULEEDITORGUI_H__

#include "Module.h"
class GameObject;


class ModuleEditorGUI : public Module
{
public: 
	ModuleEditorGUI( bool active = true);
	~ModuleEditorGUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();

private:
	bool ShowMainMenu();

	void ShowConsole(bool *enabled) const;
	void ShowStats(bool *enabled) const;
	void ShowInspector(bool *enabled) const;
	void ShowHierarchy(bool *enabled);
	
	void ShowPhysicsConfiguration(bool *enabled) const; 

	void ShowAbout(bool *enabled) const;
	void ShowPlayer(bool *enabled) const;

private:
	bool show_about = false;
	bool show_console = false;
	bool show_inspector = false;
	bool show_hierarchy = false;
	bool show_stats = false;
	bool show_player = false;
	bool show_physics_configuration = false; 

	GameObject *inspector_selected_gameobject = nullptr;
};

#endif