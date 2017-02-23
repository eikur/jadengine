#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <Windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "ImGui/imgui_internal.h"

#include "ModuleEditorGUI.h"

ModuleEditorGUI::ModuleEditorGUI(bool active) : Module(active)
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
	bool t = true;
	ImVec4 clear_color = ImColor(0, 0, 0);

	ImGui_ImplSdlGL3_NewFrame(App->window->m_window);

	//ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiWindowFlags_AlwaysAutoResize);
	if (show_about) ShowAbout(&show_about);
	if (show_console) ShowConsole(&show_console);
	if (show_hierarchy) ShowHierarchy(&show_hierarchy);
	if (show_inspector) ShowInspector(&show_inspector);
	if (show_stats) ShowStats(&show_stats);

	if (ShowMainMenu() == false)
		return UPDATE_STOP;

// example > log
// example> overlayLayout
// help > about
	ImGui::ShowTestWindow();

	Draw();
	return UPDATE_CONTINUE;
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
void ModuleEditorGUI::ShowAbout(bool *enabled)
{
	ImGui::Begin("About JADEngine", enabled, ImGuiWindowFlags_AlwaysAutoResize);
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

void ModuleEditorGUI::ShowConsole(bool *p_open) {

}

void ModuleEditorGUI::ShowStats(bool *p_open) {

}

void ModuleEditorGUI::ShowInspector(bool *p_open) {

}

void ModuleEditorGUI::ShowHierarchy(bool *p_open) {

}