#include "Application.h"
#include "PanelShaderEditor.h"

#include "ModuleGUI.h"
#include "ModuleShaders.h"
#include "ModuleFileSystem.h"

PanelShaderEditor::PanelShaderEditor(const char* name) : Panel(name)
{
	editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
	shaderPath = "";
	shader = nullptr;
}


PanelShaderEditor::~PanelShaderEditor()
{
}

void PanelShaderEditor::Draw()
{
	auto cpos = editor.GetCursorPosition();

	ImGui::Begin("Shader Editor", &active, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				if (shader)
				{
					std::string backupCode;
					backupCode = shader->code;
					auto textToSave = editor.GetText();
					shader->code = textToSave;

					if (App->shaders->CompileShader(shader))
					{
						App->fileSystem->writeFile(shader->path.c_str(), textToSave.c_str(), textToSave.size(), true);
						App->shaders->UpdateShaders();
					}
					else
					{
						shader->code = backupCode;
					}
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::BeginChild("Editor", ImVec2(500, 600), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), shaderPath.c_str());

	editor.Render("ShaderEditor");

	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild("Shader Selector");
	ImGui::Text("Select Shader:");
	std::string shaderName;
	if (shader)
	{
		shaderName = shader->name;
	}
	else
	{
		shaderName = "none";
	}
	if (ImGui::BeginCombo("shaders", shaderName.c_str()))
	{
		for (int i = 0; i < App->shaders->shaders.size() + 1; ++i)
		{
			bool selected = false;
			std::string aux_name;
			if (i == App->shaders->shaders.size())
			{
				aux_name = "none";
				if (ImGui::Selectable(aux_name.c_str(), &selected))
				{
					shader = nullptr;
					editor.SetText("");
				}
				continue;
			}

			aux_name = App->shaders->shaders[i]->name;

			if (ImGui::Selectable(aux_name.c_str(), &selected))
			{
				shader = App->shaders->shaders[i];
				editor.SetText(App->shaders->shaders[i]->code);
			}
		}
		ImGui::EndCombo();
	}
	ImGui::EndChild();

	ImGui::End();
}