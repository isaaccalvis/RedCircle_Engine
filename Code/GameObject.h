#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include <string>
#include "Globals.h"

enum COMPONENT_TYPE
{
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, GameObject* parent = nullptr);
	~GameObject();

	void Update(float dt);

	Component* CreateComponent(COMPONENT_TYPE type, std::string name);
	Component* GetComponent(COMPONENT_TYPE type, std::string name);
public:

private:
	std::string name;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};

#endif