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
			if (ImGui::MenuItem("Exit", "CTRL+X")) { return false; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Console", "C")) {}
			if (ImGui::MenuItem("Hierarchy", "H")) {}
			if (ImGui::MenuItem("Inspector", "I")) {}
			if (ImGui::MenuItem("Stats", "S")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Source Code")) {
				ShellExecute(NULL, "open", "www.github.com/eikur/jadengine",
					NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About JADEngine", NULL, &show_about)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return true;
}

//------------------------------ about --------------------------
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