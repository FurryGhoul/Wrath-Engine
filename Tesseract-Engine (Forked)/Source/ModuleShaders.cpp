#include "Application.h"
#include "ModuleShaders.h"
#include "ModuleFileSystem.h"
#include "ModuleResource.h"
#include "ResourceShader.h"

ModuleShaders::ModuleShaders(bool start_enabled) : Module(start_enabled)
{

}

ModuleShaders::~ModuleShaders()
{

}

bool ModuleShaders::Start()
{
	bool ret = true;

	ResourceShader* defaultShaderProgram = AddShaderProgram("default_shader", "Assets/Shaders/default_vertex_shader.vrtx", "Assets/Shaders/default_fragment_shader.frg");
	defaultShaderProgram->LoadInMemory();
	if (defaultShaderProgram->IsLoaded())
	{
		shaderPrograms.push_back(defaultShaderProgram);
		defaultShaderProgram->AddUniform(UniformType::GLOBAL, "model_matrix");
		defaultShaderProgram->AddUniform(UniformType::VIEW, "view");
		defaultShaderProgram->AddUniform(UniformType::PROJECTION, "projection");
		defaultShaderProgram->AddUniform(UniformType::TEXTURE, "hasTexture");
	}

	ResourceShader* waterShaderProgram = AddShaderProgram("water_shader", "Assets/Shaders/water_vertex_shader.vrtx", "Assets/Shaders/water_fragment_shader.frg");
	waterShaderProgram->LoadInMemory();
	if (waterShaderProgram->IsLoaded())
	{
		shaderPrograms.push_back(waterShaderProgram);
		waterShaderProgram->AddUniformFloat(UniformType::FLOAT, "frequency", 5.0f);
		waterShaderProgram->AddUniformFloat(UniformType::FLOAT, "height", 2.0f);
		waterShaderProgram->AddUniform(UniformType::GLOBAL, "model_matrix");
		waterShaderProgram->AddUniform(UniformType::VIEW, "view");
		waterShaderProgram->AddUniform(UniformType::PROJECTION, "projection");
		waterShaderProgram->AddUniform(UniformType::TIME, "time");
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
	newShader->path = path;

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

	shaders.push_back(newShader);

	return newShader;
}

ResourceShader * ModuleShaders::AddShaderProgram(std::string name, std::string vertexPath, std::string fragmentPath)
{
	ResourceShader* shaderProgram = (ResourceShader*)App->resources->AddResource(R_SHADER);
	shaderProgram->name = name; 

	shaderProgram->vertexPath = vertexPath;
	shaderProgram->fragmentPath = fragmentPath;

	return shaderProgram;
}

bool ModuleShaders::CompileShader(Shader * shader)
{
	bool ret = false;

	if (shader->ID != 0)
	{
		glDeleteShader(shader->ID);
	}

	if (shader->type == ShaderType::VERTEX)
	{
		shader->ID = glCreateShader(GL_VERTEX_SHADER);
	}
	if (shader->type == ShaderType::FRAGMENT)
	{
		shader->ID = glCreateShader(GL_FRAGMENT_SHADER);
	}

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

bool ModuleShaders::CompileShaderProgram(ResourceShader * shaderProgram)
{
	bool ret = false;

	if (shaderProgram->ID != 0)
	{
		glDeleteProgram(shaderProgram->ID);
	}

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

void ModuleShaders::UpdateShaders()
{
	for (int i = 0; i < shaderPrograms.size(); ++i)
	{
		CompileShaderProgram(shaderPrograms[i]);
	}
}

uint ModuleShaders::GetShader(std::string name)
{
	for (int i = 0; i < shaderPrograms.size(); ++i)
	{
		if (shaderPrograms[i]->name == name)
		{
			return shaderPrograms[i]->ID;
		}
	}
	return 0;
}

ResourceShader * ModuleShaders::GetShaderFromID(uint ID)
{
	for (int i = 0; i < shaderPrograms.size(); ++i)
	{
		if (shaderPrograms[i]->ID == ID)
		{
			return shaderPrograms[i];
		}
	}
	return nullptr;
}
