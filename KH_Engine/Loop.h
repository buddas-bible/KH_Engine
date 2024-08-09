#pragma once
#include "CollisionDetection.h"
#include "InputManager.h"
#include "RigidBodyManager.h"
#include "RigidBody.h"

#define CList pColliderList->colliderList

class ColliderList;
class Graphics;
class Spring;
class SoftBody;

class Loop
{
private:
	Graphics* pGraphics;
	InputManager* pInputManager;
	ColliderList* pColliderList;
	RigidBodyManager* rigidbodyManager;
	CollisionDetection collisionDetection;

public:
	Loop();
	~Loop();

	void MainLoop();

private:
	int state = 2;
	size_t index = 0;

	void Initialize();
	void Finalize();
	
	void KeyUpdate();

	void Update(float dt);
	void PhysicsUpdate(float dt);
	// void CollisionCheck();
	void Render();

	std::vector<RigidBody*> RigidBodyList;
	std::vector<Spring*> springList;
	std::vector<SoftBody*> softbodyList;
	std::vector<Manifold> manifold;

	enum State
	{
		State_Exit = 0,
		State_AABB = -1,
		State_OBB = 1,
		State_GJK = 2,
	};
};