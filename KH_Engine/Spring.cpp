#include "Spring.h"
#include "Convert.h"

#include "RigidBody.h"

class Graphics;

Spring::Spring(RigidBody*& rigidA, RigidBody*& rigidB) :
	A(rigidA), B(rigidB)
{

}

Spring::~Spring()
{

}

void Spring::SetProperties(float length, float springConstant, float dampingForce)
{
	this->l = length;
	this->k = springConstant;
	this->d = dampingForce;
}

void Spring::PhysicsUpdate(float dt)
{
	float distance{ (B->GetPosition() - A->GetPosition()).Size() };

	Vector2D direction_ab{ (B->GetPosition() - A->GetPosition()).Normalize() };

	Vector2D V_ab{ B->GetVelocity() - A->GetVelocity() };

	Vector2D forceA;
	Vector2D dampingA;

	if (fabs(distance - l) < SPRING_EPSILON)
	{
		return;
	}

	forceA = -k * (distance - l) * direction_ab;
	dampingA = -d * ((V_ab).Dot(direction_ab)) * direction_ab;

	Vector2D springForce = forceA + dampingA;

	A->ApplyForce(-1.f * springForce);
	B->ApplyForce(springForce);
}

void Spring::Render()
{
	static Graphics* graphics = Graphics::GetInstance();

	edge[0] = { A->GetPosition() };
	edge[1] = { B->GetPosition() };

	graphics->DrawLine(Convert::Point2fArray(edge));
}
