#pragma once
#include "UI_Element.h"
#include "SDL/include/SDL.h"

class UI_Config : public UI_Element
{
public:
	UI_Config(Application* app, bool start_enabled = true);
	~UI_Config();

	void Draw(bool* open);

	char app_name[30] = "Motores Juan";
	char org[30] = "UPC CITM";

private:
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldesktop = false;

	char* RDTSC;
	char* MMX;
	char* AVX;
	char* SSE;
	char* SSE2;
	char* SSE3;
	char* SSE41;
	char* SSE42;

	float fps[100];
	float ms[100];

	float bright_slider_scroll = 1;
};

