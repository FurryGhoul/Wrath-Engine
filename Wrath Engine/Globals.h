#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define RELEASE(x){ if(x != NULL) {delete x; x = NULL;}}
#define RELEASE_ARRAY( x ) { if( x != NULL ) {delete[] x; x = NULL;}} // Deletes an array of buffers

typedef unsigned int uint;

void StartPCGRNG();

uint Generate_UUID();

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum Component_Type 
{ 
	NO_TYPE, 
	MESH, 
	MATERIAL, 
	TRANSFORM,
	CAMERA
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "3D Physics Playground"

//Files
#define SETTINGS_FOLDER "/Settings/"

#define ASSETS_FOLDER "/Assets/"
#define MODELS_FOLDER "/Assets/Models/"

#define LIBRARY_FOLDER "/Library/"
#define MESHES_FOLDER "/Library/Meshes"
#define TEXTURES_FOLDER "/Library/Textures"
#define SCENES_FOLDER "/Library/Scenes"