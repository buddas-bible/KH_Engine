#pragma once
#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")
#include "Vector2D.h"
#include <vector>

class RigidBody;


constexpr float SPRING_EPSILON = 0.00001f;

class Spring
{
public:
	Spring(RigidBody*& rigidA, RigidBody*& rigidB);
	~Spring();

	void SetProperties(float length, float springConstant, float dampingForce);
	void PhysicsUpdate(float dt);
	void Render();

private:
	RigidBody* A{};
	RigidBody* B{};

	std::vector<Vector2D> edge{2};

	float l = 20.f;				// length
	float k = 300000.f;			// spring constant
	float d = 10000.f;			// damping force

};


/*
질량 스프링

F = -k(l-l_0) * direction
F = -d((v1 - v2).Dot(direction)) * direction
두 질점 사이의 길이를 파악.

두 질점의 위치를 가지고 방향을 뽑아내서
그 방향으로 밀어낸다고 한다면...
강한 힘이 가해졌을 때 두 질점의 위치가 순간적으로 바뀌게 된다면
원치 않은 방향으로 밀어나게 되지 않을까?

밀어내는 방향을 고정적으로 하고 싶지만??
그러면 물체가 회전하게 되면 이상해지지 않을까?
고민이 된다.

*/