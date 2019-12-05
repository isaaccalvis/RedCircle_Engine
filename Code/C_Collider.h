#ifndef __C_Collider_H__
#define __C_Collider_H__

#include "Globals.h"
#include "Component.h"

class C_Collider : public Component
{
public:
	C_Collider(GameObject* parent);
	~C_Collider();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);
	void OnLoadJson(JSON_Object* object);

	void UpdatePosition();
	void SetPosition(math::float3 position);

	PHYSIC_PRIMITIVE GetShape();
	void SetShape(PHYSIC_PRIMITIVE primitive);
	void SetShape(const char* primitiveName);

	void SetSize(math::float3 size);
	math::float3 GetSize() const;

public:
	uuid_unit colliderUUID = 0u;
private:
	btRigidBody* rigidBody = nullptr;
	math::float3 size = math::float3::one;
	PHYSIC_PRIMITIVE shapePrimitive;

};

#endif