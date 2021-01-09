#include "Application.h"
#include "ComponentShader.h"
#include "ModuleShaders.h"
#include "Uniform.h"

ComponentShader::ComponentShader(GameObject* gameobject, componentType type) : Component(gameobject, type)
{
	shaderID = App->shaders->GetShader("default_shader");
	name = "default_shader";
}

ComponentShader::~ComponentShader()
{
}

void ComponentShader::DrawInfo()
{
	if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		ShaderProgram* shaderProgram = App->shaders->GetShaderFromID(App->shaders->GetShader(name));
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

			for (int i = 0; i < shaderProgram->uniforms.size(); ++i)
			{
				if (shaderProgram->uniforms[i]->type == UniformType::INT)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::PushID("ShaderInt");
					ImGui::InputInt("", &((UniformInt*)shaderProgram->uniforms[i])->value, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::FLOAT)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					ImGui::PushID("ShaderFloat");
					ImGui::InputFloat("", &((UniformFloat*)shaderProgram->uniforms[i])->value, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::VEC2)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					float vector2[2] = { ((UniformVec2*)shaderProgram->uniforms[i])->value.x, ((UniformVec2*)shaderProgram->uniforms[i])->value.y };
					ImGui::PushID("ShaderVec2");
					ImGui::DragFloat2("", vector2, 1.f, 0, 100);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::VEC3)
				{
					ImGui::Text(shaderProgram->uniforms[i]->name.c_str());
					float vector3[3] = { ((UniformVec3*)shaderProgram->uniforms[i])->value.x, ((UniformVec3*)shaderProgram->uniforms[i])->value.y, ((UniformVec3*)shaderProgram->uniforms[i])->value.z };
					ImGui::PushID("ShaderVec3");
					ImGui::DragFloat3("", vector3, 1.f, 0, 100);
					ImGui::PopID();
				}
				else if (shaderProgram->uniforms[i]->type == UniformType::VEC4)
				{
					float vector4[4] = { ((UniformVec4*)shaderProgram->uniforms[i])->value.x, ((UniformVec4*)shaderProgram->uniforms[i])->value.y, ((UniformVec4*)shaderProgram->uniforms[i])->value.z, ((UniformVec4*)shaderProgram->uniforms[i])->value.w };
					ImGui::PushID("ShaderVec4");
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
}

void ComponentShader::Load(JSON_Value * component)
{
}
