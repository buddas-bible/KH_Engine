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
/// 충돌 이벤트를 처리하는 클래스
/// 
/// 지금은 사용하지 않음.
/// 매니폴드가 대체하고 있음.
/// </summary>
class OnCollision
{
private:
	static float GetImpulse(RigidBody* A, RigidBody* B);
	static float GetFriction(float& fricA, float& fricB);
	static float GetRestitution(float& resA, float& resB);


public:
	/// 원과 원이 충돌 시 속력 변화 이벤트
	static void EventCircleToCircle(CircleCollider& circle1, CircleCollider& circle2);
	
	/// 원과 선이 충돌시 속력 변화 이벤트
	static void EventCircleToLine(CircleCollider& circle1, LineCollider& line1);
	
	/// 
	static void CollisionEvent(ObjectCollider* A, ObjectCollider* B, Manifold& maniflod);

};
