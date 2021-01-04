#ifndef MODULE_SHADERS_H
#define MODULE_SHADERS_H

#include "Module.h"
#include <string>
#include <vector>

enum class ShaderType
{
	NONE,
	VERTEX,
	FRAGMENT
};

struct Shader
{
	std::string name;
	std::string code;
	ShaderType type;
	int ID = -1;
};

struct ShaderProgram
{
	std::string name;
	int ID = -1;
	Shader* vertexShader;
	Shader* fragmentShader;
};

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
	ShaderProgram* AddShaderProgram(std::string name, std::string vertexPath, std::string fragmentPath);

	bool CompileShader(Shader* shader);
	bool CompileShaderProgram(ShaderProgram* shaderProgram);

public:
	std::vector<ShaderProgram*> shaderPrograms;
};
#endif // !MODULE_SHADERS_H
