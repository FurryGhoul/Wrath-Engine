#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(Component_Type type, GameObject* parent) : Component(type, parent) {}

ComponentMaterial::~ComponentMaterial() {}

void ComponentMaterial::GetTextureInfo(texture textureInfo)
{
	textureID = textureInfo.textureID;
	width = textureInfo.width;
	height = textureInfo.height;
	name = textureInfo.name;
	path = textureInfo.path;
	depth = textureInfo.depth;
	BPP = textureInfo.BPP;
}
