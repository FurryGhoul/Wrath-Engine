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
		if (ImGui::Button("Create Camera"))
		{
			App->scene->AddCamera();
		}
		for (auto item = App->scene->root->children.begin(); item != App->scene->root->children.end(); ++item)
		{
			if (CreateTreeHierarchy((*item)))
			{
				item = App->scene->root->children.begin();
			}
		}

		ImGui::Separator();
		static int slices = 30; static int stacks = 30;
	}
	ImGui::End();
}

bool UI_Hierarchy::CreateTreeHierarchy(GameObject* gameObject)
{
	bool ret = false;

	uint flags;
	flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (gameObject->selected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (gameObject->children.size() <= 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	bool opened = ImGui::TreeNodeEx(gameObject->name.c_str(), flags);

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("ReParenting", &gameObject->uuid, sizeof(uint));
		ImGui::Text(gameObject->name.c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ReParenting", ImGuiDragDropFlags_SourceAllowNullID))
		{
			GameObject* dragGO = App->scene->GetGameObjectFromUUID(*(uint*)payload->Data);
			if (dragGO != nullptr)
			{
				dragGO->ReParenting(gameObject);
				ret = true;
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginPopupContextItem((gameObject->name + "rightClick").c_str(), 1))
	{
		if (ImGui::Button("Delete"))
		{
			AddToDelete(gameObject);
			selectedGO = nullptr;
		}
		ImGui::EndPopup();
	}

	if (ImGui::IsItemClicked(0))
	{
		if (selectedGO != gameObject)
		{
			if (selectedGO != nullptr)
			{
				selectedGO->selected = false;
			}
			selectedGO = gameObject;
			selectedGO->selected = true;
		}
	}

	if (opened)
	{
		if (gameObject->children.size() > 0)
		{
			for (auto item = gameObject->children.begin(); item != gameObject->children.end(); ++item)
			{
				if (CreateTreeHierarchy((*item)))
				{
					item = gameObject->children.begin();
				}
			}
		}
		ImGui::TreePop();
	}

	return ret;
}

void UI_Hierarchy::AddToDelete(GameObject* objectToDelete)
{
	objectToDelete->erase = true;

	for (auto item = objectToDelete->children.begin(); item != objectToDelete->children.end(); ++item)
	{
		AddToDelete((*item));
	}
}
