#include <list>

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_TRANSFORM)
{
	isUnique = true;
}

C_Transform::~C_Transform()
{

}

void C_Transform::Update(float dt)
{

}

math::float4x4& C_Transform::GetMatrix() const
{
	return math::float4x4::FromTRS(position, rotation, scale);
}

math::float4x4& C_Transform::GetGlobalMatrix() const
{
	std::list<GameObject*> parentsList;
	GameObject* nextParent = parent;
	while (parent->GetParent() != nullptr)
	{
		parentsList.push_back(nextParent);
		nextParent = nextParent->GetParent();
	}

	math::float4x4 globalMatrix = math::float4x4::identity;
	for (std::list<GameObject*>::iterator it = parentsList.end(); it != parentsList.begin(); it++)
	{
		math::float4x4 parentMatrix = (*it)->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetMatrix();
		globalMatrix = globalMatrix * parentMatrix;
	}
	math::float4x4 localMatrix = GetMatrix();

	return globalMatrix * localMatrix;
}