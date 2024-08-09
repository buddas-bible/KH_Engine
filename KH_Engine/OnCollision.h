#pragma once
#include <vector>
#include "Manifold.h"
class Vector2D;
class ObjectCollider;
class LineCollider;
class CircleCollider;
class BoxCollider;
class PolygonCollider;
class RigidBody;

/// <summary>
/// �浹 �̺�Ʈ�� ó���ϴ� Ŭ����
/// 
/// ������ ������� ����.
/// �Ŵ����尡 ��ü�ϰ� ����.
/// </summary>
class OnCollision
{
private:
	static float GetImpulse(RigidBody* A, RigidBody* B);
	static float GetFriction(float& fricA, float& fricB);
	static float GetRestitution(float& resA, float& resB);


public:
	/// ���� ���� �浹 �� �ӷ� ��ȭ �̺�Ʈ
	static void EventCircleToCircle(CircleCollider& circle1, CircleCollider& circle2);
	
	/// ���� ���� �浹�� �ӷ� ��ȭ �̺�Ʈ
	static void EventCircleToLine(CircleCollider& circle1, LineCollider& line1);
	
	/// 
	static void CollisionEvent(ObjectCollider* A, ObjectCollider* B, Manifold& maniflod);

};
