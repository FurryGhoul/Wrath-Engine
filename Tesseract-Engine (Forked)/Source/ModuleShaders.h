#ifndef MODULE_SHADERS_H
#define MODULE_SHADERS_H

#include "Module.h"
#include "Uniform.h"
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
	std::string path;
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

	std::vector<Uniform*> uniforms;

	void AddUniform(UniformType type, std::string name)
	{
		Uniform* newUniform = new Uniform(name, type);
		uniforms.push_back(newUniform);
	}

	void AddUniformInt(UniformType type, std::string name, int value)
	{
		UniformInt* newInt = new UniformInt(name, type, value);
		uniforms.push_back(newInt);
	}

	void AddUniformFloat(UniformType type, std::string name, float value)
	{
		UniformFloat* newFloat = new UniformFloat(name, type, value);
		uniforms.push_back(newFloat);
	}

	void AddUniformMat4(UniformType type, std::string name, float4x4 value)
	{
		UniformMat4* newMat4 = new UniformMat4(name, type, value);
		uniforms.push_back(newMat4);
	}

	void AddUniformVec2(UniformType type, std::string name, float2 value)
	{
		UniformVec2* newVec2 = new UniformVec2(name, type, value);
		uniforms.push_back(newVec2);
	}

	void AddUniformVec3(UniformType type, std::string name, float3 value)
	{
		UniformVec3* newVec3 = new UniformVec3(name, type, value);
		uniforms.push_back(newVec3);
	}

	void AddUniformVec4(UniformType type, std::string name, float4 value)
	{
		UniformVec4* newVec4 = new UniformVec4(name, type, value);
		uniforms.push_back(newVec4);
	}
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

	void UpdateShaders();

	uint GetShader(std::string name);
	ShaderProgram* GetShaderFromID(uint ID);

public:
	std::vector<ShaderProgram*> shaderPrograms;
	std::vector<Shader*> shaders;
};
#endif // !MODULE_SHADERS_H
