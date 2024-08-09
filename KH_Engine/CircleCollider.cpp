#include "CircleCollider.h"

#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")

#include "Convert.h"
#include "Transform.h"
#include "ColliderList.h"
#include "KH_Define.h"

CircleCollider::CircleCollider(
	float radius,
	Vector2D pos,
	RigidBody* rigid) : 
	radius(radius)
{
	pRigidBody = rigid;
	pRigidBody->SetPosition(pos);
	m_Position = rigid->GetPosition();
	m_Angle = rigid->GetAngle();
	m_Scale = rigid->GetScale();

	temp.resize(4);
	LTRB.resize(2);
	ComputeMass(pRigidBody->GetDensity());

	Ctype = eColliderType::Type_Circle;
	pList->colliderList.push_back(this);
}

CircleCollider::CircleCollider(float radius, Vector2D pos) : radius(radius)
{
	m_Position = pos;
	m_Angle = 0.f;
	m_Scale = {1.f, 1.f};

	temp.resize(4);
	LTRB.resize(2);
	Ctype = eColliderType::Type_Circle;
	// pList->colliderList.push_back(this);
}

CircleCollider::~CircleCollider()
{
	pList->colliderList.erase(
		remove(
			pList->colliderList.begin(),
			pList->colliderList.end(),
			this),
		pList->colliderList.end()
	);
}

float CircleCollider::GetRadius() const
{
	return radius * (m_Scale.X);
}

void CircleCollider::Render()
{
	// 충돌 중이면 빨강 원을 그림
	if (isCollision == true)
	{
		graphics->DrawCircle(
			Convert::Point2D(m_Position), radius * m_Scale.X,
			D2D1::ColorF::Blue);
	}
	// 충돌 중이 아니면 초록 원을 그림
	else
	{
		graphics->DrawCircle(
			Convert::Point2D(m_Position), radius * m_Scale.X,
			D2D1::ColorF::Black);
	}
}

void CircleCollider::AABBDebugRender()
{
	LTRB[0] = Vector2D{ m_Position.X - GetRadius(), m_Position.Y - GetRadius() };
	LTRB[1] = Vector2D{ m_Position.X + GetRadius(), m_Position.Y + GetRadius() };

	
	if (isCollision == true)
	{
		graphics->DrawRect(
			Convert::Point2fArray(LTRB),
			D2D1::ColorF::Blue);
	}
	else
	{
		graphics->DrawRect(
			Convert::Point2fArray(LTRB),
			D2D1::ColorF::Black);
	}
	
}

void CircleCollider::Update(float dT)
{
	this->m_Position = pRigidBody->GetPosition();
	this->m_Angle = pRigidBody->GetAngle();
	this->m_Scale = pRigidBody->GetScale();
}

ColliderType CircleCollider::GetType()
{
	return Ctype;
}

void CircleCollider::ComputeMass(float density)
{
	pRigidBody->SetMass(density * PI * radius * radius);
	pRigidBody->SetInertia(density * radius * radius);
}

Vector2D CircleCollider::GetFurthestPoint(Vector2D dVec)
{
	return m_Position + (dVec.Normalize() * GetRadius());
}

Edge CircleCollider::GetFeatureEdge(Vector2D dVec)
{
	/*
	Vector2D d = m_Position + dVec * GetRadius();
	return Edge{ d, d };
	*/
	return Edge{ m_Position, m_Position };
}
