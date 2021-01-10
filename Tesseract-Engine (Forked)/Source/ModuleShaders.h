#ifndef MODULE_SHADERS_H
#define MODULE_SHADERS_H

#include "Module.h"
#include "Uniform.h"
#include "ResourceShader.h"
#include <string>
#include <vector>


class ModuleShaders : public Module
{
public:
	ModuleShaders(bool start_enabled = true);
	~ModuleShaders();

	//bool Init();
	bool Start();
	//bool Update();
	//bool CleanUp();

	Shader* AddShader(std::string path);
	ResourceShader* AddShaderProgram(std::string name, std::string vertexPath, std::string fragmentPath);

	bool CompileShader(Shader* shader);
	bool CompileShaderProgram(ResourceShader* shaderProgram);

	void UpdateShaders();

	uint GetShader(std::string name);
	ResourceShader* GetShaderFromID(uint ID);

public:
	std::vector<ResourceShader*> shaderPrograms;
	std::vector<Shader*> shaders;
};
#endif // !MODULE_SHADERS_H
