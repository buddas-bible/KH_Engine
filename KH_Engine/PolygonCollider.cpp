#include "PolygonCollider.h"

#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")

#include "Convert.h"
#include "Transform.h"
#include "Matrix3x3.h"
#include "ColliderList.h"
#include "KH_Define.h"
	
PolygonCollider::PolygonCollider(float radius)
{
	points.clear();
	Initialize(4, radius);
	temp.resize(points.size());
	LTRB.resize(2);

	Ctype = eColliderType::Type_Polygon;
	pList->colliderList.push_back(this);
}

PolygonCollider::PolygonCollider(
	size_t vertex,
	float radius,
	Vector2D pos,
	RigidBody* rigid)
{
	pRigidBody = rigid;
	pRigidBody->SetPosition(pos);
	m_Position = rigid->GetPosition();
	m_Angle = rigid->GetAngle();
	m_Scale = rigid->GetScale();

	points.clear();
	Initialize(vertex, radius);
	temp.resize(points.size());
	LTRB.resize(2);

	Ctype = eColliderType::Type_Polygon;
	pList->colliderList.push_back(this);
}

PolygonCollider::~PolygonCollider()
{
	pList->colliderList.erase(
		remove(
			pList->colliderList.begin(),
			pList->colliderList.end(),
			this),
		pList->colliderList.end()
	);
}

void PolygonCollider::Initialize(size_t n, float radius)
{
	if (n < 3)
	{
		return;
	}

	float theta = (2 * PI) / n;

	// 0�� �ε����κ��� �ݽð� �������� ���� ����
	if (n % 2 == 0)
	{
		for (size_t i = 0; i < n; i++)
		{
			points.push_back(Vector2D{ 0.f, -1.f * radius } *
				Matrix3x3::RotateMatrix(- theta / 2 + theta * i));
		}
	}
	else
	{
		for (size_t i = 0; i < n; i++)
		{
			points.push_back(Vector2D{ 0.f, -1.f * radius } *
				Matrix3x3::RotateMatrix(theta * i));
		}
	}

	ComputeMass(pRigidBody->GetDensity());
}

void PolygonCollider::Render()
{
	if (isCollision == true)
	{
		graphics->DrawDynamicPolygon(
			Convert::Point2fArray(points),
			Matrix3x3::TransformD2D1Matrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y),
			D2D1::ColorF::Blue);
	}
	else
	{
		graphics->DrawDynamicPolygon(
			Convert::Point2fArray(points),
			Matrix3x3::TransformD2D1Matrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y),
			D2D1::ColorF::Black);
	}
}

void PolygonCollider::AABBDebugRender()
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
			D2D1::ColorF::Blue);
	}
	else
	{
		graphics->DrawRect(
			Convert::Point2fArray(LTRB),
			D2D1::ColorF::Black);
	}
	
}

void PolygonCollider::Update(float dT)
{
	this->m_Position = pRigidBody->GetPosition();
	this->m_Angle = pRigidBody->GetAngle();
	this->m_Scale = pRigidBody->GetScale();
}

ColliderType PolygonCollider::GetType()
{
	return Ctype;
}

void PolygonCollider::ComputeMass(float density)
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
		// c += T * inv3 * (v1 + v2);

		// ���� �����￡ ���� ���� ���Ʈ ����
		// x�࿡ ���� ���Ʈ = 1/12 * �ﰢ���� ���� * {x1^2 + x1x2 + x2^2}
		// y�࿡ ���� ���Ʈ = 1/12 * �ﰢ���� ���� * {y1^2 + y1x2 + y2^2}
		// ���� ���Ʈ�� �� �࿡ ���ؼ� ���� ���� ���ϸ� ��.
		// 1 / 12�� ��������� 0.25�� �ϴ� ��.
		float Ix{ v1.X * v1.X + v1.X * v2.X + v2.X + v2.X };
		float Iy{ v1.Y * v1.Y + v1.Y * v2.Y + v2.Y + v2.Y };
		inertia += (0.25f * inv3 * A) * (Ix + Iy);
	}

	// c = c * (1.f / area);

	/*
	for (size_t i = 0; i < points.size(); i++)
	{
		points[i] -= c;
	}
	*/

	pRigidBody->SetMass(density * area);
	pRigidBody->SetInertia(density * inertia);
}

Vector2D PolygonCollider::GetFurthestPoint(Vector2D dVec)
{
	size_t index = 0;
	Matrix3x3 transform = Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y);
	float theta = (points[index] * transform).Dot(dVec);

	for (size_t i = 1; i < points.size(); i++)
	{
		float temp = (points[i] * transform).Dot(dVec);
		
		if (temp > theta)
		{
			theta = temp;
			index = i;
		}
	}

	m_index = index;
	return points[index] * transform;
}

Edge PolygonCollider::GetFeatureEdge(Vector2D dVec)
{
	Matrix3x3 transform = Matrix3x3::TransformMatrix(m_Position.X, m_Position.Y, m_Angle, m_Scale.X, m_Scale.Y);
	
	// ���� ���ͷκ��� ���� �� ���� ã��
	Vector2D curr = GetFurthestPoint(dVec);

	// �� ���� �¿� ���� ������
	size_t prev = (m_index - 1 + points.size()) % points.size();
	size_t next = (m_index + 1) % points.size();

	Vector2D prevP = points[prev] * transform;
	Vector2D nextP = points[next] * transform;
	
	// ���� ���ϰ� ����ȭ��.
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

