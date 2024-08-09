#include "LineCollider.h"

#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")

#include "Convert.h"
#include "Transform.h"
#include "ColliderList.h"
#include "Matrix3x3.h"
#include "KH_Define.h"


/*
LineCollider::LineCollider(Object* ptr, float length)
{
	points.clear();
	points.push_back(Vector2D(ptr->m_Position.X - length / 2, ptr->m_Position.Y));
	points.push_back(Vector2D(ptr->m_Position.X + length / 2, ptr->m_Position.Y));

	Ctype = eColliderType::Type_Line;
	pObject = ptr;
	pList->colliderList.push_back(this);
}

LineCollider::LineCollider(float length) : 
	ObjectCollider(Vector2D{0.f, 0.f}, 0.f, Vector2D{0.f, 0.f}, (5.f), Vector2D(0.f, 0.f), (0.f), (0.f), Vector2D(0.f, 0.f))
{
	points.clear();
	temp.resize(4);
	LTRB.resize(2);
	points.push_back(Vector2D(- length / 2, 0.f));
	points.push_back(Vector2D(+ length / 2, 0.f));

	Ctype = eColliderType::Type_Line;
	pList->colliderList.push_back(this);
}
*/

LineCollider::LineCollider(
	float length,
	Vector2D pos,
	RigidBody* rigid)
{
	pRigidBody = rigid;
	pRigidBody->SetPosition(pos);
	m_Position = rigid->GetPosition();
	m_Angle = rigid->GetAngle();
	m_Scale = rigid->GetScale();

	points.clear();
	temp.resize(4);
	LTRB.resize(2);
	points.push_back(Vector2D(- length / 2, 0.f));
	points.push_back(Vector2D(+ length / 2, 0.f));

	Ctype = eColliderType::Type_Line;
	pList->colliderList.push_back(this);
}

LineCollider::LineCollider(Vector2D p1, Vector2D p2)
{
	m_Position = { 0.f, 0.f };
	m_Angle = 0.f;
	m_Scale = { 1.f, 1.f };

	points.clear();
	temp.resize(4);
	LTRB.resize(2);
	points.push_back(p1);
	points.push_back(p2);

	Ctype = eColliderType::Type_Line;
	pList->colliderList.push_back(this);
}

LineCollider::~LineCollider()
{
	pList->colliderList.erase(
		remove(
			pList->colliderList.begin(),
			pList->colliderList.end(),
			this),
		pList->colliderList.end()
	);
}


void LineCollider::Render()
{
	if (isCollision == true)
	{
		graphics->DrawDynamicLine(
			Convert::Point2fArray(points),
			Matrix3x3::TransformD2D1Matrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y),
			D2D1::ColorF::Blue);
	}
	else
	{
		graphics->DrawDynamicLine(
			Convert::Point2fArray(points),
			Matrix3x3::TransformD2D1Matrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y),
			D2D1::ColorF::Black);
	}
}

void LineCollider::AABBDebugRender()
{
	for (size_t i = 0; i < points.size(); i++)
	{
		temp[i] = WorldTransform(points[i], m_Position, m_Angle, m_Scale);
	}

	LTRB[0] = temp[0];
	LTRB[1] = temp[0];

	for (size_t i = 0; i < points.size(); i++)
	{
		// X 작고 Y 작음
		LTRB[0].X = (LTRB[0].X < temp[i].X ? LTRB[0].X : temp[i].X);
		LTRB[0].Y = (LTRB[0].Y < temp[i].Y ? LTRB[0].Y : temp[i].Y);

		// X 크고 Y 큼
		LTRB[1].X = (LTRB[1].X > temp[i].X ? LTRB[1].X : temp[i].X);
		LTRB[1].Y = (LTRB[1].Y > temp[i].Y ? LTRB[1].Y : temp[i].Y);
	}

	
	if (isCollision == true)
	{
		graphics->DrawRect(
			Convert::Point2fArray(LTRB),
			D2D1::ColorF::Red);
	}
	else
	{
		graphics->DrawRect(
			Convert::Point2fArray(LTRB),
			D2D1::ColorF::Green);
	}
	
}

void LineCollider::Update(float dT)
{
	this->m_Position = pRigidBody->GetPosition();
	this->m_Angle = pRigidBody->GetAngle();
	this->m_Scale = pRigidBody->GetScale();
}

ColliderType LineCollider::GetType()
{
	return Ctype;
}


void LineCollider::ComputeMass(float density)
{

}

Vector2D LineCollider::GetFurthestPoint(Vector2D dVec)
{
	size_t index = 0;
	float length = (points[index] * Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y)).Dot(dVec);

	for (size_t i = 1; i < points.size(); i++)
	{
		float temp = (points[i] * Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y)).Dot(dVec);

		if (temp > length)
		{
			length = temp;
			index = i;
		}
	}

	return points[index] * Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y);
}

Edge LineCollider::GetFeatureEdge(Vector2D dVec)
{
	Matrix3x3 transform = Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y);

	Vector2D curr = GetFurthestPoint(dVec);

	size_t prev = (m_index - 1 + points.size()) % points.size();
	size_t next = (m_index + 1) % points.size();

	Vector2D prevP = points[prev] * transform;
	Vector2D nextP = points[next] * transform;

	Vector2D edge1 = (curr - prevP).Normalize();
	Vector2D edge2 = (curr - nextP).Normalize();

	if (edge1.Dot(dVec) <= edge2.Dot(dVec))
	{
		return Edge(prevP, curr, prev, m_index);
	}
	else
	{
		return Edge(curr, nextP, m_index, next);
	}
}
