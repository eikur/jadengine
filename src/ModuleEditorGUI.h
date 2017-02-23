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

private:
	bool ShowMainMenu();

	void ShowConsole(bool *p_open);
	void ShowStats(bool *p_open);
	void ShowInspector(bool *p_open);
	void ShowHierarchy(bool *p_open);
	
	void ShowAbout(bool *p_open);

private:
	bool show_about = false;
	bool show_console = false;
	bool show_inspector = false;
	bool show_hierarchy = false;
	bool show_stats = false;
};

#endif