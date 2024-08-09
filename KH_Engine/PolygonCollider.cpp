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

	// 0번 인덱스로부터 반시계 방향으로 점을 찍음
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
	Vector2D c = {};					// 무게 중심
	float area = 0;						// 전체 면적
	float inertia = 0;					// 관성 모멘트
	const float inv3 = 1.f / 3.f;		// 3의 역수

	for (size_t i = 0; i < points.size(); i++)
	{
		size_t j = (i + 1) % points.size();
		Vector2D v1 = points[i] * Matrix3x3::ScaleRotate(m_Scale.X, m_Scale.Y, m_Angle);
		Vector2D v2 = points[j] * Matrix3x3::ScaleRotate(m_Scale.X, m_Scale.Y, m_Angle);

		// 삼각형의 면적 : 외적 / 2
		// 원점을 중심으로 삼각형을 만들기 때문에
		// 점 두개만 알고 있어도 삼각형의 면적을 구할 수 있음
		float A = v1.Cross(v2);
		float T = A * 0.5f;

		// 면적을 누적시킴
		area += T;

		// 무게중심을 구하기 위한 누적
		// 무게 중심 = ((삼각형의 면적) * (세 좌표의 합 / 3)) / 전체 면적
		// c += T * inv3 * (v1 + v2);

		// 심플 폴리곤에 대한 관성 모멘트 공식
		// x축에 대한 모멘트 = 1/12 * 삼각형의 면적 * {x1^2 + x1x2 + x2^2}
		// y축에 대한 모멘트 = 1/12 * 삼각형의 면적 * {y1^2 + y1x2 + y2^2}
		// 관성 모멘트는 두 축에 대해서 구한 값을 더하면 됨.
		// 1 / 12로 만들기위해 0.25를 하는 것.
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
	
	// 방향 벡터로부터 가장 먼 점을 찾음
	Vector2D curr = GetFurthestPoint(dVec);

	// 그 점의 좌우 점을 가져옴
	size_t prev = (m_index - 1 + points.size()) % points.size();
	size_t next = (m_index + 1) % points.size();

	Vector2D prevP = points[prev] * transform;
	Vector2D nextP = points[next] * transform;
	
	// 변을 구하고 정규화함.
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

