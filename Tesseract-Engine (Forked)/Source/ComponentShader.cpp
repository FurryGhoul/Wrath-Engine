#include "Application.h"
#include "ComponentShader.h"
#include "ModuleShaders.h"
#include "ModuleResource.h"
#include "Uniform.h"

#include "Resource.h"
#include "ResourceShader.h"

ComponentShader::ComponentShader(GameObject* gameobject, componentType type) : Component(gameobject, type)
{
	name = "default_shader";
}

ComponentShader::~ComponentShader()
{
}

void ComponentShader::DrawInfo()
{
	if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		ResourceShader* shaderProgram = App->shaders->GetShaderFromID(App->shaders->GetShader(name));
		if (shaderProgram)
		{
			if (ImGui::BeginCombo("shaders", shaderProgram->name.c_str()))
			{
				for (int i = 0; i < App->shaders->shaderPrograms.size(); ++i)
				{
					bool selected = false;
					std::string aux_name;
					aux_name = App->shaders->shaderPrograms[i]->name;

					if (ImGui::Selectable(aux_name.c_str(), &selected))
					{
						name = App->shaders->shaderPrograms[i]->name;
					}
				}
				ImGui::EndCombo();
			}

			if (shaderProgram->uniforms.size() > 0)
			{
				ImGui::Text("Uniforms:");
				ImGui::Separator();
			}
			for (int i = 0; i < shaderProgram->uniforms.size(); ++i)
			{
				if (shaderProgram->uniforms[i]->type == UniformType::INT)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::SameLine();
					std::string valueName = shaderProgram->uniforms[i]->name + "ShaderInt";
					ImGui::PushID(valueName.c_str());
					ImGui::InputInt("", &((UniformInt*)shaderProgram->uniforms[i])->value, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::FLOAT)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::SameLine();
					std::string valueName = shaderProgram->uniforms[i]->name + "ShaderFloat";
					ImGui::PushID(valueName.c_str());
					ImGui::InputFloat("", &((UniformFloat*)shaderProgram->uniforms[i])->value, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::VEC2)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::SameLine();
					float vector2[2] = { ((UniformVec2*)shaderProgram->uniforms[i])->value.x, ((UniformVec2*)shaderProgram->uniforms[i])->value.y };
					std::string valueName = shaderProgram->uniforms[i]->name + "ShaderVec2";
					ImGui::PushID(valueName.c_str());
					ImGui::DragFloat2("", vector2, 1.f, 0, 100);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::VEC3)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::SameLine();
					float vector3[3] = { ((UniformVec3*)shaderProgram->uniforms[i])->value.x, ((UniformVec3*)shaderProgram->uniforms[i])->value.y, ((UniformVec3*)shaderProgram->uniforms[i])->value.z };
					std::string valueName = shaderProgram->uniforms[i]->name + "ShaderVec3";
					ImGui::PushID(valueName.c_str());
					ImGui::DragFloat3("", vector3, 1.f, 0, 100);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::VEC4)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::SameLine();
					float vector4[4] = { ((UniformVec4*)shaderProgram->uniforms[i])->value.x, ((UniformVec4*)shaderProgram->uniforms[i])->value.y, ((UniformVec4*)shaderProgram->uniforms[i])->value.z, ((UniformVec4*)shaderProgram->uniforms[i])->value.w };
					std::string valueName = shaderProgram->uniforms[i]->name + "ShaderVec4";
					ImGui::PushID(valueName.c_str());
					ImGui::DragFloat4("", vector4, 1.f, 0, 100);
					ImGui::PopID();
				}
				else
				{
					std::string text = shaderProgram->uniforms[i]->name + ": Predefined";
					ImGui::Text(text.c_str());
				}
			}
		}
	}
}

void ComponentShader::Save(JSON_Value * component) const
{
	JSON_Value* shader = component->createValue();

	shader->addInt("Type", type);
	//shader->addUint("UID", UID);
	shader->addString("shaderName", name.c_str());

	component->addValue("", shader);
}

void ComponentShader::Load(JSON_Value * component)
{
	name = component->getString("shaderName");
}
