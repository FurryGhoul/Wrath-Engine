#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(){}

ComponentMaterial::ComponentMaterial(Component_Type type, GameObject* parent) : Component(type, parent) {}

ComponentMaterial::~ComponentMaterial() {}