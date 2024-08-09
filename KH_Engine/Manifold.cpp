#include <cmath>
#include "Manifold.h"
#include "CollisionDetection.h"

#include <iostream>
using namespace std;

Manifold::Manifold() : A(nullptr), B(nullptr)
{
	Clear();
}

Manifold::Manifold(RigidBody*& a, RigidBody*& b) : 
	A(a), B(b),
	passed(false),
	contactPoint(),
	referencePoint(),
	normalVector(),
	tangentVector(),
	depth(),
	sfriction{},
	dfriction{},
	e{}
{
	collisionDetection = new CollisionDetection;
}

Manifold::Manifold(const Manifold& m) : 
	A(m.A), B(m.B), 
	passed(m.passed),
	contactPoint(m.contactPoint),
	referencePoint(m.referencePoint),
	normalVector(m.normalVector),
	tangentVector(m.tangentVector),
	depth(m.depth),
	sfriction{},
	dfriction{},
	e{}
{
	collisionDetection = new CollisionDetection;
}

Manifold::~Manifold()
{
	delete collisionDetection;
	collisionDetection = nullptr;
}

void Manifold::Initialize(Vector2D gravity, float dt)
{
	this->sfriction = GetFriction(A->GetStaticFriction(), B->GetStaticFriction());
	this->dfriction = GetFriction(A->GetDynamicFriction(), B->GetDynamicFriction());
	this->e = GetRestitution(A->GetCOR(), B->GetCOR());

	// 상대속도 비교
	for (size_t i = 0; i < contactPoint.size(); i++)
	{
		Vector2D AtoContactPoint = contactPoint[i] - A->GetPosition();
		Vector2D BtoContactPoint = contactPoint[i] - B->GetPosition();

		Vector2D Bv = B->GetVelocity() + Cross(B->GetAngularVelocity(), BtoContactPoint);
		Vector2D Av = A->GetVelocity() + Cross(A->GetAngularVelocity(), AtoContactPoint);
		Vector2D Sv = Bv - Av;

		// 상대속도의 크기가 중력가속도에 의해 생기는 속력의 크기보다 작으면 반발 계수가 0이 됨.
		if (Sv.SizeSquare() < ((gravity * dt).SizeSquare() + 0.00001f))
		{
			e = 0.f;
		}
	}
}

void Manifold::Collision()
{
	ObjectCollider* a = A->GetCollider();
	ObjectCollider* b = B->GetCollider();

	if (!collisionDetection->GJK(a, b))
		return;

	this->A = collisionDetection->manifold.A;
	this->B = collisionDetection->manifold.B;
	this->contactPoint = collisionDetection->manifold.contactPoint;
	this->referencePoint = collisionDetection->manifold.referencePoint;
	this->normalVector = collisionDetection->manifold.normalVector;
	this->tangentVector = collisionDetection->manifold.tangentVector;
	this->depth = collisionDetection->manifold.depth;

	this->sfriction = GetFriction(A->GetStaticFriction(), B->GetStaticFriction());
	this->dfriction = GetFriction(A->GetDynamicFriction(), B->GetDynamicFriction());
	this->e = GetRestitution(A->GetCOR(), B->GetCOR());
}

float Manifold::GetFriction(float& fricA, float& fricB)
{
	return powf((fricA * fricB), 0.5);
}

float Manifold::GetRestitution(float& resA, float& resB)
{
	if (resA < resB)
	{
		return resA;
	}
	else
	{
		return resB;
	}
}

void Manifold::CollisionEvent()
{
	// A, B의 질량이 0이라면 운동을 하지 않음
	if (((A->GetInvMass() + B->GetInvMass()) - 0.f) <= 0.000001f)
	{
		A->SetVelocity({ 0.f, 0.f });
		B->SetVelocity({ 0.f, 0.f });
		return;
	}

	this->sfriction = GetFriction(A->GetStaticFriction(), B->GetStaticFriction());
	this->dfriction = GetFriction(A->GetDynamicFriction(), B->GetDynamicFriction());
	this->e = GetRestitution(A->GetCOR(), B->GetCOR());

	Vector2D p_a{ A->GetPosition() };
	Vector2D p_b{ B->GetPosition() };

	Vector2D v_a{ A->GetVelocity() };
	Vector2D v_b{ B->GetVelocity() };

	float w_a{ A->GetAngularVelocity() };
	float w_b{ B->GetAngularVelocity() };

	for (size_t i = 0; i < contactPoint.size(); i++)
	{
		// 질량 중심에서 충돌 지점까지의 벡터
		Vector2D AtoContactPoint = contactPoint[i] - p_a;
		Vector2D BtoContactPoint = contactPoint[i] - p_b;

		// 상대속도
		Vector2D Av = v_a + Cross(w_a, AtoContactPoint);
		Vector2D Bv = v_b + Cross(w_b, BtoContactPoint);
		Vector2D Sv = Bv - Av;

		// 충돌 지점에서 노말 방향으로의 상대 속도
		float Cv = Sv.Dot(normalVector);
		
		if (Cv > 0.f)
		{
			return;
		}

		/// 임펄스 공식의 분모 부분임.
		float AN = AtoContactPoint.Cross(normalVector);
		float BN = BtoContactPoint.Cross(normalVector);
		float invMass = A->GetInvMass() + B->GetInvMass();
		float invInertia = (AN * AN) * A->GetInvInertia() + (BN * BN) * B->GetInvInertia();
		float numerator = invMass + invInertia;

		float j = -(1.f + e) * Cv;

		j /= numerator;
		j /= contactPoint.size();

		Vector2D impulse = normalVector * j;
		A->ApplyImpulse(-1.f * impulse, AtoContactPoint);
		B->ApplyImpulse(impulse, BtoContactPoint);

		/// 마찰
// 		Av = A->GetVelocity() + Cross(w_a, AtoContactPoint);
// 		Bv = B->GetVelocity() + Cross(w_b, BtoContactPoint);
// 		Sv = Bv - Av;

		// 노말 방향의 상대속도를 구해서 상대속도에서 빼면 그에 수직인 벡터가 나옴
		// Vector2D tangent = tangentVector;
		Vector2D nSv = normalVector * Cv;
		Vector2D tangent = (Sv - nSv).Normalize();

		float jtangent = -1.f * Sv.Dot(tangent);
		jtangent /= numerator;
		jtangent /= contactPoint.size();

		if (std::fabs(jtangent - 0.0f) <= 0.000001f)
		{
			return;
		}

		Vector2D tangentImpulse;
		if (std::fabs(jtangent) < (j * sfriction))
		{
			tangentImpulse = tangent * jtangent;
		}
		else
		{
			tangentImpulse = tangent * -1.f * j * dfriction;
		}

		A->ApplyImpulse(-1.f * tangentImpulse, AtoContactPoint);
		B->ApplyImpulse(tangentImpulse, BtoContactPoint);
	}
}

void Manifold::PushOut()
{
	Vector2D& Apos = A->GetPosition();
	Vector2D& Bpos = B->GetPosition();

	Vector2D c{};
	float d{};

	// 깊이가 0.05보다 얕으면 밀어내지 않음.
	if ((depth - 0.05f) > 0.0f)
	{
		d = depth - 0.05f;
	}
	else
	{
		d = 0.f;
	}

	c = d / (A->GetInvMass() + B->GetInvMass()) * normalVector * 0.4f;
	Vector2D ac = c * A->GetInvMass();
 	Apos -= ac;
	Vector2D bc = c * B->GetInvMass();
	Bpos += bc;
}

void Manifold::Clear()
{
	contactPoint.clear();
	referencePoint = {};
	normalVector = {};
	tangentVector = {};
	depth = 0.f;
}