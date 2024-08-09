#include "ColliderList.h"
#include "CircleCollider.h"

#include "RigidBody.h"


ColliderList::ColliderList()
{
	colliderList.clear();
}

ColliderList::~ColliderList()
{
	for (auto& e : colliderList)
	{
		delete e;
		e = nullptr;
	}
	colliderList.clear();
}

ColliderList* ColliderList::GetInstance()
{
	static ColliderList* instance = nullptr;
	if (instance == nullptr)
	{
		instance = new ColliderList;
	}
	return instance;
}

void ColliderList::AddCollider(ObjectCollider* collider)
{
	colliderList.push_back(collider);
}

ObjectCollider* ColliderList::CreateCircleCollider(float radius, Vector2D pos, RigidBody* rigid)
{
	ObjectCollider* collider = new CircleCollider(radius, pos, rigid);
	return collider;
}

void ColliderList::RemoveCollider(ObjectCollider* collider)
{
	colliderList.erase(remove(colliderList.begin(), colliderList.end(), collider));
}
