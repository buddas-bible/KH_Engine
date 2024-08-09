#include "RigidBodyManager.h"
#include "RigidBody.h"


RigidBodyManager::RigidBodyManager()
{
	rigidbodyList.clear();
}

RigidBodyManager::~RigidBodyManager()
{
	for (auto& e : rigidbodyList)
	{
		delete e;
		e = nullptr;
	}
	rigidbodyList.clear();
}

RigidBodyManager* RigidBodyManager::GetInstance()
{
	static RigidBodyManager* instance = nullptr;
	if (instance == nullptr)
	{
		instance = new RigidBodyManager;
	}
	return instance;
}

void RigidBodyManager::AddRigidBody(RigidBody* rigidbody)
{
	rigidbodyList.push_back(rigidbody);
}

RigidBody* RigidBodyManager::CreateRigidBody()
{
	RigidBody* rigidbody = new RigidBody();
	return rigidbody;
}

void RigidBodyManager::RemoveRigidBody(RigidBody* rigidbody)
{
	rigidbodyList.erase(remove(rigidbodyList.begin(), rigidbodyList.end(), rigidbody));
}
