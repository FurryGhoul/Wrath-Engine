#include "UI_Console.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleInterface.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"

UI_Console::UI_Console(Application* app, bool start_enabled) : UI_Element(app, start_enabled) {}
UI_Console::~UI_Console() {}

void UI_Console::Draw(bool* open)
{
	ImGui::Begin("Console", open);
	if (ImGui::Button("Clear")) { text_buffer.clear(); } ImGui::SameLine();
	if (ImGui::Button("Copy")) { ImGui::LogToClipboard(); }

	ImGui::TextUnformatted(text_buffer.begin());
	if (scrolling) ImGui::SetScrollHere(1.0f); scrolling = false;
	ImGui::End();
}

void UI_Console::CreateLog(const char* log)
{
	text_buffer.appendf(log);
	scrolling = true;
}
