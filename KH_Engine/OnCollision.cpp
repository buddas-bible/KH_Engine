#include "OnCollision.h"
#include "Vector2D.h"
#include "Matrix3x3.h"
#include "Transform.h"

#include "ObjectCollider.h"
#include "LineCollider.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "PolygonCollider.h"

#include "KH_Define.h"
#include "CollisionDetection.h"
#include "RigidBody.h"

#include <iostream>
#include <cmath>
#include <utility>


float OnCollision::GetImpulse(RigidBody* A, RigidBody* B)
{
	return 0;
}

float OnCollision::GetFriction(float& fricA, float& fricB)
{
	return powf((fricA * fricA + fricB * fricB), 0.5);
}

float OnCollision::GetRestitution(float& resA, float& resB)
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

/*
void OnCollision::EventCircleToCircle(CircleCollider& circle1, CircleCollider& circle2)
{
	// ź�� ���
	// ������ ��ü���� ������ �ִ°� �´ٰ� ���������� �������ϱ�
	// ���� ź�� �浹
	float e = 1.f;

	// �浹�� ���� �߽��� �մ� ����
	Vector2D C1toC2 = circle2.m_Position - circle1.m_Position;
	
	// �߽��� ���� ���͸� ����ȭ
	// �ش� ���͸� ������ �ؼ� �ӷ��� ������ ����
	Vector2D C1toC2UnitVec = C1toC2.Normalize();
	// Vector2D N = C1toC2UnitVec * Matrix3x3::RotateMatrix(PI / 2);
	
	// v1 = ((m1 - m2) / (m1 + m2)) * v1 + ((m2 + m2) / (m1 + m2)) * v2 
	// Vector2D C1VeloX = (circle1.GetRadius() - e * circle2.GetRadius()) / (circle1.GetRadius() + circle2.GetRadius()) *
	// 	proj1Vector
	// /*circle1.velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec*/
	// 	+ (circle2.GetRadius() + e * circle2.GetRadius()) / (circle1.GetRadius() + circle2.GetRadius()) * 
	// 	proj2Vector;
	// /*circle2.velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec;*/


// v2 = ((m1 + m1) / (m1 + m2)) * v1 + ((m2 - m1) / (m1 + m2)) * v2 
	// Vector2D C2VeloX = (circle1.GetRadius() + e * circle1.GetRadius()) / (circle1.GetRadius() + circle2.GetRadius()) * 
	// 	proj1Vector
	// /*circle1.velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec*/
 	//	+ (circle2.GetRadius() - e * circle1.GetRadius()) / (circle1.GetRadius() + circle2.GetRadius()) * 
 	//	proj2Vector;
 	// /*circle2.velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec;*/
	/*
	
	Vector2D proj1Vector = circle1.*velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec;
	Vector2D proj2Vector = circle2.pRigidBody->GetVelocity().Projection(C1toC2UnitVec) * C1toC2UnitVec;

	// ��ź�� �浹 ����
	// v1 - (m2 * (1 + e) / (m1 + m2)) * (v1 - v2)
	Vector2D C1VeloX = proj1Vector - 
		(circle2.GetRadius() * (1 + e)) / (circle1.GetRadius() + circle2.GetRadius()) *
		(proj1Vector - proj2Vector);
	
	// v2 + (m1 * (1 + e) / (m1 + m2)) * (v1 - v2)
	Vector2D C2VeloX = proj2Vector + 
		(circle1.GetRadius() * (1 + e)) / (circle1.GetRadius() + circle2.GetRadius()) *
		(proj1Vector - proj2Vector);

	/// ź�� ��� �Ű� �Ⱦ��� �Ʒ��� �ᵵ ��
	// Vector2D C1VeloX = circle1.velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec;
	// Vector2D C2VeloX = circle2.velocity.Projection(C1toC2UnitVec) * C1toC2UnitVec;

	/// C1VeloX, C2VeloX�� ���� �򰥸��� �ƴ�.
	// Vector2D C1VeloY = circle1.velocity - C2VeloX;
	// Vector2D C2VeloY = circle2.velocity - C1VeloX;
	circle1.m_Velocity = C1VeloX + (circle1.m_Velocity - C2VeloX);
	circle2.m_Velocity = C2VeloX + (circle2.m_Velocity - C1VeloX);
}
*/

void OnCollision::EventCircleToLine(CircleCollider& circle1, LineCollider& line1)
{
	// �����ص� �� �����
	// ���� ���� �浹 ���� ��
	// ������ �������� 
}

void OnCollision::CollisionEvent(ObjectCollider* A, ObjectCollider* B, Manifold& maniflod)
{
	RigidBody*& rigidA = A->pRigidBody;
	RigidBody*& rigidB = B->pRigidBody;


	if (((rigidA->GetInvMass() + rigidB->GetInvMass()) - 0.f) <= 0.000001f)
	{
		rigidA->SetVelocity({ 0.f, 0.f });
		rigidB->SetVelocity({ 0.f, 0.f });
		return;
	}

	float sfriction = GetFriction(rigidA->GetStaticFriction(), rigidB->GetStaticFriction());
	float dfriction = GetFriction(rigidA->GetDynamicFriction(), rigidB->GetDynamicFriction());
	float e = GetRestitution(rigidA->GetCOR(), rigidB->GetCOR());

	for (size_t i = 0; i < maniflod.contactPoint.size(); i++)
	{
		// ���� �߽ɿ��� �浹 ���������� ����
		Vector2D AtoContactPoint = maniflod.contactPoint[i] - rigidA->GetPosition();
		Vector2D BtoContactPoint = maniflod.contactPoint[i] - rigidB->GetPosition();

		// ���ӵ�
		Vector2D Bv = rigidB->GetVelocity() + Cross(rigidB->GetAngularVelocity(), BtoContactPoint);
		Vector2D Av = rigidA->GetVelocity() + Cross(rigidA->GetAngularVelocity(), AtoContactPoint);
		Vector2D Sv = Bv - Av;

		// �浹 �������� �븻 ���������� �ӵ�
		float Cv = Sv.Dot(maniflod.normalVector);

		if (Cv > 0.f)
		{
			return;
		}

		/// ���޽� ������ �и� �κ���.
		float AN = AtoContactPoint.Cross(maniflod.normalVector);
		float BN = BtoContactPoint.Cross(maniflod.normalVector);
		float invMass = rigidA->GetInvMass() + rigidB->GetInvMass();
		float invInertia = (AN * AN) * rigidA->GetInvInertia() + (BN * BN) * rigidB->GetInvInertia();
		float numerator = invMass + invInertia;

		float j = -(1.f + e) * Cv;

		j /= numerator;
		j /= maniflod.contactPoint.size();

		Vector2D impulse = maniflod.normalVector * j;
		rigidB->ApplyImpulse(impulse, BtoContactPoint);
		rigidA->ApplyImpulse(-1.f * impulse, AtoContactPoint);


		/// ����
		Sv = Bv - Av;

		Vector2D tangent = Sv - (maniflod.normalVector * Sv.Dot(maniflod.normalVector));
		tangent = tangent.Normalize();

		float jtangent = -1.f * Sv.Dot(tangent);
		jtangent /= numerator;
		jtangent /= maniflod.contactPoint.size();

		if (fabs(jtangent - 0.f) <= 0.000001f)
		{
			return;
		}

		Vector2D tangentImpulse;
		if (fabs(jtangent) < (j * sfriction))
		{
			tangentImpulse = tangent * jtangent;
		}
		else
		{
			tangentImpulse = tangent * -1.f * j * dfriction;
		}

		rigidB->ApplyImpulse(tangentImpulse, BtoContactPoint);
		rigidA->ApplyImpulse(-1.f * tangentImpulse, AtoContactPoint);
	}

	/*
	Vector2D& Apos = rigidA->GetPosition();
	Vector2D& Bpos = rigidB->GetPosition();

	Vector2D c{};
	float d{};

	// ���̰� 0.05���� ������ �о�� ����.
	if ((maniflod.depth - 0.05f) > 0.f)
	{
		d = maniflod.depth - 0.05f;
	}
	else
	{
		d = 0.f;
	}
	
	c = d / (rigidA->GetInvMass() + rigidB->GetInvMass()) * maniflod.normalVector * 0.4f;
	Apos -= c * rigidA->GetInvMass();
	Bpos += c * rigidB->GetInvMass();
	*/
}
