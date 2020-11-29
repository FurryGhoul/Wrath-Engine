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

bool ModuleLoader::Init(const JSON_Object& config) 
{ 
	return true; 
}

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

	aiReleaseImport(scene);

	return true;
}

bool ModuleLoader::RecursiveLoadChildren(const aiScene* scene, const aiNode* node, GameObject* parent, string path)
{
	GameObject* GO = new GameObject(parent, node->mName.C_Str());
	App->scene->root->children.push_back(GO);
	App->scene->gameobjects.push_back(GO);

	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scale, rotation, translation);

	float3 new_translation = { translation.x, translation.y, translation.z };
	float3 new_scale = { scale.x, scale.y, scale.z };
	Quat new_rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

	float4x4 new_local = float4x4::FromTRS(new_translation, new_rotation, new_scale);

	ComponentTransform* transform = (ComponentTransform*)GO->AddComponent(TRANSFORM);
	transform->SetTransformation(new_local);

	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	
		GameObject* newGO = new GameObject(GO, mesh->mName.C_Str());
		GO->children.push_back(newGO);

		ComponentMesh* new_mesh = (ComponentMesh*)newGO->AddComponent(MESH);

		ComponentTransform* new_transform = (ComponentTransform*)newGO->AddComponent(TRANSFORM);

		new_transform->SetLocalMatrix(transform->GetLocalMatrix());
		new_transform->SetGlobalMatrix(transform->GetGlobalMatrix());

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D meshColour(1.f, 1.f, 1.f);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, meshColour);

		ImportMesh(mesh, new_mesh, path, meshColour);

		/*new_mesh->num_vertices = mesh->mNumVertices;
		new_mesh->vertices = new float[new_mesh->num_vertices * 3];
		memcpy(new_mesh->vertices, mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
		LOG("New mesh with %d vertices", new_mesh->num_vertices);

		new_mesh->normals = new float[new_mesh->num_vertices * 3];
		memcpy(new_mesh->normals, mesh->mNormals, sizeof(float) * new_mesh->num_vertices * 3);*/

		//if (mesh->HasTextureCoords(0))
		//{
		//	new_mesh->texture_coords = new float[mesh->mNumVertices * 2];

		//	for (uint j = 0; j < mesh->mNumVertices * 2; j += 2)
		//	{
		//		new_mesh->texture_coords[j] = mesh->mTextureCoords[0][j / 2].x;
		//		new_mesh->texture_coords[j + 1] = mesh->mTextureCoords[0][j / 2].y;
		//	}
		//}

		if (scene->HasMaterials())
		{
			ComponentMaterial* new_material = (ComponentMaterial*)newGO->AddComponent(MATERIAL);

			aiString material_path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);

			string material_name = material_path.C_Str();

			for (int i = material_name.size() - 1; i >= 0; i--)
			{
				if (material_name[i] == '/' || material_name[i] == '\\')
				{
					material_name = material_name.substr(i+1, material_name.length()-1);
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
			new_material->textureID = Texturing(new_material, materialFullPath.c_str());
			new_material->name = material_name;


			if (new_material->textureID == 0)
			{
				path = "Assets/Textures/" + material_name;
				new_material->textureID = Texturing(new_material, path.c_str());
				LOG("%s", path);
			}
		}

		if (mesh->HasFaces())
		{
			/*LOG("New mesh with %d face", mesh->mNumFaces);
			int faceIndex = 0;
			new_mesh->num_indices = mesh->mNumFaces * 3;
			new_mesh->indices = new uint[new_mesh->num_indices];
			for (uint i = 0; i < mesh->mNumFaces; ++i)
			{
				if (mesh->mFaces[i].mNumIndices != 3) 
				{ 
					LOG("WARNING, geometry face with != 3 indices!"); 
				}
				else 
				{ 
					memcpy(&new_mesh->indices[faceIndex * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint)); 
					faceIndex++;
				}
			}*/
			/*if (faceIndex > 0)
			{
				glGenBuffers(1, (GLuint*)&(new_mesh->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glGenBuffers(1, (GLuint*)&(new_mesh->id_texcoords));
				glBindBuffer(GL_TEXTURE_COORD_ARRAY, new_mesh->id_texcoords);
				glBufferData(GL_TEXTURE_COORD_ARRAY, sizeof(uint) * new_mesh->num_vertices * 2, new_mesh->texture_coords, GL_STATIC_DRAW);
				glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
			}*/

			App->scene->gameobjects.push_back(newGO);
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
	compMesh->num_vertices = mesh->mNumVertices;
	compMesh->vertices = new float[compMesh->num_vertices * 3];
	memcpy(compMesh->vertices, mesh->mVertices, sizeof(float) * compMesh->num_vertices * 3);
	LOG("New mesh with %d vertices", compMesh->num_vertices);

	compMesh->normals = new float[compMesh->num_vertices * 3];
	memcpy(compMesh->normals, mesh->mNormals, sizeof(float) * compMesh->num_vertices * 3);

	if (mesh->HasTextureCoords(0))
	{
		compMesh->texture_coords = new float[mesh->mNumVertices * 2];

		for (uint j = 0; j < mesh->mNumVertices * 2; j += 2)
		{
			compMesh->texture_coords[j] = mesh->mTextureCoords[0][j / 2].x;
			compMesh->texture_coords[j + 1] = mesh->mTextureCoords[0][j / 2].y;
		}
	}

	if (mesh->HasFaces())
	{
		LOG("New mesh with %d face", mesh->mNumFaces);
		int faceIndex = 0;
		compMesh->num_indices = mesh->mNumFaces * 3;
		compMesh->indices = new uint[compMesh->num_indices];
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
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
	}

	compMesh->colour = { colour.r, colour.g, colour.b, 1.f };

	SaveMesh(compMesh);

	return true;
}

bool ModuleLoader::SaveMesh(ComponentMesh* compMesh)
{
	//Vertices
	uint numVSize = sizeof(int);
	uint vSize = sizeof(float) * compMesh->num_vertices * 3;

	uint nSize = sizeof(float) * compMesh->num_vertices * 3;

	uint texCoordSize = sizeof(float) * compMesh->num_vertices * 2;

	//Indices
	uint numISize = sizeof(int);
	uint iSize = sizeof(uint) * compMesh->num_indices * 3;

	//Colour
	uint cSize = sizeof(float) * 4;

	//Saving Shit
	int totalSize = numVSize + vSize + nSize + texCoordSize + numISize + iSize + cSize;
	char* buffer = new char[totalSize];
	char* pointer = buffer; 

	memcpy(pointer, &numVSize, sizeof(int));
	pointer += sizeof(int);

	memcpy(pointer, &vSize, sizeof(float));
	pointer += sizeof(float);

	memcpy(pointer, &nSize, sizeof(float));
	pointer += sizeof(float);

	memcpy(pointer, &texCoordSize, sizeof(float));
	pointer += sizeof(float);

	memcpy(pointer, &numISize, sizeof(int));
	pointer += sizeof(int);

	memcpy(pointer, &iSize, sizeof(uint));
	pointer += sizeof(uint);

	memcpy(pointer, &cSize, sizeof(float));
	pointer += sizeof(float);

	FILE* fMesh;
	fMesh = fopen("Assets\Models\meshInfo.wrth", "w");
	fputs(buffer, fMesh);
	fclose(fMesh);

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

