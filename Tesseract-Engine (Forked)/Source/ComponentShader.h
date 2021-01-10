#ifndef __COMPONENTSHADER_H__
#define __COMPONENTSHADER_H__

#include "Component.h"
#include <string>

class ShaderProgram;

class ComponentShader : public Component
{
public:
	ComponentShader(GameObject* gameobject, componentType type);


	~ComponentShader();

	void DrawInfo();

	void Save(JSON_Value* component) const;
	void Load(JSON_Value* component);

public:
		std::string name;
};
#endif // !__COMPONENTSHADER_H__
