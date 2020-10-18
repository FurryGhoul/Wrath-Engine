#include "UI_Hierarchy.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleInterface.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"



#include "ImGui/imgui_internal.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

UI_Hierarchy::UI_Hierarchy(Application* app, bool start_enabled) : UI_Element(app, start_enabled)
{
	prim_posx = 0.0f; prim_posy = 0.0f; prim_posz = 0.0f;
	prim_sizeX = 100; prim_sizeY = 100;
}

UI_Hierarchy::~UI_Hierarchy() {}

update_status UI_Hierarchy::Update(float dt)
{

	return UPDATE_CONTINUE;
}


void UI_Hierarchy::Draw(bool* open)
{
	if (ImGui::Begin("Hierarchy", open))
	{
		ImGui::Separator();
		static int slices = 30; static int stacks = 30;

		if (ImGui::CollapsingHeader("Create Primitives"))
		{
			ImGui::Separator();

			if (ImGui::Button("Create Cube")){}

			if (ImGui::Button("Create Plane")){}

			if (ImGui::Button("Create Sphere")){}

			if (ImGui::Button("Create Cone")){}
		}
	}
	ImGui::End();
}
