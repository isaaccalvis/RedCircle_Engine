#include "Component.h"

Component::Component(GameObject* parent, COMPONENT_TYPE type)
{
	this->parent = parent;
	this->type = type;
}

Component::~Component()
{

}

void Component::SetEnable(bool set)
{
	active = set;
}

bool Component::GetEnabled() const
{
	return active;
}

C_Transform* Component::GetComponentAsTransform() const
{
	return (C_Transform*)this;
}

C_Mesh* Component::GetComponentAsMesh() const
{
	return (C_Mesh*)this;
}

C_Material* Component::GetComponentAsMaterial() const
{
	return (C_Material*)this;
}

C_Camera* Component::GetComponentAsCamera() const
{
	return (C_Camera*)this;
}