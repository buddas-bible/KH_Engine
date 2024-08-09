#pragma once
#include "vector"
#include "Vector2D.h"

class ObjectCollider;
class LineCollider;
class CircleCollider;
class BoxCollider;
class PolygonCollider;
class RigidBody;


class ColliderList
{
private:
	ColliderList();

public:
	~ColliderList();

	static ColliderList* GetInstance();
	std::vector<ObjectCollider*> colliderList;

	void AddCollider(ObjectCollider* collider);
	ObjectCollider* CreateCircleCollider(float radius, Vector2D pos, RigidBody* rigid);
	void RemoveCollider(ObjectCollider* collider);

};