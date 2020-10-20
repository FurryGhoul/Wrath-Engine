#pragma once

#include "UI_Element.h"
#include "SDL/include/SDL.h"

class UI_Console : public UI_Element
{
public:
	UI_Console(Application* app, bool start_enabled = true);
	~UI_Console();

	void Draw(bool* open);
	void CreateLog(const char* log);
	void Clear() { text_buffer.clear(); }

	bool scrolling = false;
	ImGuiTextBuffer text_buffer;
};
