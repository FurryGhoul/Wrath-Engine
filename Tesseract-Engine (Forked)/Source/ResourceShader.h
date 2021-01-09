#ifndef __RESOURCESHADER_H__
#define __RESOURCESHADER_H__

#include "Resource.h"

class Shader;

class ResourceShader : public Resource
{
public:

	ResourceShader(uint UID, ResType type);
	~ResourceShader();

	static void setImportDefaults(JSON_Value& importSettings);

	bool LoadInMemory();
	bool UnloadFromMemory();

public:
	Shader* shader;
};
#endif // !__RESOURCESHADER_H__
