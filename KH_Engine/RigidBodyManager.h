#pragma once
#include <vector>

class RigidBody;

class RigidBodyManager
{
private:
	RigidBodyManager();

public:
	~RigidBodyManager();

	static RigidBodyManager* GetInstance();
	std::vector<RigidBody*> rigidbodyList;

	void AddRigidBody(RigidBody* rigidbody);
	RigidBody* CreateRigidBody();
	void RemoveRigidBody(RigidBody* rigidbody);
};