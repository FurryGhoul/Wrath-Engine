#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

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
	App->loader->Import("BakerHouse.fbx");

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

	return UPDATE_CONTINUE;
}

void ModuleScene::Draw()
{
	for (auto item = gameobjects.begin(); item != gameobjects.end(); item++)
	{
		for (auto iter = (*item)->components.begin(); iter != (*item)->components.end(); ++iter)
		{
			if ((*iter)->c_type == MESH)
			{
				ComponentMesh* mesh = (ComponentMesh*)(*iter);

				glColor3f(1.0, 1.0, 1.0);

				glEnableClientState(GL_VERTEX_ARRAY);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
				glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

				if (ComponentMaterial* material = (ComponentMaterial*)(*iter)->parent->GetComponent(MATERIAL) )
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
			}
		}
	}
}
