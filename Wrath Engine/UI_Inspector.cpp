#include "UI_Inspector.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInterface.h"
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

				math::float4x4 localTransform = transform->GetLocalMatrix();

				ImGui::DragInt3("Position", (int*)&localTransform.TranslatePart(), 0.25f);
				ImGui::SliderInt3("Rotation", (int*)&localTransform.RotatePart().ToEulerXYZ(), 0.0f, 360.0f);
				ImGui::DragInt3("Scale", (int*)&localTransform.ExtractScale(), 0.25f, 1.0f, 1000.0f);
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
		/*for (auto item = App->scene->gameobjects.begin(); item != App->scene->gameobjects.end(); ++item)
		{
			if ((*item)->selected && (*item)->children.size() > 0)
			{
				selectedGO = (*item);

				if ((ImGui::CollapsingHeader("Transform")))
				{
					ImGui::Checkbox("Active", &transform);
					static int vecpos[3] = { 0, 0, 0 }, vecrot[3] = { 0, 0, 0 }, vecscale[3] = { 1, 1, 1 };

					ImGui::DragInt3("Position", vecpos, 0.25f);
					ImGui::SliderInt3("Rotation", vecrot, 0.0f, 360.0f);
					ImGui::DragInt3("Scale", vecscale, 0.25f, 1.0f, 1000.0f);

					ImGuiIO& io = ImGui::GetIO();
					io.WantCaptureKeyboard;
				}

				if ((ImGui::CollapsingHeader("Mesh")) && (*item)->components.size() > 0)
				{
					meshes = new ComponentMesh();
					for (auto item = selectedGO->children.begin(); item != selectedGO->children.end(); ++item)
					{
						ComponentMesh* auxmesh = (ComponentMesh*)(*item)->GetComponent(MESH);
						meshes->num_vertices += auxmesh->num_vertices;
					}

					ImGui::Text("Number of vertices: %d", meshes->num_vertices);
					ImGui::Text("Number of children: %d", selectedGO->children.size());
				}

				if ((ImGui::CollapsingHeader("Texture")))
				{
					materials = new ComponentMaterial();
					for (auto item = selectedGO->children.begin(); item != selectedGO->children.end(); ++item)
					{
						ComponentMaterial* auxmaterial = (ComponentMaterial*)(*item)->GetComponent(MATERIAL);
						materials->textureID = auxmaterial->textureID;
						materials->path = auxmaterial->path;
						materials->width = auxmaterial->width;
						materials->height = auxmaterial->height;
						materials->depth = auxmaterial->depth;
						materials->BPP = auxmaterial->BPP;
					}
					ImGui::Text("Texture Width: %.01f", materials->width);
					ImGui::Text("Texture Height: %.01f", materials->height);
					ImGui::Text("Path: MotoresJuan/Game/%s", materials->path);
					float panelWidth = ImGui::GetWindowContentRegionWidth();
					float conversionFactor = panelWidth / materials->width;
					ImVec2 imageSize = { materials->height * conversionFactor, panelWidth };
					ImGui::Image((ImTextureID)materials->textureID, imageSize);
				}
			}

			if ((*item)->selected && (*item)->children.size() == 0)
			{
				selectedGO = (*item);

				if ((ImGui::CollapsingHeader("Transform")))
				{
					ImGui::Checkbox("Active", &transform);
					static int vecpos[3] = { 0, 0, 0 }, vecrot[3] = { 0, 0, 0 }, vecscale[3] = { 1, 1, 1 };

					ImGui::DragInt3("Position", vecpos, 0.25f);
					ImGui::SliderInt3("Rotation", vecrot, 0.0f, 360.0f);
					ImGui::DragInt3("Scale", vecscale, 0.25f, 1.0f, 1000.0f);

					for (int i = 0; i < 3; ++i) { meshPos[i] = vecpos[i]; }

					ImGuiIO& io = ImGui::GetIO();
					io.WantCaptureKeyboard;
				}

				if ((ImGui::CollapsingHeader("Mesh")) && (*item)->components.size() > 0)
				{
					meshes = (ComponentMesh*)selectedGO->GetComponent(MESH);

					ImGui::Text("Number of vertices: %d", meshes->num_vertices);
					ImGui::Text("Number of children: %d", selectedGO->children.size());
				}

				if ((ImGui::CollapsingHeader("Texture")))
				{
					ImGui::Text("Texture Width: %.01f", App->loader->TextureSize.x);
					ImGui::Text("Texture Height: %.01f", App->loader->TextureSize.y);
					ImGui::Text("Path: MotoresJuan/Game/%s", App->loader->path.c_str());
				}
			}
		}*/
	}
	ImGui::End();
}
