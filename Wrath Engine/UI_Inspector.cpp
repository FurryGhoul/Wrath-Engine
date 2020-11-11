#include "UI_Inspector.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInterface.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#include "MathGeoLib/Math/float4x4.h"

UI_Inspector::UI_Inspector(Application* app, bool start_enabled) : UI_Element(app, start_enabled) {}

UI_Inspector::~UI_Inspector() {}

void UI_Inspector::Draw(GameObject* selectedGO, bool* open)
{
	if (ImGui::Begin("Inspector", open))
	{
		static bool foo = false;
		static bool transform = true;
		const char* name = "";

		for (auto item = App->scene->gameobjects.begin(); item != App->scene->gameobjects.end(); ++item)
			if ((*item)->selected) { name = (*item)->name.c_str(); }

		ImGui::Text(name);
		ImGui::Separator();
		ImGui::Checkbox("Active", &foo);
		
		if (selectedGO != nullptr)
		{
			if (ComponentTransform* transform = (ComponentTransform*)selectedGO->GetComponent(TRANSFORM))
			{
				ImGui::Text("Transform");

				math::float4x4 localTransform = transform->GetGlobalMatrix();
				
				ImGui::DragFloat3("Position", (float*)&localTransform.TranslatePart(), 0.25f);
				ImGui::SliderFloat3("Rotation", (float*)&localTransform.RotatePart().ToEulerXYZ(), 0.0f, 360.0f);
				ImGui::DragFloat3("Scale", (float*)&localTransform.ExtractScale(), 0.25f, 1.0f, 1000.0f);
				ImGui::Separator();
				ImGui::Separator();
			}

			if (ComponentMesh* mesh = (ComponentMesh*)selectedGO->GetComponent(MESH))
			{
				ImGui::Text("Mesh");

				ImGui::Text("Number of vertices: %d", mesh->num_vertices);
				ImGui::Text("Number of indices: %d", mesh->num_indices);
				ImGui::Text("Number of children: %d", selectedGO->children.size());
				ImGui::Separator();
				ImGui::Checkbox("Normals", &App->scene->normals);
				ImGui::Separator();
				ImGui::Separator();
			}

			if (ComponentMaterial* material = (ComponentMaterial*)selectedGO->GetComponent(MATERIAL))
			{
				ImGui::Text("Material");

				ImGui::Text("Name: %s", material->name.c_str());
				float panelWidth = ImGui::GetWindowContentRegionWidth();
				float conversionFactor = panelWidth / material->width;
				ImVec2 imageSize = { material->height * conversionFactor, panelWidth };
				ImGui::Image((ImTextureID)material->textureID, imageSize);
				ImGui::Text("Texture Width: %u", material->width);
				ImGui::Text("Texture Height: %u", material->height);
				ImGui::Text("Path: MotoresJuan/Game/%s", material->path.c_str());
			}
		}
	}
	ImGui::End();
}
