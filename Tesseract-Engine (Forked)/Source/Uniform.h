#ifndef __UNIFORM_H__
#define __UNIFORM_H__

#include "Application.h"
#include <string>

enum class UniformType
{
	INT,
	FLOAT,
	MAT4,
	VEC2,
	VEC3,
	VEC4,
	PROJECTION,
	VIEW,
	GLOBAL,
	TIME,
	TEXTURE
};

class Uniform
{
public:
	Uniform(std::string name, UniformType type) { this->name = name; this->type = type; }

	virtual void LoadToGPU(int shader) {}
	std::string name;
	int location;

	UniformType type;
};

class UniformInt : public Uniform
{
public:
	UniformInt(std::string name, UniformType type, int value) : Uniform(name, type) { this->value = value; }
	void LoadToGPU(int shader) 
	{
		location = glGetUniformLocation(shader, name.c_str());
		glUniform1i(location, value);
	}
	int value;
};

class UniformFloat : public Uniform
{
public:
	UniformFloat(std::string name, UniformType type, float value) : Uniform(name, type) { this->value = value; }
	void LoadToGPU(int shader)
	{
		location = glGetUniformLocation(shader, name.c_str());
		glUniform1f(location, value);
	}
	float value;
};

class UniformMat4 : public Uniform
{
public:
	UniformMat4(std::string name, UniformType type, float4x4 value) : Uniform(name, type) { this->value = value; }
	void LoadToGPU(int shader)
	{
		location = glGetUniformLocation(shader, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, (float*)value.v);
	}
	float4x4 value;
};

class UniformVec2 : public Uniform
{
public:
	UniformVec2(std::string name, UniformType type, float2 value) : Uniform(name, type) { this->value = value; }
	void LoadToGPU(int shader)
	{
		location = glGetUniformLocation(shader, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	float2 value;
};

class UniformVec3 : public Uniform
{
public:
	UniformVec3(std::string name, UniformType type, float3 value) : Uniform(name, type) { this->value = value; }
	void LoadToGPU(int shader)
	{
		location = glGetUniformLocation(shader, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	float3 value;
};

class UniformVec4 : public Uniform
{
public:
	UniformVec4(std::string name, UniformType type, float4 value) : Uniform(name, type) { this->value = value; }
	void LoadToGPU(int shader)
	{
		location = glGetUniformLocation(shader, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	float4 value;
};
#endif // !__UNIFORM_H__