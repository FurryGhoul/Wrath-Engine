#include "ComponentMesh.h"

ComponentMesh::ComponentMesh() {}

ComponentMesh::ComponentMesh(Component_Type type, GameObject* parent) : Component(type, parent) {}

ComponentMesh::~ComponentMesh() {}
