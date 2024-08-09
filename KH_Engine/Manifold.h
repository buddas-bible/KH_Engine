#pragma once
#include <vector>
#include "Vector2D.h"
#include "RigidBody.h"

class CollisionDetection;

/// �浹 ������ ������ ����ü
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


// ���θ� �о�� �ڸ��� ��� �Ѵٸ�
// ���� ���� �浹 ������ ũ�⸦ ������ ������ ���θ� �о��.
// �� �� 