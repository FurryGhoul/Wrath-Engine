#ifndef __RESOURCESHADER_H__
#define __RESOURCESHADER_H__

#include "Resource.h"
#include "Uniform.h"

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

class ResourceShader : public Resource
{
public:

	ResourceShader(uint UID, ResType type);
	~ResourceShader();

	static void setImportDefaults(JSON_Value& importSettings);

	bool LoadInMemory();
	bool UnloadFromMemory();

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

public:

	std::string name;
	int ID = -1;
	Shader* vertexShader;
	Shader* fragmentShader;

	std::string vertexPath;
	std::string fragmentPath;

	std::vector<Uniform*> uniforms;
};
#endif // !__RESOURCESHADER_H__
