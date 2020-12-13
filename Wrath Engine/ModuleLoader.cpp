#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "DevIL/includes/il.h"
#include "DevIL/includes/ilu.h"
#include "DevIL/includes/ilut.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment( lib, "DevIL/libx86/DevIL.lib")
#pragma comment( lib, "DevIL/libx86/ILU.lib")
#pragma comment( lib, "DevIL/libx86/ILUT.lib")

typedef unsigned int uint;
using namespace Assimp;

ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	glEnable(GL_TEXTURE_2D);
}

ModuleLoader::~ModuleLoader() {}

bool ModuleLoader::Start() 
{ 
	return true; 
}

update_status ModuleLoader::PreUpdate(float dt) 
{ 
	return UPDATE_CONTINUE; 
}

update_status ModuleLoader::Update(float dt) 
{ 
	return UPDATE_CONTINUE; 
}

update_status ModuleLoader::PostUpdate(float dt) 
{ 
	return UPDATE_CONTINUE; 
}

bool ModuleLoader::OwnImport(const string& file)
{
	//App->scene->LoadScene(file);
	return true;
}

bool ModuleLoader::Import(const string& pFile)
{
	string file_path = pFile;
	path = pFile.c_str();
	const aiScene* scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	const aiNode* node = scene->mRootNode;

	if (scene != nullptr && scene->HasMeshes())
	{
		RecursiveLoadChildren(scene, node, nullptr, path);
	}
	else { LOG("Error loading scene %s", file_path); }

	App->scene->root->RecalculateAABB();

	aiReleaseImport(scene);

	return true;
}

bool ModuleLoader::RecursiveLoadChildren(const aiScene* scene, const aiNode* node, GameObject* parent, string path)
{
	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, translation);

	float3 new_translation = { translation.x, translation.y, translation.z };
	float3 new_scale = { scale.x, scale.y, scale.z };
	Quat new_rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

	std::string name = node->mName.C_Str();
	static const char* transformNodes[5] = { "$AssimpFbx$_PreRotation", "$AssimpFbx$_Rotation", "$AssimpFbx$_PostRotation","$AssimpFbx$_Scaling", "$AssimpFbx$_Translation" };

	for (int i = 0; i < 5; i++)
	{
		if (name.find(transformNodes[i]) != string::npos && node->mNumChildren > 0)
		{
			node = node->mChildren[0];

			node->mTransformation.Decompose(scale, rotation, translation);

			new_translation += { translation.x, translation.y, translation.z };
			new_scale = { scale.x * new_scale.x, scale.y * new_scale.y, scale.z * new_scale.z };
			new_rotation = new_rotation * Quat(rotation.x, rotation.y, rotation.z, rotation.w);

			name = node->mName.C_Str();
			i = -1;
		}
	}

	GameObject* GO = new GameObject(parent, node->mName.C_Str());
	if (parent == nullptr)
	{
		App->scene->root->children.push_back(GO);
	}
	App->scene->gameobjects.push_back(GO);

	float4x4 new_local = float4x4::FromTRS(new_translation, new_rotation, new_scale);

	ComponentTransform* transform = (ComponentTransform*)GO->AddComponent(TRANSFORM);
	transform->SetTransformation(new_local);
	transform->compTranslation = new_translation;
	transform->compScale = new_scale;
	transform->compRotation = new_rotation;

	int failCount = 0;
	
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	
		GameObject* newGO = GO;

		if (i > failCount)
		{
			newGO = new GameObject();
			newGO->parent = parent;
			newGO->name = GO->name + "_" + std::to_string(i);

			ComponentTransform* childTransform = (ComponentTransform*)newGO->AddComponent(TRANSFORM);
			childTransform->SetTransformation(new_local);
			childTransform->compTranslation = new_translation;
			childTransform->compScale = new_scale;
			childTransform->compRotation = new_rotation;
		}

		ComponentMesh* new_mesh = (ComponentMesh*)newGO->AddComponent(MESH);

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D meshColour(1.f, 1.f, 1.f);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, meshColour);

		if (ImportMesh(mesh, new_mesh, path, meshColour))
		{
			if (scene->HasMaterials())
			{
				ComponentMaterial* new_material = (ComponentMaterial*)newGO->AddComponent(MATERIAL);

				ImportMaterial(material, new_material, path);
			}
		}
		else
		{
			failCount++;
		}
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		RecursiveLoadChildren(scene, node->mChildren[i], GO, path);
	}

	return true;
}

bool ModuleLoader::ImportMesh(const aiMesh* mesh, ComponentMesh* compMesh, string path, aiColor3D colour)
{
	bool ret = false;

	compMesh->num_vertices = mesh->mNumVertices;
	compMesh->vertices = new float[compMesh->num_vertices * 3];
	memcpy(compMesh->vertices, mesh->mVertices, sizeof(float) * compMesh->num_vertices * 3);
	LOG("New mesh with %d vertices", compMesh->num_vertices);

	compMesh->normals = new float[compMesh->num_vertices * 3];
	memcpy(compMesh->normals, mesh->mNormals, sizeof(float) * compMesh->num_vertices * 3);

	if (mesh->HasFaces())
	{
		ret = true;
		LOG("New mesh with %d face", mesh->mNumFaces);

		if (mesh->HasTextureCoords(0))
		{
			compMesh->num_texture = mesh->mNumVertices;
			compMesh->texture_coords = new float[compMesh->num_texture * 2];

			for (uint j = 0; j < compMesh->num_texture * 2; j += 2)
			{
				compMesh->texture_coords[j] = mesh->mTextureCoords[0][j / 2].x;
				compMesh->texture_coords[j + 1] = mesh->mTextureCoords[0][j / 2].y;
			}
		}

		int faceIndex = 0;
		compMesh->num_indices = mesh->mNumFaces * 3;
		compMesh->indices = new uint[compMesh->num_indices];
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
				ret = false;
				break;
			}
			else
			{
				memcpy(&compMesh->indices[faceIndex * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				faceIndex++;
			}
		}

		if (faceIndex > 0)
		{
			glGenBuffers(1, (GLuint*)&(compMesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compMesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * compMesh->num_indices, compMesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&(compMesh->id_texcoords));
			glBindBuffer(GL_TEXTURE_COORD_ARRAY, compMesh->id_texcoords);
			glBufferData(GL_TEXTURE_COORD_ARRAY, sizeof(uint) * compMesh->num_vertices * 2, compMesh->texture_coords, GL_STATIC_DRAW);
			glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}

	compMesh->colour = { colour.r, colour.g, colour.b, 1.f };

	if (ret)
	{
		SaveMesh(compMesh);
	}

	return ret;
}

bool ModuleLoader::ImportMaterial(const aiMaterial* material, ComponentMaterial* compMat, string path)
{
	bool ret;

	aiString material_path;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);

	string material_name = material_path.C_Str();

	for (int i = material_name.size() - 1; i >= 0; i--)
	{
		if (material_name[i] == '/' || material_name[i] == '\\')
		{
			material_name = material_name.substr(i + 1, material_name.length() - 1);
			break;
		}
	}

	string materialFullPath = path;

	for (int i = materialFullPath.size() - 1; i >= 0; i--)
	{
		if (materialFullPath[i] == '/' || materialFullPath[i] == '\\')
		{
			break;
		}
		else
		{
			materialFullPath.pop_back();
		}
	}

	materialFullPath += material_name;
	compMat->textureID = Texturing(compMat, materialFullPath.c_str());
	compMat->name = material_name;


	if (compMat->textureID == 0)
	{
		path = "Assets/Textures/" + material_name;
		compMat->textureID = Texturing(compMat, path.c_str());
		LOG("%s", path);
	}

	//SaveMaterial(compMat);

	return true;
}

bool ModuleLoader::SaveMaterial(ComponentMaterial* compMat)
{
	return true;
}

bool ModuleLoader::SaveMesh(ComponentMesh* compMesh)
{
	uint bytes;
	uint ranges[4] = { compMesh->num_vertices, compMesh->num_normals, compMesh->num_texture, compMesh->num_indices};

	//Vertices
	uint vSize = sizeof(float) * compMesh->num_vertices * 3;

	uint nSize = sizeof(float) * compMesh->num_vertices * 3;

	uint texCoordSize = 0;
	if (compMesh->texture_coords)
	{
		texCoordSize = sizeof(float) * compMesh->num_vertices * 2;
	}

	//Indices
	uint iSize = sizeof(uint) * compMesh->num_indices;

	//Saving Shit
	uint totalSize = sizeof(ranges) + vSize + nSize + texCoordSize + iSize + sizeof(float) * 3;
	char* buffer = new char[totalSize];
	char* pointer = buffer; 

	bytes = sizeof(ranges);
	memcpy(pointer, ranges, bytes);
	pointer += bytes;

	bytes = vSize;
	memcpy(pointer, compMesh->vertices, bytes);
	pointer += bytes;

	bytes = nSize;
	memcpy(pointer, compMesh->normals, bytes);
	pointer += bytes;

	if (compMesh->texture_coords)
	{
		bytes = texCoordSize;
		memcpy(pointer, compMesh->texture_coords, bytes);
		pointer += bytes;
	}

	bytes = iSize;
	memcpy(pointer, compMesh->indices, bytes);
	pointer += bytes;

	float cSize[3] = { compMesh->colour.x, compMesh->colour.y, compMesh->colour.z };
	bytes = sizeof(float) * 3;
	memcpy(pointer, cSize, bytes);

	string pathName = "Library\\Mesh\\";
	pathName += compMesh->parent->name.c_str();
	pathName += ".wrth";

	FILE* fMesh;
	fMesh = fopen(pathName.c_str(), "w");
	if (fMesh != NULL)
	{
		fwrite(buffer, sizeof(char), totalSize, fMesh);
		fclose(fMesh);
	}

	RELEASE_ARRAY(buffer);

	return true;
}

uint ModuleLoader::Texturing(ComponentMaterial* material, const char* file_name)
{
	/*if (material == nullptr)
	{

	}*/
	ILuint imageID = 0;
	ILenum error;
	ILinfo ImageInfo;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(file_name))
	{
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		iluGetImageInfo(&ImageInfo);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &material->textureID);
		glBindTexture(GL_TEXTURE_2D, material->textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		uint texture_width = ImageInfo.Width; uint texture_height = ImageInfo.Height;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);

		material->path = file_name;
		material->width = ImageInfo.Width;
		material->height = ImageInfo.Height;
		material->depth = ImageInfo.Depth;
		material->BPP = ImageInfo.Bpp;

		LOG("Texture loaded succesfuly!")

			TextureSize = { (float)texture_width, (float)texture_height };
	}

	else
	{
		while (error = ilGetError()) 
		{ 
			LOG("Error  %d: %s loading %s", error, iluErrorString(error), file_name); 
		}
		return 0;
	}

	ilDeleteImages(1, &imageID);
	return material->textureID;
}



bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

