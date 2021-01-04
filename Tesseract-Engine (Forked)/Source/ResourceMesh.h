#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__

#include "Resource.h"
#include "MathGeoLib/MathGeoLib.h"
#include <vector>

class ResourceMesh : public Resource
{
public:

	ResourceMesh(uint UID, ResType type);
	~ResourceMesh();

	static void setImportDefaults(JSON_Value& importSettings);

	void calculateNormals();

	bool LoadInMemory();
	bool LoadMesh();
	bool UnloadFromMemory();

public:

	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint num_normals = 0;
	float* normals = nullptr;

	vec Color = { 1.0f,1.0f,1.0f };

	uint num_texCoords = 0;
	float* texCoords = nullptr;

	std::vector<float> faceNormals;

	AABB boundingBox;

};

#endif // !__RESOURCEMESH_H__
