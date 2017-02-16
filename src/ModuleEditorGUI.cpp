#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

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
	ImVec4 clear_color = ImColor(114, 144, 154);

	ImGui_ImplSdlGL3_NewFrame(App->window->m_window);

//	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("About",  &t);
	ImGui::Text("jadengine");
	ImGui::Text("LINE OF DESCRIPTIOn");
	ImGui::Text("author1 // author2 // author 3");
	ImGui::Text("Doom pit of hell libraries");
	ImGui::Text("License: GPL2.0");
	ImGui::End();

	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);

	ImGui::Render();
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