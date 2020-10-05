#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 75.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Player_camera();

public:

	PhysVehicle3D* vehicle;
	
	float turn;
	float acceleration;
	float brake;
	
	vec3 car_origin;
	vec3 car_dir;
	vec3 cam_new_pos;
	vec3 point_look;

	bool start_race=false;
	bool checkpoint = false;
	
	btTransform car_pos;

	Timer race_time;

	mat4x4 init_matrix;

	uint laps;

};