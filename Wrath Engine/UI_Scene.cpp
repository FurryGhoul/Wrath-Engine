#include "UI_Scene.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInterface.h"
#include "ModuleWindow.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

#define MARGIN 85

UI_Scene::UI_Scene(Application* app, bool start_enabled) : UI_Element(app, start_enabled) {}

UI_Scene::~UI_Scene() {}

void UI_Scene::Draw(bool* open)
{
	if (ImGui::Begin("Scene", open, ImGuiWindowFlags_NoScrollbar))
	{
		vec2 win_size = (ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		vec2 screen_size = (App->window->width, App->window->height);

		ImGui::SetCursorPos(ImVec2(win_size.x - screen_size.x, win_size.y - screen_size.y));
		ImGui::Image((ImTextureID)App->renderer3D->buffer_tex, ImVec2(App->window->width, App->window->height), { 1,1 }, { 0,0 });

		if (active_grid) DrawGrid(grid_size);
		if (active_axis) DrawAxis(&active_axis);
	}
	ImGui::End();
}

void UI_Scene::DrawGrid(int GridSize)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);

	for (int i = -GridSize; i <= GridSize; i++)
	{
		glVertex3f((float)i, 0, (float)-GridSize);
		glVertex3f((float)i, 0, (float)GridSize);
		glVertex3f((float)-GridSize, 0, (float)i);
		glVertex3f((float)GridSize, 0, (float)i);
	}
	glEnd();
}

void UI_Scene::DrawAxis(bool active)
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);	glVertex3f(0.0, 0.0, 0.0);		glVertex3f(1.0, 0.0, 0.0);		// X
	glColor3f(0.0, 1.0, 0.0);	glVertex3f(0.0, 0.0, 0.0);		glVertex3f(0.0, 1.0, 0.0);		// Y
	glColor3f(0.0, 0.0, 1.0);	glVertex3f(0.0, 0.0, 0.0);		glVertex3f(0.0, 0.0, 1.0);		// Z

	glEnd();
}