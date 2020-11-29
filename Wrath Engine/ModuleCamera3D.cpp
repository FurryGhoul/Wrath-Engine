#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "ComponentCamera.h"

#include <math.h>

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

	int mouse_x = +App->input->GetMouseXMotion();
	int mouse_y = -App->input->GetMouseYMotion();

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
			editorCamera->camera_frustum.front = rotation.Mul(editorCamera->camera_frustum.front).Normalized();
			editorCamera->camera_frustum.up = rotation.Mul(editorCamera->camera_frustum.up).Normalized();
		}
		
		editorCamera->camera_frustum.pos = cameraPos = cameraRef + editorCamera->camera_frustum.front*newPos.Length();
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