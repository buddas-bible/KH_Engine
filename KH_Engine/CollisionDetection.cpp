#include "CollisionDetection.h"

#include <iostream>

#include "Vector3D.h"
#include "Transform.h"

#include <cmath>

#include "LineCollider.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "PolygonCollider.h"

#define	GJK_MAXCOUNT 50
#define	EPA_MAXCOUNT 50

CollisionDetection::CollisionDetection()
{

}

CollisionDetection::~CollisionDetection()
{

}


float CollisionDetection::CounterClockwise(Vector2D A, Vector2D B)
{
	return A.Cross(B);
}

float CollisionDetection::CounterClockwise(Vector2D A, Vector2D B, Vector2D P)
{
	return CounterClockwise(A - P, B - P);
}



bool CollisionDetection::GJK(ObjectCollider*& A, ObjectCollider*& B)
{
	// 임의의 방향벡터
	Vector2D directionVec = { 1.f, 0.f };;

	// 방향벡터로부터 서포트 포인트를 구함
	std::vector<Vector2D> Simplex;

	// 원점
	Vector2D origin{ 0.f, 0.f };
	Vector2D temp;
	
	size_t count = 0;

	// while (count < GJK_MAXCOUNT)
	while (true)
	{
		switch (Simplex.size())
		{
			case 0:
			{
				// 서포트 포인트 A를 구함
				Simplex.push_back(GetSupportPoint(A, B, directionVec));
				directionVec = directionVec * -1.f;
			}
			break;

			case 1:
			{
				// 서포트 포인트 B를 구함
				Vector2D OA{ Simplex[0].Normalize() };
				Vector2D AO{ OA * -1.f };
				Simplex.push_back(GetSupportPoint(A, B, directionVec));

				// 서포트 포인트 B가 원점 너머에 있지 않으면 충돌 하지 않음
				Vector2D OB{ Simplex[1].Normalize() };

				/*
				if (OB.Dot(AO) < 0)
				{
					return false;
				}
				*/
			}
			break;

			case 2:
			{
				// 원점을 향한 방향 벡터를 구하기 위해서 삼중곱을 함
				Vector2D AB = (Simplex[1] - Simplex[0]);
				Vector2D AO = (Simplex[0] * -1.f);
				directionVec = TripleProduct(AB, AO, AB);
				directionVec = directionVec.Normalize();
				
				// AB 위에 원점이 있을 경우
				if (directionVec == origin)
				{
					directionVec = Vector2D(-AB.Y, AB.X).Normalize();
					// m_Simplex = Simplex;
					// return true;
				}

				// 서포트 포인트 C를 구함
				Simplex.push_back(GetSupportPoint(A, B, directionVec));

				// 서포트 포인트 C가 원점 너머에 있지 않으면 충돌하지 않음
				// Vector2D OB{ Simplex[1].Normalize() };
				Vector2D OC{ Simplex[2].Normalize() };

				if (OC.Dot(directionVec) <= 0)
				{
					return false;
				}
			}
			break;

			case 3:
			{
				// 점이 중복된다면 리턴
				if ((Simplex[0] == Simplex[2]) || 
					(Simplex[0] == Simplex[1]) || 
					(Simplex[1] == Simplex[2]))
				{
					return false;
				}
			
				Vector2D CA{ (Simplex[0] - Simplex[2]) };
				Vector2D CB{ (Simplex[1] - Simplex[2]) };
				Vector2D CO{ Simplex[2].Normalize() * -1.f };

				Vector2D nBC{ TripleProduct(CA, CB, CB) };
				Vector2D nAC{ TripleProduct(CB, CA, CA) };

				nBC = nBC.Normalize();
				nAC = nAC.Normalize();

				// AB에서 원점을 향해서 점을 찾기 시작한 것이니
				// nBC, nAC를 원점과 내적해서 각도가 제대로 나오는가 판단
				if (nBC.Dot(CO) <= 0 && nAC.Dot(CO) <= 0)
				{
					// 심플렉스 내부에 원점이 존재함

					// 현재 심플렉스 정보를 저장함
					m_Simplex = Simplex;
					
					EPA(A, B);
	
					FindContactPoint(manifold.normalVector, A, B);
					manifold.tangentVector = { -manifold.normalVector.Y, manifold.normalVector.X };
					return true;
				}

				if (nBC.Dot(CO) > 0)
				{
					// BC 영역 안에 원점이 존재함.
					directionVec = nBC;

					temp = Simplex[0];
					// 점 A 삭제 및 배열을 당김
					Simplex.erase(Simplex.begin() + 0);
					count++;
					break;
				}
				else if (nAC.Dot(CO) > 0)
				{
					// AC 영역 안에 원점이 존재함.
					directionVec = nAC;

					temp = Simplex[1];
					// 점 B 삭제 및 배열을 당김
					Simplex.erase(Simplex.begin() + 1);
					count++;
					break;
				}
			}
			break;
		}
	}
	return false;
}

Vector2D CollisionDetection::GetSupportPoint(ObjectCollider*& A, ObjectCollider*& B, const Vector2D& direction)
{
	return (A->GetFurthestPoint(direction)) - (B->GetFurthestPoint(direction * -1.f));
}


Vector2D CollisionDetection::EPA(ObjectCollider*& A, ObjectCollider*& B)
{
	size_t i = 0;

	while (i < EPA_MAXCOUNT)
	{
		// 가까운 변을 찾는다.
		MinkowskiEdge edge = GetSupportEdge(m_Simplex);

		// 가장 가까운 변의 노말 벡터를 방향벡터로 삼아 서포트 포인트를 구한다.
		Vector2D p = GetSupportPoint(A, B, edge.normal);

		// 서포트 포인트를 노말벡터에 투영해서 거리를 구한다.
		float dist = p.Dot(edge.normal);
		
		if (dist < 0.01f)
		{
			manifold.Clear();
			manifold.A = A->pRigidBody;
			manifold.B = B->pRigidBody;
			manifold.normalVector = edge.normal;
			manifold.depth = edge.dist;
			return (edge.normal * (edge.dist + 0.001f));
		}
		
		// 서포트 포인트를 노말벡터에 투영한 값이
		// 찾은 변의 정보와 유사하다면?
		if ((dist - edge.dist) < 0.01f)
		{
			// 반복 충돌을 방지하기 위해 아주 작은 값은 더해서 반환
			/// 벡터는 구했지만 어느 점에서 어느 변을 향한 벡터인지는 모른다.
			manifold.Clear();
			manifold.normalVector = edge.normal;
			manifold.depth = edge.dist;
			return (edge.normal * (edge.dist + 0.001f));
		}
		else
		{
			// 찾은 점을 심플렉스에 넣고 다시 탐색
			m_Simplex.insert(m_Simplex.begin() + edge.index, p);
			i++;
		}
	}

	return Vector2D(0, 0);
}


MinkowskiEdge CollisionDetection::GetSupportEdge(std::vector<Vector2D> Simplex)
{
	MinkowskiEdge edge;

	for (size_t i = 0; i < Simplex.size(); i++)
	{
		size_t j = (i + 1) % Simplex.size();

		// 변의 노말 벡터를 구한다.
		Vector2D AB{ Simplex[j] - Simplex[i] };

		// 변에 수직인 벡터를 구한다.
		// Vector2D normalVec = TripleProduct(AB, Simplex[i], AB).Normalize();

		// 심플렉스가 어떻게 감겼는지 체크
		Vector2D normalVec;

		// 음수면 시계, 양수면 반시계
		if (CounterClockwise(AB, Simplex[i]) > 0)
		{
			// OA와 AB가 시계로 나왔다면
			// 심플렉스는 시계 방향으로 감김
			normalVec = { Vector2D(-AB.Y, AB.X).Normalize() };
		}
		else
		{
			// 반시계 방향으로 감김
			normalVec = { Vector2D(AB.Y, -AB.X).Normalize() };
		}

		// 변의 수직인 벡터에 투영해서 원점과의 거리를 구한다.
		float dist = Simplex[i].Dot(normalVec);

		// 기존에 구한 것보다 거리가 가깝다면 저장.
		if (dist < edge.dist)
		{
			edge.dist = dist;
			edge.normal = normalVec;
			edge.index = j;
		}
	}

	// 거리가 가장 가까웠던 변의 정보를 반환
	return edge;
}

std::vector<Vector2D>& CollisionDetection::GetSimplex()
{
	return m_Simplex;
}

void CollisionDetection::FindContactPoint(const Vector2D& dVec, ObjectCollider*& A, ObjectCollider*& B)
{
	// 각 도형에 충돌한 것으로 추정되는 변을 가져옴
	Edge edgeA = A->GetFeatureEdge(dVec);
	Edge edgeB = B->GetFeatureEdge(-1.f * dVec);

	// 기준이 되는 변을 찾아야함.
	Edge* reference;
	Edge* incident;

	// 방향 벡터와 수직인지 체크
	float aPerpendicular = fabs(edgeA.directionVector.Dot(dVec));
	float bPerpendicular = fabs(edgeB.directionVector.Dot(dVec));

	// 방향벡터가 수직인 것을 찾아서 바꿈
	if (aPerpendicular <= bPerpendicular)
	{
		reference = &edgeA;
		incident = &edgeB;
		manifold.A = A->pRigidBody;
		manifold.B = B->pRigidBody;
		manifold.normalVector = dVec;
	}
	else
	{
		reference = &edgeB;
		incident = &edgeA;
		manifold.A = B->pRigidBody;
		manifold.B = A->pRigidBody;
		manifold.normalVector = dVec * -1.f;
	}

	// 원인 경우.
	if (A->Ctype == eColliderType::Type_Circle || B->Ctype == eColliderType::Type_Circle)
	{
		if (edgeA.directionVector.SizeSquare() == 0.f)
		{
			reference = &edgeB;
			incident = &edgeA;
			manifold.normalVector = dVec/* * -1.f*/;
		}
		else if (edgeB.directionVector.SizeSquare() == 0.f)
		{
			reference = &edgeA;
			incident = &edgeB;
			manifold.normalVector = dVec;
		}
	}

	/// a
	EdgeClip(incident, reference->Point1, reference->directionVector, false);
	EdgeClip(incident, reference->Point2, -1.f * reference->directionVector, false);
	EdgeClip(incident, reference->Point1, -1.f * manifold.normalVector, true);

	
	if (incident->GetLengthSquare() <= (0.002f * 0.001f))
	{
		// 충돌 지점이 하나.
 		manifold.contactPoint.push_back(incident->Point1);
	}
	else
	{
		// 충돌 지점이 둘.
		manifold.contactPoint.push_back(incident->Point1);
		manifold.contactPoint.push_back(incident->Point2);
	}

	// double max = reference->normalVector.Dot()

	manifold.referencePoint = reference->Point1;
}

void CollisionDetection::EdgeClip(Edge*& e, const Vector2D& p1, const Vector2D& dVec, bool remove)
{
	float d1 = (e->Point1 - p1).Dot(dVec);
	float d2 = (e->Point2 - p1).Dot(dVec);

	if (d1 >= 0.f && d2 >= 0.f)
	{
		return;
	}

	if (d1 < 0.f)
	{
		if (remove)
		{
			e->Point1 = e->Point2;
		}
		else
		{
			e->Point1 = 
				e->Point1 + (e->Point2 - e->Point1) * (-d1 / (fabs(d1) + fabs(d2)));
		}
	}
	if (d2 < 0.f)
	{
		if (remove)
		{
			e->Point2 = e->Point1;
		}
		else
		{
			e->Point2 =
				e->Point2 + (e->Point1 - e->Point2) * (-d2 / (fabs(d1) + fabs(d2)));
		}
	}
}

bool CollisionDetection::LineToLine(const LineCollider& A, const LineCollider& B)
{
	return LineToLine(
		WorldTransform(A.points, A.m_Position, A.m_Angle, A.m_Scale),
		WorldTransform(B.points, B.m_Position, B.m_Angle, B.m_Scale));
}

bool CollisionDetection::LineToLine(const std::vector<Vector2D>& Apoints, const std::vector<Vector2D>& Bpoints)
{
	float a1{ CounterClockwise(Apoints[0], Apoints[1], Bpoints[0]) };
	float a2{ CounterClockwise(Apoints[0], Apoints[1], Bpoints[1]) };

	float b1{ CounterClockwise(Bpoints[0], Bpoints[1], Apoints[0]) };
	float b2{ CounterClockwise(Bpoints[0], Bpoints[1], Apoints[1]) };

	if (a1 * a2 == 0.f && b1 * b2 == 0.f)
	{
		return false;
	}
	else if (a1 * a2 < 0.f && b1 * b2 < 0.f)
	{
		return true;
	}
	else if (a1 * a2 == 0.f && b1 * b2 < 0.f)
	{
		return true;
	}
	else if (a1 * a2 < 0.f && b1 * b2 == 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CollisionDetection::PointToCircle(const Vector2D& point, const  Vector2D& cPoint, const float radius)
{
	Vector2D length{ cPoint - point };
	if (length.SizeSquare() <= radius * radius)
	{
		return true;
	}

	return false;
}

bool CollisionDetection::CircleToCircle(const CircleCollider& obj1, const CircleCollider& obj2)
{
	return CircleToCircle(
		obj1.m_Position, obj1.GetRadius(),
		obj2.m_Position, obj2.GetRadius());
}

bool CollisionDetection::CircleToCircle(const Vector2D& obj1Pos, const float& obj1radius, const  Vector2D& obj2Pos, const float& obj2radius)
{
	Vector2D length{ obj1Pos - obj2Pos };
	float sumRadius{ (obj1radius + obj2radius) };
	if (length.SizeSquare() <= sumRadius * sumRadius)
	{
		return true;
	}

	return false;
}



bool CollisionDetection::LineToCircle(const LineCollider& line, const CircleCollider& circle)
{
	return LineToCircle(
		WorldTransform(line.points[0], line.m_Position, line.m_Angle, line.m_Scale),
		WorldTransform(line.points[1], line.m_Position, line.m_Angle, line.m_Scale),
		circle.m_Position, 
		circle.GetRadius());
}

bool CollisionDetection::LineToCircle(const Vector2D& lineP1, const Vector2D& lineP2, const Vector2D& circleP, const float circleR)
{
	// 점p2에서 점p1을 향한 벡터
	Vector2D P2ToP1{ lineP1 - lineP2 };
	Vector2D P1ToP2{ P2ToP1 * -1.f };

	// 점p2에서 점p1을 향한 벡터를 정규화한 벡터
	Vector2D nP2ToP1{ P2ToP1.Normalize() };
	Vector2D nP1ToP2{ nP2ToP1 * -1.f };

	// 점p2에서 원의 중심을 향한 벡터
	Vector2D P2ToCircle{ circleP - lineP2 };
	Vector2D P1ToCircle{ circleP - lineP1 };

	// 원 중심을 선분에 투영한 벡터
	Vector2D P2toO{ nP2ToP1 * P2ToCircle.Projection(nP2ToP1) };

	float thetaP2{ P2ToP1.FindTheta(P2ToCircle) };
	float thetaP1{ P1ToP2.FindTheta(P1ToCircle) };

	Vector2D circleToline{ P2toO - P2ToCircle };

	if (PointToCircle(lineP2, circleP, circleR))
	{
		return true;
	}
	if (PointToCircle(lineP1, circleP, circleR))
	{
		return true;
	}
	if (thetaP2 <= 1.5708f && thetaP1 <= 1.5708f)
	{
		if (circleToline.SizeSquare() <= circleR * circleR)
		{
			return true;
		}
	}

	return false;
}


bool CollisionDetection::CircleToOBB(const CircleCollider& A, const PolygonCollider& B)
{
	return CircleToOBB(
		A.m_Position, A.GetRadius(), B.m_Position, 
		WorldTransform(B.points, B.m_Position, B.m_Angle, B.m_Scale));
}

bool CollisionDetection::CircleToOBB(const Vector2D& Apos, const float Aradius, const Vector2D Bpos, const std::vector<Vector2D>& Bpoints)
{
	std::vector<LineCollider> edge;

	// 폴리곤의 각 변을 선분 콜라이더로 분해함
	// 선분들과 원의 충돌로 처리함
	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		size_t j = (i + 1) % Bpoints.size();

		edge.push_back(LineCollider(Bpoints[j], Bpoints[i]));
	
		if (LineToCircle(edge[i], CircleCollider(Aradius, Apos)))
		{
			return true;
		}
	}
	std::vector<Vector2D> vertexToCCenter;
	std::vector<Vector2D> edgevector;

	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		vertexToCCenter.push_back(Apos - Bpoints[i]);
	}
	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		size_t j = (i + 1) % Bpoints.size();

		edgevector.push_back(Bpoints[i] - Bpoints[j]);
	}


	std::vector<float> temp(Bpoints.size());
	
	// 폴리곤 안에 원이 들어가있는 경우 체크
	// 선분과 원의 중심을 외적해서 모든 외적 값이 일관적이면 안에 들어있다.
	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		temp[i] = vertexToCCenter[i].Cross(edgevector[i]);
	}

	bool x = (temp[0] > 0.f);

	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		if ((temp[i] > 0.f) == x)
		{
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CollisionDetection::AABB(const std::vector<Vector2D> Apoints, const std::vector<Vector2D> Bpoints)
{
	Vector2D ALefttop = Apoints[0];
	Vector2D ARightbottom = Apoints[0];

	for (size_t i = 0; i < Apoints.size(); i++)
	{
		ALefttop.X = (ALefttop.X > Apoints[i].X ? Apoints[i].X : ALefttop.X);
		ALefttop.Y = (ALefttop.Y > Apoints[i].Y ? Apoints[i].Y : ALefttop.Y);

		ARightbottom.X = (ARightbottom.X < Apoints[i].X ? Apoints[i].X : ARightbottom.X);
		ARightbottom.Y = (ARightbottom.Y < Apoints[i].Y ? Apoints[i].Y : ARightbottom.Y);
	}

	Vector2D BLefttop = Bpoints[0];
	Vector2D BRightbottom = Bpoints[0];
	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		BLefttop.X = (BLefttop.X > Bpoints[i].X ? Bpoints[i].X : BLefttop.X);
		BLefttop.Y = (BLefttop.Y > Bpoints[i].Y ? Bpoints[i].Y : BLefttop.Y);

		BRightbottom.X = (BRightbottom.X < Bpoints[i].X ? Bpoints[i].X : BRightbottom.X);
		BRightbottom.Y = (BRightbottom.Y < Bpoints[i].Y ? Bpoints[i].Y : BRightbottom.Y);
	}

	return AABB(
		ALefttop, ARightbottom,
		BLefttop, BRightbottom);
}

bool CollisionDetection::AABB(
	const Vector2D& box1Lt, const Vector2D& box1Rb,
	const Vector2D& box2Lt, const  Vector2D& box2Rb)
{
	if ((box1Rb.X < box2Lt.X) || (box1Lt.X > box2Rb.X))
	{
		return false;
	}
	if ((box1Rb.Y < box2Lt.Y) || (box1Lt.Y > box2Rb.Y))
	{
		return false;
	}

	return true;
}

bool CollisionDetection::AABB(ObjectCollider* obj1, ObjectCollider* obj2)
{
	std::vector<Vector2D> obj1Point;
	std::vector<Vector2D> obj2Point;

	if ((obj1 == nullptr) || (obj2 == nullptr))
	{
		return false;
	}

	if (obj1->Ctype == eColliderType::Type_Box)
	{
		BoxCollider* _obj1 = dynamic_cast<BoxCollider*>(obj1);
		obj1Point = WorldTransform(_obj1->points, _obj1->m_Position, _obj1->m_Angle, _obj1->m_Scale);
	}
	else if (obj1->Ctype == eColliderType::Type_Circle)
	{
		CircleCollider* _obj1 = dynamic_cast<CircleCollider*>(obj1);
		obj1Point.push_back(Vector2D(_obj1->m_Position.X - _obj1->GetRadius(), _obj1->m_Position.Y - _obj1->GetRadius()));
		obj1Point.push_back(Vector2D(_obj1->m_Position.X + _obj1->GetRadius(), _obj1->m_Position.Y + _obj1->GetRadius()));
	}
	else if (obj1->Ctype == eColliderType::Type_Line)
	{
		LineCollider* _obj1 = dynamic_cast<LineCollider*>(obj1);
		obj1Point = WorldTransform(_obj1->points, _obj1->m_Position, _obj1->m_Angle, _obj1->m_Scale);
	}
	else if (obj1->Ctype == eColliderType::Type_Polygon)
	{
		PolygonCollider* _obj1 = dynamic_cast<PolygonCollider*>(obj1);
		obj1Point = WorldTransform(_obj1->points, _obj1->m_Position, _obj1->m_Angle, _obj1->m_Scale);
	}
	else if (obj1->Ctype == eColliderType::Type_Box)
	{
		BoxCollider* _obj1 = dynamic_cast<BoxCollider*>(obj1);
		obj1Point = WorldTransform(_obj1->points, _obj1->m_Position, _obj1->m_Angle, _obj1->m_Scale);
	}
	else
	{
		return false;
	}


	if (obj2->Ctype == eColliderType::Type_Box)
	{
		BoxCollider* _obj2 = dynamic_cast<BoxCollider*>(obj2);
		obj2Point = WorldTransform(_obj2->points, _obj2->m_Position, _obj2->m_Angle, _obj2->m_Scale);
	}
	else if (obj2->Ctype == eColliderType::Type_Circle)
	{
		CircleCollider* _obj2 = dynamic_cast<CircleCollider*>(obj2);
		obj2Point.push_back(Vector2D(_obj2->m_Position.X - _obj2->GetRadius(), _obj2->m_Position.Y - _obj2->GetRadius()));
		obj2Point.push_back(Vector2D(_obj2->m_Position.X + _obj2->GetRadius(), _obj2->m_Position.Y + _obj2->GetRadius()));
	}
	else if (obj2->Ctype == eColliderType::Type_Line)
	{
		LineCollider* _obj2 = dynamic_cast<LineCollider*>(obj2);
		obj2Point = WorldTransform(_obj2->points, _obj2->m_Position, _obj2->m_Angle, _obj2->m_Scale);
	}
	else if (obj2->Ctype == eColliderType::Type_Polygon)
	{
		PolygonCollider* _obj2 = dynamic_cast<PolygonCollider*>(obj2);
		obj2Point = WorldTransform(_obj2->points, _obj2->m_Position, _obj2->m_Angle, _obj2->m_Scale);
	}
	else if (obj2->Ctype == eColliderType::Type_Box)
	{
		BoxCollider* _obj2 = dynamic_cast<BoxCollider*>(obj1);
		obj2Point = WorldTransform(_obj2->points, _obj2->m_Position, _obj2->m_Angle, _obj2->m_Scale);
	}
	else
	{
		return false;
	}

	return AABB(obj1Point, obj2Point);
}



bool CollisionDetection::OBB(const PolygonCollider& A, const  PolygonCollider& B)
{
	return OBB(
		WorldTransform(A.points, A.m_Position, A.m_Angle, A.m_Scale),
		WorldTransform(B.points, B.m_Position, B.m_Angle, B.m_Scale)
	);
}

/// 새로 해야할 OBB 정리
// 두 도형의 꼭짓점을 알고있음
// 꼭짓점에서 꼭짓점을 빼서 변 벡터를 구함
// 변 벡터를 축으로 하여 원점 기준으로 꼭짓점을 투영함
// 두 도형 모두 최소, 최대값이 나올 것
// 그 최대 최소가 겹치는지 체크
// 모든 축에 대해서 반복

bool CollisionDetection::OBB(const std::vector<Vector2D>& Apoints, const  std::vector<Vector2D>& Bpoints)
{
	std::vector<Vector2D> Avertex = Apoints;
	std::vector<Vector2D> Bvertex = Bpoints;
	std::vector<Vector2D> Aedge;
	std::vector<Vector2D> Bedge;

	for (size_t i = 0; i < Apoints.size(); i++)
	{
		size_t j = i + 1 % Apoints.size();

		Aedge.push_back(Apoints[j] - Apoints[i]);
	}
	for (size_t i = 0; i < Bpoints.size(); i++)
	{
		size_t j = i + 1 % Bpoints.size();

		Bedge.push_back(Bpoints[j] - Bpoints[i]);
	}

	/// A의 변을 축으로 해서 거리 비교
	for (size_t i = 0; i < Aedge.size(); i++)
	{
		Vector2D UnitVec = Aedge[i].Normalize();

		UnitVec = { UnitVec.Y * -1.f , UnitVec.X };

		float Amin{ (Avertex[0].Projection(UnitVec)) };
		float Amax{ Amin };

		for (size_t j = 1; j < Avertex.size(); j++)
		{
			float d = (Avertex[j].Projection(UnitVec));
			Amin = Amin < d ? Amin : d;
			Amax = Amax > d ? Amax : d;
		}

		float Bmin{ (Bvertex[0].Projection(UnitVec)) };
		float Bmax{ Bmin };

		for (size_t j = 1; j < Bvertex.size(); j++)
		{
			float d = (Bvertex[j].Projection(UnitVec));
			Bmin = Bmin < d ? Bmin : d;
			Bmax = Bmax > d ? Bmax : d;
		}

		if ((Amax < Bmin) || (Amin > Bmax))
		{
			return false;
		}
	}

	/// B의 변을 축으로 해서 거리 비교
	for (size_t i = 0; i < Bedge.size(); i++)
	{
		Vector2D UnitVec = Bedge[i].Normalize();

		UnitVec = { UnitVec.Y * -1.f , UnitVec.X };

		float Amin{ (Avertex[0].Projection(UnitVec)) };
		float Amax{ Amin };

		for (size_t j = 1; j < Avertex.size(); j++)
		{
			float d = (Avertex[j].Projection(UnitVec));
			Amin = Amin < d ? Amin : d;
			Amax = Amax > d ? Amax : d;
		}

		float Bmin{ (Bvertex[0].Projection(UnitVec)) };
		float Bmax{ Bmin };

		for (size_t j = 1; j < Bvertex.size(); j++)
		{
			float d = (Bvertex[j].Projection(UnitVec));
			Bmin = Bmin < d ? Bmin : d;
			Bmax = Bmax > d ? Bmax : d;
		}

		if ((Amax < Bmin) || (Amin > Bmax))
		{
			return false;
		}
	}

	return true;
}

bool CollisionDetection::LineToOBB(const LineCollider& A, const PolygonCollider& B)
{
	return LineToOBB(
		A.m_Position, 
		WorldTransform(A.points, A.m_Position, A.m_Angle, A.m_Scale),
		B.m_Position, 
		WorldTransform(B.points, B.m_Position, B.m_Angle, B.m_Scale)
	);
}

bool CollisionDetection::LineToOBB(
	const Vector2D& APos, const std::vector<Vector2D>& APoints,
	const Vector2D& BPos, const std::vector<Vector2D>& BPoints)
{
	std::vector<LineCollider> Bedge;
	std::vector<Vector2D> unitVec;

	for (size_t i = 0; i < BPoints.size(); i++)
	{
		if (i == (BPoints.size() - 1))
		{
			Bedge.push_back(LineCollider(BPoints[i], BPoints[0]));
			unitVec.push_back(BPoints[i] - BPoints[0]);
		}
		else
		{
			Bedge.push_back(LineCollider(BPoints[i], BPoints[i + 1]));
			unitVec.push_back(BPoints[i] - BPoints[i + 1]);
		}
		if (LineToLine(LineCollider(APoints[0], APoints[1]), Bedge[i]))
		{
			return true;
		}
	}

	// 폴리곤 안에 원이 들어가있는 경우 체크
	// 선분과 원의 중심을 외적해서 모든 외적 값이 일관적이면 안에 들어있다.
	std::vector<float> temp0(BPoints.size());
	std::vector<float> temp1(BPoints.size());

	for (size_t i = 0; i < BPoints.size(); i++)
	{
		temp0[i] = unitVec[i].Cross(APoints[0] - BPoints[i]);
		temp1[i] = unitVec[i].Cross(APoints[1] - BPoints[i]);
	}

	bool x0 = (temp0[0] < 0.f);
	bool x1 = (temp1[0] < 0.f); 

	for (size_t i = 0; i < BPoints.size(); i++)
	{
		if ((temp0[i] < 0.f) == x0)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	for (size_t i = 0; i < BPoints.size(); i++)
	{
		if ((temp0[i] < 0.f) == x1)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}