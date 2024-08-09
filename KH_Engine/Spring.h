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
���� ������

F = -k(l-l_0) * direction
F = -d((v1 - v2).Dot(direction)) * direction
�� ���� ������ ���̸� �ľ�.

�� ������ ��ġ�� ������ ������ �̾Ƴ���
�� �������� �о�ٰ� �Ѵٸ�...
���� ���� �������� �� �� ������ ��ġ�� ���������� �ٲ�� �ȴٸ�
��ġ ���� �������� �о�� ���� ������?

�о�� ������ ���������� �ϰ� ������??
�׷��� ��ü�� ȸ���ϰ� �Ǹ� �̻������� ������?
����� �ȴ�.

*/