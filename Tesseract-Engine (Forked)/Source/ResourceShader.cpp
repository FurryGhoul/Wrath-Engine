#include "ResourceShader.h"

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
	return false;
}

bool ResourceShader::UnloadFromMemory()
{
	return false;
}
