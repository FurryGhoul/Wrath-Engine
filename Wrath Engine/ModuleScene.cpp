#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include "Parson/parson.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading scene");

	root = new GameObject(nullptr, "Root");

	return true;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	for (auto item = gameobjects.begin(); item != gameobjects.end(); ++item)
	{
		(*item)->Update();
	}

	for (auto item = gameobjects.begin(); item != gameobjects.end(); ++item)
	{
		if ((*item)->erase)
		{
			DeleteGameObject((*item));
			item = gameobjects.begin();
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleScene::SaveScene(string fileName)
{
	JSON_Value* sceneValue = json_value_init_object();
	JSON_Value* sceneObjects = json_value_init_array();
	json_object_set_value(json_object(sceneValue), "Game Objects", sceneObjects);

	for (int i = 0; i < gameobjects.size(); ++i)
	{
		JSON_Value* objectGO = json_value_init_object();
		gameobjects[i]->SaveGameObject(json_object(objectGO));
		json_array_append_value(json_array(sceneObjects), objectGO);
	}

	string filePath = "Library\\Scene\\";
	string buffer = App->file_system->FromJSONtoString(sceneValue);
	buffer += "\0";
	FILE* sceneFile;
	filePath += fileName + ".json";
	sceneFile = fopen(filePath.c_str(), "w");
	if (sceneFile != NULL)
	{
		fwrite(buffer.c_str(), sizeof(char), buffer.size(), sceneFile);
		fclose(sceneFile);
	}
	
	json_value_free(sceneValue);
}

void ModuleScene::LoadScene(string filePath)
{
	JSON_Value* sceneValue = json_parse_file(filePath.c_str());
	JSON_Array* gameObjects = json_object_get_array(json_object(sceneValue), "Game Objects");
	vector<GameObject*> objectWithParent; 

	for (int i = 0; i < json_array_get_count(gameObjects); ++i)
	{
		JSON_Object* jsonGO = json_array_get_object(gameObjects, i);
		GameObject* loadedGO = ((GameObject*)jsonGO)->LoadGameObject(json_object(sceneValue));
		gameobjects.push_back(loadedGO);
		if (loadedGO->parentUUID != 0)
		{
			objectWithParent.push_back(loadedGO);
		}
	}

	for (int i = 0; i < objectWithParent.size(); ++i)
	{
		for (int j = 0; j < gameobjects.size(); ++j)
		{
			if (objectWithParent[i]->parentUUID == gameobjects[j]->uuid)
			{
				objectWithParent[i]->AddParent(gameobjects[j]);
			}
		}
	}
}

void ModuleScene::Draw()
{
	GameObjectsToDraw();

	for (auto item = objectsToDraw.begin(); item != objectsToDraw.end(); item++)
	{
		for (auto iter = (*item)->components.begin(); iter != (*item)->components.end(); ++iter)
		{
			if ((*iter)->c_type == MESH)
			{
				ComponentMesh* mesh = (ComponentMesh*)(*iter);

				glColor3f(mesh->colour.x, mesh->colour.y, mesh->colour.z);

				glPushMatrix();
				glMultMatrixf((float*)((ComponentTransform*)(*item)->GetComponent(TRANSFORM))->GetGlobalMatrix().Transposed().ptr());

				glEnableClientState(GL_VERTEX_ARRAY);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
				glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

				if (ComponentMaterial* material = (ComponentMaterial*)(*iter)->parent->GetComponent(MATERIAL))
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindTexture(GL_TEXTURE_2D, material->textureID);
					glTexCoordPointer(2, GL_FLOAT, 0, &mesh->texture_coords[0]);
				}

				glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				if ((*item)->selected)
				{
					if (ComponentMesh* mesh = (ComponentMesh*)(*item)->GetComponent(MESH))
					{
						if (normals)
						{
							App->renderer3D->DrawNormals(mesh);
						}

					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					glColor3f(1.0, 1.0, 1.0);

					glEnableClientState(GL_VERTEX_ARRAY);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
					glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

					glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_VERTEX_ARRAY);

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				glPopMatrix();
			}
		}
	}

	if (App->renderer3D->wireframe)
	{
		App->renderer3D->ActivateWireframe();
	}
}

void ModuleScene::GameObjectsToDraw()
{
	objectsToDraw.clear();
	activeCamera = App->camera->editorCamera;
	for (auto item = cameras.begin(); item != cameras.end(); ++item)
	{
		if ((*item)->active)
		{
			activeCamera = (*item);
			App->camera->editorCamera->active = false;
		}
		else
		{
			activeCamera = App->camera->editorCamera;
			App->camera->editorCamera->active = true;
		}
	}

	for (auto item = gameobjects.begin(); item != gameobjects.end(); ++item)
	{
		if (activeCamera->ContainsAABB((*item)->boundingBox) || activeCamera->culling == false)
		{
			(*item)->drawable = true;
			objectsToDraw.push_back((*item));
		}

		else
		{
			(*item)->drawable = false;
		}
	}
}

void ModuleScene::DeleteGameObject(GameObject* objecttodelete, bool erasefromparent)
{
	for (auto item = gameobjects.begin(); item != gameobjects.end(); ++item)
	{
		if ((*item) == objecttodelete)
		{
			gameobjects.erase(item);
			break;
		}
	}

	if (objecttodelete->parent != nullptr)
	{
		for (auto item = objecttodelete->parent->children.begin(); item != objecttodelete->parent->children.end(); ++item)
		{
			if ((*item) == objecttodelete)
			{
				objecttodelete->parent->children.erase(item);
				break;
			}
		}
	}

	for (auto item = objecttodelete->children.begin(); item != objecttodelete->children.end(); ++item)
	{
		(*item)->parent = nullptr;
	}

	delete(objecttodelete);
	objecttodelete = nullptr;
}

GameObject* ModuleScene::GetGameObjectFromUUID(uint uuid)
{
	for (auto item = gameobjects.begin(); item != gameobjects.end(); ++item)
	{
		if ((*item)->uuid == uuid)
		{
			return (*item);
		}
	}

	return nullptr;
}

void ModuleScene::AddCamera()
{
	string cameraName = "Camera_" + std::to_string(cameras.size());
	if (cameras.size() == 0)
	{
		cameraName = "Camera";
	}

	GameObject* gameobject = App->scene->root->AddChildren(cameraName);
	
	ComponentCamera* camera = (ComponentCamera*)gameobject->AddComponent(CAMERA);
	ComponentTransform* transformations = (ComponentTransform*)gameobject->AddComponent(TRANSFORM);

	cameras.push_back(camera);

	LOG("Game Object Camera Created.");
}
