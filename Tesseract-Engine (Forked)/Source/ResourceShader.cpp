#include "ResourceShader.h"

#include "ModuleShaders.h"

ResourceShader::ResourceShader(uint UID, ResType type):Resource(UID, type)
{
}

ResourceShader::~ResourceShader()
{
}

void ResourceShader::setImportDefaults(JSON_Value & importSettings)
{
}

bool ResourceShader::LoadInMemory()
{
	vertexShader = App->shaders->AddShader(vertexPath);
	fragmentShader = App->shaders->AddShader(fragmentPath);
	if (vertexShader && fragmentShader)
	{
		if (App->shaders->CompileShaderProgram(this) == true)
		{
			loaded++;
		}
	}
	return true;
}

bool ResourceShader::UnloadFromMemory()
{
	glDeleteProgram(ID);
	glDeleteShader(vertexShader->ID);
	glDeleteShader(fragmentShader->ID);
	return true;
}
