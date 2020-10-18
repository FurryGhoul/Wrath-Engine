#pragma once
#include "UI_Element.h"
#include "SDL/include/SDL.h"

class UI_Scene : public UI_Element
{
public:
	UI_Scene(Application* app, bool start_enabled = true);
	~UI_Scene();

	void Draw(bool* open);
	void DrawGrid(int GridSize);
	void DrawAxis(bool active);

	bool active_grid = true;
	bool active_axis = true;

	int grid_size = 20;
};
