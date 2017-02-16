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
	ImGui_ImplSdlGL3_NewFrame(App->window->m_window);
	clear_color = ImColor(114, 144, 154);
	ImGui::Text("Hello world");
	ImGui::ColorEdit3("clear color", (float*)&clear_color);

	return UPDATE_CONTINUE;
}

bool ModuleEditorGUI::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}