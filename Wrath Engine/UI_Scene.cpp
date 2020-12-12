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
		vec2 win_size = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

		ImGui::SetCursorPos(ImVec2(0, 0));

		App->ui->uiScreenX = ImGui::GetWindowPos().x + ImGui::GetCursorPos().x;
		App->ui->uiScreenY = ImGui::GetWindowPos().y + ImGui::GetCursorPos().y;

		if (App->ui->uiScreenW != win_size.x || App->ui->uiScreenH != win_size.y)
		{
			float newAspectRatio = win_size.x/ win_size.y;
			App->camera->editorCamera->SetAspectRatio(newAspectRatio);
			App->renderer3D->changedFOV = true;
			App->ui->uiScreenW = win_size.x;
			App->ui->uiScreenH = win_size.y;
			LOG("x %f, y %f", win_size.x, win_size.y);
		}

		ImGui::Image((ImTextureID)App->renderer3D->buffer_tex, ImVec2(win_size.x, win_size.y), { 0,1 }, { 1,0 });
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

void UI_Scene::CubeDirectMode()
{
	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);

	glColor3f(1.f, 1.f, 1.f);

	//FRONT
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);

	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);

	//RIGHT
	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, -1, -1);

	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);

	//LEFT
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);

	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, 1, 1);

	//BACK
	glVertex3f(1, 1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, -1, -1);

	glVertex3f(-1, -1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);

	//TOP
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, -1);

	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, 1);

	//BOT
	glVertex3f(1, -1, 1);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, -1, -1);

	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);

	glEnd();
}

void UI_Scene::CubeVertexArray()
{
	float vertexArray[] = {
		-1, 1, 1,
		-1, -1, 1,
		1, -1, 1,

		1, -1, 1,
		1, 1, 1,
		-1, 1, 1,

		1, 1, 1,
		1, -1, 1,
		1, -1, -1,

		1, 1, 1,
		1, -1, -1,
		1, 1, -1,

		-1, 1, -1,
		-1, -1, -1,
		-1, -1, 1,

		-1, 1, -1,
		-1, -1, 1,
		-1, 1, 1,

		1, 1, -1,
		1, -1, -1,
		-1, -1, -1,

		-1, -1, -1,
		-1, 1, -1,
		1, 1, -1,

		-1, 1, 1,
		1, 1, 1,
		-1, 1, -1,

		1, 1, -1,
		-1, 1, -1,
		1, 1, 1,

		1, -1, 1,
		-1, -1, 1,
		-1, -1, -1,

		-1, -1, -1,
		1, -1, -1,
		1, -1, 1,
	};

	cbo = 0;
	glGenBuffers(1, (GLuint*)&(cbo));
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, vertexArray, GL_STATIC_DRAW);
}