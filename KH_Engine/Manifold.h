#pragma once
#include <vector>
#include "Vector2D.h"
#include "RigidBody.h"

class CollisionDetection;

/// 충돌 정보를 가지는 구조체
struct Manifold
{
public:
	CollisionDetection* collisionDetection;

public:
	Manifold();
	Manifold(RigidBody*& a, RigidBody*& b);
	Manifold(const Manifold& m);
	~Manifold();

	RigidBody* A;
	RigidBody* B;
	bool passed;

	std::vector<Vector2D> contactPoint{};
	Vector2D referencePoint{};

	Vector2D normalVector{};
	Vector2D tangentVector{};
	float depth{};

	float sfriction{};
	float dfriction{};
	float e{};

	float GetFriction(float& fricA, float& fricB);
	float GetRestitution(float& resA, float& resB);
	void PushOut();

	void Clear();
	void Initialize(Vector2D gravity, float dt);
	void Collision();
	void CollisionEvent();
};


// 서로를 밀어내서 자리를 잡게 한다면
// 가장 깊은 충돌 지점의 크기를 반으로 나눠서 서로를 밀어낸다.
// 그 후 