#ifndef __PANELSHADEREDITOR_H__
#define __PANELSHADEREDITOR_H__

#include "Panel.h" 
#include "ImGuiColorTextEdit/TextEditor.h"

#include <string>

struct Shader;

class PanelShaderEditor : public Panel
{
public:
	PanelShaderEditor(const char* name);
	~PanelShaderEditor();

	void Draw();

public:
	TextEditor editor;

	std::string shaderPath;

	Shader* shader;

};
#endif // !__PANELSHADEREDITOR_H__
