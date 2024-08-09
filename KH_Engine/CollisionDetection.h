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
	/// GJK (Gilbert Johnson Keerthi) �˰���
	/// 
	/// ���� ���ͷκ��� ���� �� ���� ���ϰ� ���� ���� ����Ʈ ����Ʈ p1�� ���Ѵ�.
	/// ���� ���͸� �ݴ�� �ؼ� ���ο� ����Ʈ ����Ʈ p2�� ���Ѵ�.
	/// �� ���� �̿��� �������� ���� ������ ã�� ����Ʈ ����Ʈ p3�� �ϳ� �� ã�´�.
	/// �� ���� �̷�� ���÷����� �ȿ� ������ �ִ��� Ȯ��
	/// ������ p1-p3, p2-p3 �������� ���� ���� ã�� Ž��
	bool GJK(ObjectCollider*& A, ObjectCollider*& B);
	std::vector<Vector2D>& GetSimplex();

private:
	Vector2D GetSupportPoint(ObjectCollider*& A, ObjectCollider*& B, const Vector2D& direction);
#pragma endregion GJK
	
#pragma region EPA
public:
	/// EPA (Expanding Polytope Algorithm)
	///
	/// GJK�� ã�� ���÷������� �������κ��� ���� ����� ���� ã��
	/// �� ���� ������ ���⺤�ͷ� ����Ʈ ����Ʈ�� ã�� ���÷����� Ȯ���Ų��.
	/// ���� �ݺ��ϸ鼭
	/// �Ÿ��� ���� ������ �� �̻� Ȯ���� �� ���� ���� ã����
	/// ������ �� ������ �Ÿ��� ��ȯ�Ѵ�.
	Vector2D EPA(ObjectCollider*& A, ObjectCollider*& B);

	Manifold manifold;

private:
	/// ����� ���� ������ ���ϴ� �Լ�
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
	/// �� ���� �浹 üũ
	bool LineToLine(const LineCollider& A, const LineCollider& B);

	/// �� ���� �浹 üũ
	bool CircleToCircle(
		const CircleCollider& A,
		const CircleCollider& B);

	/// ���а� ���� �浹 üũ
	bool LineToCircle(const LineCollider& line, const CircleCollider& circle);

	/// �и��� �簢���� ���� �浹 üũ
	bool CircleToOBB(const CircleCollider& A, const PolygonCollider& B);

	/// ���ĵ� �� �簢���� �浹 üũ
	bool AABB(
		ObjectCollider* obj1,
		ObjectCollider* obj2);

	/// ���� �и��� �� �簢���� �浹 üũ
	bool OBB(
		const PolygonCollider& A,
		const PolygonCollider& B);

	bool LineToOBB(
		const LineCollider& A,
		const PolygonCollider& B);

private:
	/// �� �ȿ� ���� �ִ��� üũ
	bool PointToCircle(
		const Vector2D& point,
		const Vector2D& cPoint,
		const float radius);

	/// �� ���� �浹 üũ
	bool LineToLine(const std::vector<Vector2D>& Apoints, const std::vector<Vector2D>& Bpoints);

	/// CCW
	/// �ð�����̸� ����, �ݽð� �����̸� ���
	float CounterClockwise(const Vector2D A, const Vector2D B);
	float CounterClockwise(const Vector2D A, const Vector2D B, const Vector2D P);

	/// �� ���� �浹 üũ
	bool CircleToCircle(
		const Vector2D& obj1Pos, const float& obj1radius,
		const Vector2D& obj2Pos, const float& obj2radius);

	/// ���а� ���� �浹 üũ
	bool LineToCircle(const Vector2D& lineP1, const Vector2D& lineP2, const Vector2D& circleP, const float circleR);

	bool CircleToOBB(
		const Vector2D& Apos, const float Aradius,
		const Vector2D Bpos, const std::vector<Vector2D>& Bpoints);

	bool AABB(
		const std::vector<Vector2D> Apoints,
		const std::vector<Vector2D> Bpoints);
	
	/// ���ĵ� �� �簢���� �浹 üũ
	bool AABB(
		const Vector2D& box1Lt, const Vector2D& box1Rb,
		const Vector2D& box2Lt, const  Vector2D& box2Rb);

	/// ���� �и��� �� �簢���� �浹 üũ
	bool OBB(
		const std::vector<Vector2D>& Avertex,
		const std::vector<Vector2D>& Bvertex);

	bool LineToOBB(
		const Vector2D& APos, const std::vector<Vector2D>& APoints,
		const Vector2D& BPos, const std::vector<Vector2D>& BPoints);
};


