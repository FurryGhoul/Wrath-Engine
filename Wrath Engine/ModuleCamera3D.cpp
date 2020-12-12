#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include <math.h>
#include <queue>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	cameraPos = vec(0.0f, 4.0f, 10.0f);
	cameraRef = vec(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init()
{
	editorCamera = new ComponentCamera(CAMERA, nullptr);

	LOG("Starting camera");
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	vec newPos(0, 0, 0);

	float Sensitivity = dt;
	float speed = 9.0f * dt;
	float wheel = 100.0f * dt;

	int mouse_x = App->input->GetMouseXMotion();
	int mouse_y = -App->input->GetMouseYMotion();


	//LOG("x: %f, y: %f", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
	//LOG("sX: %f, sY: %f", App->ui->uiScreenX, App->ui->uiScreenY);

	float DeltaX = (float)mouse_x * Sensitivity;
	float DeltaY = (float)mouse_y * Sensitivity;;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) { speed /= 16.0f * dt; }
	if (App->input->GetMouseZ() != 0) { if (App->input->GetMouseZ() > 0) { cameraPos -= Z * wheel; } else cameraPos += Z * wheel; }

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		LookAt({ 0, 0, 0 });
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)	//Normal movement 
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += speed * editorCamera->camera_frustum.front;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= speed * editorCamera->camera_frustum.front;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += speed * editorCamera->camera_frustum.WorldRight();
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= speed * editorCamera->camera_frustum.WorldRight();
		
		cameraPos += newPos;
		cameraRef += newPos;

		//Mouse X Right
		if (mouse_x != 0)
		{
			Quat rotation = Quat::RotateY(DeltaX);
			editorCamera->camera_frustum.front = rotation.Mul(editorCamera->camera_frustum.front).Normalized();
			editorCamera->camera_frustum.up = rotation.Mul(editorCamera->camera_frustum.up).Normalized();
		}
		//Mouse Y Left
		if (mouse_y != 0)
		{
			Quat rotation = Quat::RotateAxisAngle(editorCamera->camera_frustum.WorldRight(), DeltaY);
			editorCamera->camera_frustum.front = rotation.Mul(editorCamera->camera_frustum.front).Normalized();
			editorCamera->camera_frustum.up = rotation.Mul(editorCamera->camera_frustum.up).Normalized();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)	//Orbit movement
	{
		vec newPos = cameraPos - cameraRef;
		//Mouse X Right
		if (mouse_x != 0)
		{
			Quat rotation = Quat::RotateY(DeltaX);
			editorCamera->camera_frustum.front = rotation.Mul(editorCamera->camera_frustum.front).Normalized();
			editorCamera->camera_frustum.up = rotation.Mul(editorCamera->camera_frustum.up).Normalized();
		}
		//Mouse Y Left
		if (mouse_y != 0)
		{
			Quat rotation = Quat::RotateAxisAngle(editorCamera->camera_frustum.WorldRight(), DeltaY);

			if (rotation.Mul(editorCamera->camera_frustum.up).Normalized().y > 0.0f)
			{
				editorCamera->camera_frustum.up = rotation.Mul(editorCamera->camera_frustum.up).Normalized();
				editorCamera->camera_frustum.front = rotation.Mul(editorCamera->camera_frustum.front).Normalized();
			}			
		}
		
		editorCamera->camera_frustum.pos = cameraPos = cameraRef - editorCamera->camera_frustum.front*newPos.Length();
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_LEFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		GameObject* selectedGO = StartMousePicking();

		if (App->hierarchy->selectedGO)
		{
			App->hierarchy->selectedGO->selected = false;
		}

		if (selectedGO)
		{
			App->hierarchy->selectedGO = selectedGO;
			selectedGO->selected = true;
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)	//Move camera
	{
		//Mouse X Right
		if (mouse_x != 0)
		{
			newPos += X * DeltaX * 2;
		}
		//Mouse Y Left
		if (mouse_y != 0)
		{
			newPos -= Y * DeltaY * 2;
		}

		cameraPos += newPos;
		cameraRef += newPos;
	}

	if (App->input->GetMouseZ() != 0)
	{
		if (App->input->GetMouseZ() > 0)
		{
			cameraPos -= Z * wheel;
		}

		else
			cameraPos += Z * wheel;
	}

	editorCamera->camera_frustum.pos = cameraPos;

	Z = -editorCamera->camera_frustum.front;
	Y = editorCamera->camera_frustum.up;
	X = editorCamera->camera_frustum.WorldRight();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec &Spot)
{
	cameraRef = Spot;

	vec dir = Spot - editorCamera->camera_frustum.pos;

	float3x3 viewMat = float3x3::LookAt(editorCamera->camera_frustum.front, dir.Normalized(), editorCamera->camera_frustum.up, vec(0.0f, 1.0f, 0.0f));

	editorCamera->camera_frustum.front = viewMat.MulDir(editorCamera->camera_frustum.front).Normalized();
	editorCamera->camera_frustum.up = viewMat.MulDir(editorCamera->camera_frustum.up).Normalized();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec &Movement)
{
	cameraPos += Movement;
	cameraRef += Movement;
}

mat4x4 ModuleCamera3D::At(const vec3& right, const vec3& up, const vec3& dir)
{
	mat4x4 matrix_1 = { right.x, right.y, right.z, 0, up.x, up.y, up.z, 0, dir.x, dir.y, dir.z, 0, 0, 0, 0, 1 };
	mat4x4 matrix_2 = { X.x, X.y, X.z, -cameraPos.x, Y.x, Y.y, Y.z, -cameraPos.y, Z.x, Z.y, Z.z, -cameraPos.z, 0, 0, 0, 1 };

	return matrix_1 * matrix_2;
}

GameObject* ModuleCamera3D::StartMousePicking()
{
	GameObject* objectToSelect = nullptr;

	float mouseX = ImGui::GetMousePos().x - App->ui->uiScreenX;
	float mouseY = ImGui::GetMousePos().y - App->ui->uiScreenY;
	mouseX = (mouseX/(App->ui->uiScreenW / 2)) - 1;
	mouseY = (mouseY/(App->ui->uiScreenH / 2)) - 1;

	//LOG("X %f | Y %f", mouseX, -mouseY);

	LineSegment picking = editorCamera->camera_frustum.UnProjectLineSegment(mouseX, -mouseY);
	App->renderer3D->ray = picking;

	std::priority_queue <Hit_Object*, std::vector<Hit_Object*>, Hit_Distance> hitObjects;
	float hitDistance = 0;
	float endDistance = 0;

	for (auto item = App->scene->gameobjects.begin(); item != App->scene->gameobjects.end(); item++)
	{
		bool hit = picking.Intersects((*item)->boundingBox, hitDistance, endDistance);
		if (hit)
		{
			Hit_Object* hitObject = new Hit_Object((*item), hitDistance);
			hitObjects.push(hitObject);
		}
	}

	float closestDist = -1;
	float actualDist = 0;

	while (!hitObjects.empty())
	{
		Hit_Object* auxObject = hitObjects.top();
		actualDist = FindClosest(auxObject->gameObject, picking);
		hitObjects.pop();

		if (actualDist > -1 && (actualDist < closestDist || closestDist == -1))
		{
			closestDist = actualDist;
			objectToSelect = auxObject->gameObject;
		}
	}

	return objectToSelect;
}

float ModuleCamera3D::FindClosest(GameObject* hitobject, LineSegment ray)
{
	float ret = -1.f;

	ComponentTransform* hitTransform = ((ComponentTransform*)hitobject->GetComponent(TRANSFORM));
	ComponentMesh* hitMesh = ((ComponentMesh*)hitobject->GetComponent(MESH));

	ray.Transform(hitTransform->GetGlobalMatrix().Inverted());

	if (hitMesh != nullptr)
	{
		float hitDistance;
		float3 hitPoint;
		for (int i = 0; i < hitMesh->num_indices; i += 3)
		{
			math::Triangle meshTriangle;
			meshTriangle.a = { hitMesh->vertices[hitMesh->indices[i]*3], hitMesh->vertices[hitMesh->indices[i]*3 + 1], hitMesh->vertices[hitMesh->indices[i]*3 + 2] };
			meshTriangle.b = { hitMesh->vertices[hitMesh->indices[i + 1] * 3], hitMesh->vertices[hitMesh->indices[i + 1] * 3 + 1], hitMesh->vertices[hitMesh->indices[i + 1] * 3 + 2] };
			meshTriangle.c = { hitMesh->vertices[hitMesh->indices[i + 2] * 3], hitMesh->vertices[hitMesh->indices[i + 2] * 3 + 1], hitMesh->vertices[hitMesh->indices[i + 2] * 3 + 2] };
			bool hit = ray.Intersects(meshTriangle, &hitDistance, &hitPoint);

			if (hit && hitDistance > 0 && (hitDistance < ret || ret == -1))
			{
				ret = hitDistance;
			}
		}
	}

	return ret;
}
