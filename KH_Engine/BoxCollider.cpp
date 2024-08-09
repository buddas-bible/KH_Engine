#include "BoxCollider.h"

#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")

#include "Convert.h"
#include "Transform.h"
#include "ColliderList.h"
#include "Matrix3x3.h"
#include "KH_Define.h"

/*
BoxCollider::BoxCollider(Object* ptr, Vector2D radius)
{
	points.clear();
	points.push_back(Vector2D(ptr->m_Position.X - radius.X, ptr->m_Position.Y - radius.Y));	// LT
	points.push_back(Vector2D(ptr->m_Position.X + radius.X, ptr->m_Position.Y - radius.Y));	// RT
	points.push_back(Vector2D(ptr->m_Position.X + radius.X, ptr->m_Position.Y + radius.Y));	// RB
	points.push_back(Vector2D(ptr->m_Position.X - radius.X, ptr->m_Position.Y + radius.Y));	// LB

	Ctype = CollisionType::Type_Box;
	pObject = ptr;
	pList->colliderList.push_back(this);
}
*/

BoxCollider::BoxCollider(Vector2D radius)
{
	points.clear();
	temp.resize(4);
	LTRB.resize(2);
	points.push_back(Vector2D(-1 * radius.X, -1 * radius.Y));	// LT
	points.push_back(Vector2D(radius.X, -1 * radius.Y));		// RT
	points.push_back(Vector2D(radius.X, radius.Y));			// RB
	points.push_back(Vector2D(-1 * radius.X,  radius.Y));		// LB

	Ctype = eColliderType::Type_Box;
	pList->colliderList.push_back(this);
}

BoxCollider::BoxCollider(
	Vector2D radius,
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
	points.push_back(Vector2D(-1 * radius.X, -1 * radius.Y));	// LT
	points.push_back(Vector2D(radius.X, -1 * radius.Y));		// RT
	points.push_back(Vector2D(radius.X, radius.Y));			// RB
	points.push_back(Vector2D(-1 * radius.X, radius.Y));		// LB
	ComputeMass(pRigidBody->GetDensity());

	Ctype = eColliderType::Type_Box;
	pList->colliderList.push_back(this);
}

BoxCollider::~BoxCollider()
{
	pList->colliderList.erase(
		remove(
			pList->colliderList.begin(),
			pList->colliderList.end(),
			this),
		pList->colliderList.end()
	);
}


void BoxCollider::Render()
{
	if (isCollision == true)
	{
		graphics->DrawDynamicRect(
			Convert::Point2fArray(points),
			Matrix3x3::TransformD2D1Matrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y),
			D2D1::ColorF::Blue);
	}
	else
	{
		graphics->DrawDynamicRect(
			Convert::Point2fArray(points),
			Matrix3x3::TransformD2D1Matrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y),
			D2D1::ColorF::Black);
	}
}

void BoxCollider::AABBDebugRender()
{
	for (size_t i = 0; i < points.size(); i++)
	{
		temp[i] = WorldTransform(points[i], m_Position, m_Angle, m_Scale);
	}

	LTRB[0] = temp[0];
	LTRB[1] = temp[0];

	for (size_t i = 0; i < points.size(); i++)
	{
		// X �۰� Y ����
		LTRB[0].X = (LTRB[0].X < temp[i].X ? LTRB[0].X : temp[i].X);
		LTRB[0].Y = (LTRB[0].Y < temp[i].Y ? LTRB[0].Y : temp[i].Y);

		// X ũ�� Y ŭ
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

void BoxCollider::Update(float dT)
{
	this->m_Position = pRigidBody->GetPosition();
	this->m_Angle = pRigidBody->GetAngle();
	this->m_Scale = pRigidBody->GetScale();
}

ColliderType BoxCollider::GetType()
{
	return Ctype;
}

void BoxCollider::ComputeMass(float density)
{
	Vector2D c = {};					// ���� �߽�
	float area = 0;						// ��ü ����
	float inertia = 0;					// ���� ���Ʈ
	const float inv3 = 1.f / 3.f;		// 3�� ����

	for (size_t i = 0; i < points.size(); i++)
	{
		size_t j = (i + 1) % points.size();
		Vector2D v1 = points[i] * Matrix3x3::ScaleRotate(m_Scale.X, m_Scale.Y, m_Angle);
		Vector2D v2 = points[j] * Matrix3x3::ScaleRotate(m_Scale.X, m_Scale.Y, m_Angle);

		// �ﰢ���� ���� : ���� / 2
		// ������ �߽����� �ﰢ���� ����� ������
		// �� �ΰ��� �˰� �־ �ﰢ���� ������ ���� �� ����
		float A = v1.Cross(v2);
		float T = A * 0.5f;

		// ������ ������Ŵ
		area += T;

		// �����߽��� ���ϱ� ���� ����
		// ���� �߽� = ((�ﰢ���� ����) * (�� ��ǥ�� �� / 3)) / ��ü ����
		c += T * inv3 * (v1 + v2);

		// ���� �����￡ ���� ���� ���Ʈ ����
		// x�࿡ ���� ���Ʈ = 1/12 * �ﰢ���� ���� * {x1^2 + x1x2 + x2^2}
		// y�࿡ ���� ���Ʈ = 1/12 * �ﰢ���� ���� * {y1^2 + y1x2 + y2^2}
		// ���� ���Ʈ�� �� �࿡ ���ؼ� ���� ���� ���ϸ� ��.
		// 1 / 12�� ��������� 0.25�� �ϴ� ��.
		float Ix{ v1.X * v1.X + v1.X * v2.X + v2.X + v2.X };
		float Iy{ v1.Y * v1.Y + v1.Y * v2.Y + v2.Y + v2.Y };
		inertia += (0.25f * inv3 * A) * (Ix + Iy);
	}

	c = c * (1.f / area);

	pRigidBody->SetMass(density * area);
	pRigidBody->SetInertia(density * inertia);
}

Vector2D BoxCollider::GetFurthestPoint(Vector2D dVec)
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

Edge BoxCollider::GetFeatureEdge(Vector2D dVec)
{
	Matrix3x3 transform = Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y);

	Vector2D curr = GetFurthestPoint(dVec);

	size_t prev = (m_index - 1 + points.size()) % points.size();
	size_t next = (m_index + 1) % points.size();

	Vector2D prevP = points[prev] * transform;
	Vector2D nextP = points[next] * transform;

	Vector2D edge1 = (curr - prevP).Normalize();
	Vector2D edge2 = (curr - nextP).Normalize();

	if (abs(edge1.Dot(dVec)) <= abs(edge2.Dot(dVec)))
	{
		return Edge(prevP, curr, prev, m_index);
	}
	else
	{
		return Edge(curr, nextP, m_index, next);
	}
}