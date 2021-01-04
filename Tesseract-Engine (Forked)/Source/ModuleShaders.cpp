#include "Application.h"
#include "ModuleShaders.h"
#include "ModuleFileSystem.h"

ModuleShaders::ModuleShaders(bool start_enabled) : Module(start_enabled)
{

}

ModuleShaders::~ModuleShaders()
{

}

bool ModuleShaders::Start()
{
	bool ret = true;

	ShaderProgram* defaultShaderProgram = AddShaderProgram("default_shader", "Assets/Shaders/default_vertex_shader.vrtx", "Assets/Shaders/default_fragment_shader.frg");
	if (defaultShaderProgram)
	{
		shaderPrograms.push_back(defaultShaderProgram);
	}

	return ret;
}

Shader * ModuleShaders::AddShader(std::string path)
{
	Shader* newShader = new Shader();
	std::string filePath; 
	std::string filename;
	std::string extension;
	App->fileSystem->splitPath(path.c_str(), &filePath, &filename, &extension);

	newShader->name = filename;

	if (extension == "vrtx")
	{
		newShader->type = ShaderType::VERTEX;
	}
	if (extension == "frg")
	{
		newShader->type = ShaderType::FRAGMENT;
	}

	newShader->code = App->fileSystem->ImportShaders(path.c_str());

	if (CompileShader(newShader) == false)
	{
		return nullptr;
	}

	return newShader;
}

ShaderProgram * ModuleShaders::AddShaderProgram(std::string name, std::string vertexPath, std::string fragmentPath)
{
	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->name = name; 
	shaderProgram->vertexShader = AddShader(vertexPath);
	shaderProgram->fragmentShader = AddShader(fragmentPath);

	CompileShaderProgram(shaderProgram);

	return shaderProgram;
}

bool ModuleShaders::CompileShader(Shader * shader)
{
	bool ret = false;

	if (shader->type == ShaderType::VERTEX)
	{
		shader->ID = glCreateShader(GL_VERTEX_SHADER);
	}
	if (shader->type == ShaderType::FRAGMENT)
	{
		shader->ID = glCreateShader(GL_FRAGMENT_SHADER);
	}

	/*int size = strlen(shader->code.c_str());
	++size;

	char* buffer = new char[size];
	strcpy(buffer, shader->code.c_str());*/

	const char* shaderCode = shader->code.c_str();
	glShaderSource(shader->ID, 1, &shaderCode, NULL);
	glCompileShader(shader->ID);

	int success;
	char infoLog[512];
	glGetShaderiv(shader->ID, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(shader->ID, 512, NULL, infoLog);
		LOG("Shader compilation error: %s", infoLog);
		glDeleteShader(shader->ID);
	}
	else
	{
		LOG("Shader %s compiled successfully", shader->name.c_str());
		ret = true;
	}

	return ret;
}

bool ModuleShaders::CompileShaderProgram(ShaderProgram * shaderProgram)
{
	bool ret = false;

	if (shaderProgram->fragmentShader && shaderProgram->vertexShader)
	{
		int success;
		char infoLog[512];

		shaderProgram->ID = glCreateProgram();
		glAttachShader(shaderProgram->ID, shaderProgram->vertexShader->ID);
		glAttachShader(shaderProgram->ID, shaderProgram->fragmentShader->ID);
		glLinkProgram(shaderProgram->ID);
		glGetProgramiv(shaderProgram->ID, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shaderProgram->ID, 512, NULL, infoLog);
			LOG("Shader link error: %s", infoLog);
		}

		ret = true;
	}

	return ret;
}
