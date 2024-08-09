#pragma once
#include <vector>

#include "Vector2D.h"
#include "Edge.h"
#include "Manifold.h"

#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")

class Vector2D;
class ObjectCollider;
class LineCollider;
class CircleCollider;
class BoxCollider;
class PolygonCollider;

struct MinkowskiEdge
{
	size_t index;
	float dist;
	Vector2D normal;

	MinkowskiEdge() : index(0), dist(INFINITY), normal(0.f,0.f)
	{ }
};

class CollisionDetection
{
public:
	CollisionDetection();
	~CollisionDetection();



public:
	// std::vector<std::vector<size_t>> m_currCollision{};
	// std::vector<std::vector<size_t>> m_prevCollision{};

#pragma region GJK
public:
	/// GJK (Gilbert Johnson Keerthi) 알고리즘
	/// 
	/// 방향 벡터로부터 가장 먼 점을 구하고 점을 통해 서포트 포인트 p1를 구한다.
	/// 방향 벡터를 반대로 해서 새로운 서포트 포인트 p2를 구한다.
	/// 두 점을 이용한 선분으로 원점 방향을 찾고 서포트 포인트 p3를 하나 더 찾는다.
	/// 세 점을 이루는 심플렉스에 안에 원점이 있는지 확인
	/// 없으면 p1-p3, p2-p3 선분으로 원점 방향 찾고 탐색
	bool GJK(ObjectCollider*& A, ObjectCollider*& B);
	std::vector<Vector2D>& GetSimplex();

private:
	Vector2D GetSupportPoint(ObjectCollider*& A, ObjectCollider*& B, const Vector2D& direction);
#pragma endregion GJK
	
#pragma region EPA
public:
	/// EPA (Expanding Polytope Algorithm)
	///
	/// GJK로 찾은 심플렉스에서 중점으로부터 가장 가까운 변을 찾아
	/// 그 변의 수직인 방향벡터로 서포트 포인트를 찾고 심플렉스를 확장시킨다.
	/// 위를 반복하면서
	/// 거리가 가장 가깝고 더 이상 확장할 수 없는 변을 찾으면
	/// 중점과 그 변과의 거리를 반환한다.
	Vector2D EPA(ObjectCollider*& A, ObjectCollider*& B);

	Manifold manifold;

private:
	/// 가까운 변의 정보를 구하는 함수
	MinkowskiEdge GetSupportEdge(
		std::vector<Vector2D> Simplex);

	std::vector<Vector2D> m_Simplex;
#pragma endregion EPA

#pragma region Manifold
public:
	void FindContactPoint(const Vector2D& dVec, ObjectCollider*& A, ObjectCollider*& B);
	void EdgeClip(Edge*& e, const Vector2D& p1, const Vector2D& dVec, bool remove);

private:

#pragma endregion Manifold

public:
	/// 두 선의 충돌 체크
	bool LineToLine(const LineCollider& A, const LineCollider& B);

	/// 두 원의 충돌 체크
	bool CircleToCircle(
		const CircleCollider& A,
		const CircleCollider& B);

	/// 선분과 원의 충돌 체크
	bool LineToCircle(const LineCollider& line, const CircleCollider& circle);

	/// 분리축 사각형과 원의 충돌 체크
	bool CircleToOBB(const CircleCollider& A, const PolygonCollider& B);

	/// 정렬된 두 사각형의 충돌 체크
	bool AABB(
		ObjectCollider* obj1,
		ObjectCollider* obj2);

	/// 축이 분리된 두 사각형의 충돌 체크
	bool OBB(
		const PolygonCollider& A,
		const PolygonCollider& B);

	bool LineToOBB(
		const LineCollider& A,
		const PolygonCollider& B);

private:
	/// 원 안에 점이 있는지 체크
	bool PointToCircle(
		const Vector2D& point,
		const Vector2D& cPoint,
		const float radius);

	/// 두 선의 충돌 체크
	bool LineToLine(const std::vector<Vector2D>& Apoints, const std::vector<Vector2D>& Bpoints);

	/// CCW
	/// 시계방향이면 음수, 반시계 방향이면 양수
	float CounterClockwise(const Vector2D A, const Vector2D B);
	float CounterClockwise(const Vector2D A, const Vector2D B, const Vector2D P);

	/// 두 원의 충돌 체크
	bool CircleToCircle(
		const Vector2D& obj1Pos, const float& obj1radius,
		const Vector2D& obj2Pos, const float& obj2radius);

	/// 선분과 원의 충돌 체크
	bool LineToCircle(const Vector2D& lineP1, const Vector2D& lineP2, const Vector2D& circleP, const float circleR);

	bool CircleToOBB(
		const Vector2D& Apos, const float Aradius,
		const Vector2D Bpos, const std::vector<Vector2D>& Bpoints);

	bool AABB(
		const std::vector<Vector2D> Apoints,
		const std::vector<Vector2D> Bpoints);
	
	/// 정렬된 두 사각형의 충돌 체크
	bool AABB(
		const Vector2D& box1Lt, const Vector2D& box1Rb,
		const Vector2D& box2Lt, const  Vector2D& box2Rb);

	/// 축이 분리된 두 사각형의 충돌 체크
	bool OBB(
		const std::vector<Vector2D>& Avertex,
		const std::vector<Vector2D>& Bvertex);

	bool LineToOBB(
		const Vector2D& APos, const std::vector<Vector2D>& APoints,
		const Vector2D& BPos, const std::vector<Vector2D>& BPoints);
};


