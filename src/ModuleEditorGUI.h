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

	void ShowConsole(bool *p_open) const;
	void ShowStats(bool *p_open) const;
	void ShowInspector(bool *p_open) const;
	void ShowHierarchy(bool *p_open);
	
	void ShowAbout(bool *p_open) const;
	void ShowPlayer(bool *p_open) const;

private:
	bool show_about = false;
	bool show_console = false;
	bool show_inspector = false;
	bool show_hierarchy = false;
	bool show_stats = false;
	bool show_player = false;

	GameObject *inspector_selected_gameobject = nullptr;
};

#endif