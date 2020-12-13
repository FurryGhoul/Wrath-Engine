#include "UI_Inspector.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInterface.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "MathGeoLib/Math/float4x4.h"

UI_Inspector::UI_Inspector(Application* app, bool start_enabled) : UI_Element(app, start_enabled) {}

UI_Inspector::~UI_Inspector() {}

void UI_Inspector::Draw(GameObject* selectedGO, bool* open)
{
	if (ImGui::Begin("Inspector", open))
	{
		static bool foo = false;
		static bool cameraActive = false;
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
				bool modified = false;
				
				ImGui::PushItemWidth(75);
				ImGui::Text("Position:");
				ImGui::Text("X:"); ImGui::SameLine();
				ImGui::PushID("1");
				if (ImGui::InputFloat("", &transform->compTranslation.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::SameLine();
				ImGui::PopID();
				ImGui::Text("Y:"); ImGui::SameLine();
				ImGui::PushID("2");
				if (ImGui::InputFloat("", &transform->compTranslation.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::SameLine();
				ImGui::PopID();
				ImGui::Text("Z:"); ImGui::SameLine();
				ImGui::PushID("3");
				if (ImGui::InputFloat("", &transform->compTranslation.z, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::PopID();

				ImGui::Text("Rotation:");
				ImGui::Text("X:"); ImGui::SameLine();
				ImGui::PushID("4");
				if (ImGui::InputFloat("", &transform->compRotation.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::SameLine();
				ImGui::PopID();
				ImGui::Text("Y:"); ImGui::SameLine();
				ImGui::PushID("5");
				if (ImGui::InputFloat("", &transform->compRotation.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::SameLine();
				ImGui::PopID();
				ImGui::Text("Z:"); ImGui::SameLine();
				ImGui::PushID("6");
				if (ImGui::InputFloat("", &transform->compRotation.z, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::PopID();

				ImGui::Text("Scale:");
				ImGui::Text("X:"); ImGui::SameLine();
				ImGui::PushID("7");
				if (ImGui::InputFloat("", &transform->compScale.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::SameLine();
				ImGui::PopID();
				ImGui::Text("Y:"); ImGui::SameLine();
				ImGui::PushID("8");
				if (ImGui::InputFloat("", &transform->compScale.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::SameLine();
				ImGui::PopID();
				ImGui::Text("Z:"); ImGui::SameLine();
				ImGui::PushID("9");
				if (ImGui::InputFloat("", &transform->compScale.z, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					modified = true;
				}
				ImGui::PopID();
				ImGui::PopItemWidth();

				ImGui::Separator();
				ImGui::Separator();

				if (modified)
				{
					transform->SetLocalMatrix(float4x4::FromTRS(transform->compTranslation, transform->compRotation, transform->compScale));
					App->renderer3D->RecalculateGlobalMatrix(App->scene->root);
					modified = false;
					App->scene->root->RecalculateAABB();
				}
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

			if (ComponentCamera* camera = (ComponentCamera*)selectedGO->GetComponent(CAMERA))
			{
				ImGui::Text("Camera");
				ImGui::Checkbox("Activate Camera", &camera->active);
				ImGui::Checkbox("Culling", &camera->culling);
			}
		}
	}
	ImGui::End();
}
