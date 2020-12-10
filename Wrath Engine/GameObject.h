#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include <list>
#include <string>
#include <vector>
#include <map>

#include "MathGeoLib/Geometry/AABB.h"
#include "Parson/parson.h"

struct Mesh;

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, string name);
	~GameObject();

	void AddParent(GameObject* newparent);
	Component* AddComponent(Component_Type comp_type);
	GameObject* AddChildren(std::string name);

	void SaveGameObject(JSON_Object* object);
	GameObject* LoadGameObject(JSON_Object* object);

	void CalculateGlobal();

	void Update();

	Component* GetComponent(Component_Type comp_type);

public:
	string name;

	GameObject* parent = nullptr;

	list<Component*> components;
	vector<GameObject*> children;

	bool selected = false;

	uint uuid;
	uint parentUUID;
};

#endif 
