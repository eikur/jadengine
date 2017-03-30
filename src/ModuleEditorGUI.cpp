#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <Windows.h>
#include "ModuleEditorCamera.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "ImGui/imgui_internal.h"

#include "ModuleEditorGUI.h"

ModuleEditorGUI::ModuleEditorGUI(bool active) 
: Module(active), show_hierarchy(true), show_inspector(true), show_player(true)
{
}

ModuleEditorGUI::~ModuleEditorGUI()
{
}

bool ModuleEditorGUI::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->m_window);
	return true;
}

update_status ModuleEditorGUI::Update(float)
{
	ImVec4 clear_color = ImColor(0, 0, 0);

	ImGui_ImplSdlGL3_NewFrame(App->window->m_window);

	if (show_about) ShowAbout(&show_about);
	if (show_console) ShowConsole(&show_console);
	if (show_hierarchy) ShowHierarchy(&show_hierarchy);
	if (show_inspector) ShowInspector(&show_inspector);
	if (show_stats) ShowStats(&show_stats);
	if (show_player) ShowPlayer(&show_player);

	if (ShowMainMenu() == false)
		return UPDATE_STOP;
	else
	{
	//	ImGui::ShowTestWindow();

		Draw();
		return UPDATE_CONTINUE;
	}
}

bool ModuleEditorGUI::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleEditorGUI::Draw()
{
	ImGui::Render();

}

//------------------------------ main menu --------------------------
bool ModuleEditorGUI::ShowMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "ESC")) { return false; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Console", NULL, &show_console)) {}
			if (ImGui::MenuItem("Hierarchy", NULL, &show_hierarchy)) {}
			if (ImGui::MenuItem("Inspector", NULL, &show_inspector)) {}
			if (ImGui::MenuItem("Stats", NULL, &show_stats)) {}
			if (ImGui::MenuItem("Player", NULL, &show_player)) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Documentation"))
		{
			if (ImGui::MenuItem("Repository")) {
				ShellExecute(NULL, "open", "www.github.com/eikur/jadengine",
					NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("About", NULL, &show_about)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return true;
}

//------------------------------ Show Menu Items --------------------------
void ModuleEditorGUI::ShowAbout(bool *enabled) const 
{
	ImGui::SetNextWindowPos(ImVec2(App->window->m_screen_width / 2 - 150, App->window->m_screen_height/2 - 100));
	if (ImGui::Begin("About JADEngine", enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)==false) {
		ImGui::End();
		return;
	}
	ImGui::Separator();
	ImGui::Text("JADEngine uses OpenGL, DevIL and Assimp libraries.");
	ImGui::Text("For further detail, check Source Code.");
	ImGui::Separator();
	ImGui::Text("This engine is licensed under GPL 2.0 clauses.");
	ImGui::Separator();
	ImGui::Text("Adolfo Zarrias // Daniel Perez // Jorge Soriano");
	ImGui::Text("2017");
	ImGui::End();
}

void ModuleEditorGUI::ShowConsole(bool *p_open) const {
	// example > log
}

void ModuleEditorGUI::ShowStats(bool *enabled) const {
	// example> overlayLayout
	ImGui::SetNextWindowPos(ImVec2(App->window->m_screen_width*3/4, 25));
	ImGui::SetNextWindowSize(ImVec2(App->window->m_screen_width/4 - 5, 80));
	if (!ImGui::Begin("Statistics", enabled, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("Stats");
	ImGui::Separator();
	ImGui::Text("%.2f/60 fps", App->FPS );
	ImGui::Text("Mouse: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	ImGui::End();

}

void ModuleEditorGUI::ShowPlayer(bool *enabled) const {
	// example> overlayLayout
	unsigned short int width = 200;
	ImGui::SetNextWindowPos(ImVec2(App->window->m_screen_width / 2 - width / 2, 25));
	ImGui::SetNextWindowSize(ImVec2(width, 40));
	if (!ImGui::Begin("Player", enabled, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Columns(3, 0, false);
	ImGui::Button("Play", ImVec2(50, 20)); 
	ImGui::NextColumn();
	ImGui::Button("Pause", ImVec2(50, 20));
	ImGui::NextColumn();
	ImGui::Button("Tick", ImVec2(50, 20));
	ImGui::End();
}

void ModuleEditorGUI::ShowInspector(bool *enabled) const {
	// example > Widgets > Collapsing Headers
	ImGui::SetNextWindowPos(ImVec2(25, App->window->m_screen_height / 2 + 25));
	ImGui::SetNextWindowSizeConstraints(ImVec2(App->window->m_screen_width / 4, App->window->m_screen_height / 2 -25), ImVec2(App->window->m_screen_width / 4, App->window->m_screen_height / 2));

	if (ImGui::Begin("Inspector", enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize) == false) {
		ImGui::End();
		return;
	}
	if (inspector_selected_gameobject != nullptr)
	{
		inspector_selected_gameobject->OnEditor(); 
	}
	ImGui::End();
}

void ModuleEditorGUI::ShowHierarchy(bool *enabled) {

	// example > Widgets > Advanced tree node
	ImGui::SetNextWindowPos(ImVec2(25, 25));
	ImGui::SetNextWindowSizeConstraints(ImVec2(App->window->m_screen_width / 4, App->window->m_screen_height / 2-25), ImVec2(App->window->m_screen_width / 4, App->window->m_screen_height / 2));

	if (ImGui::Begin("Scene Hierarchy", enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize) == false) {
		ImGui::End();
		return;
	}
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (ImGui::TreeNode("Root"))
	{
		int i = 0; 
		for (std::vector<GameObject*>::const_iterator it = App->scene->game_objects.cbegin(); it != App->scene->game_objects.cend(); ++it)
		{
			i = (*it)->OnHierarchy(i, node_flags, inspector_selected_gameobject); // generate trees for each game object
			i++;
		}
		ImGui::TreePop();
	}
	ImGui::End();
	
}