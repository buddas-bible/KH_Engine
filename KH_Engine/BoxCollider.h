#pragma once
#include "ObjectCollider.h"
#include "vector"

class ObjectCollider;

class BoxCollider :
	public ObjectCollider
{
public:
	BoxCollider(Vector2D radius);
	BoxCollider(
		Vector2D radius,
		Vector2D pos,
		RigidBody* rigid);

	virtual ~BoxCollider();


	/// 각 꼭짓점
	std::vector<Vector2D> points;

private:
	/// AABB 디버그 정보를 위한 임시 배열
	std::vector<Vector2D> temp;
	std::vector<Vector2D> LTRB;
	size_t m_index;

public:
	/// 매번 변환해서 그림을 그림
	virtual void Render() override;
	virtual void AABBDebugRender() override;
	virtual void Update(float dT) override;

public:
	virtual ColliderType	GetType() override;
	virtual void			ComputeMass(float density) override;				// 질량
	virtual Vector2D GetFurthestPoint(Vector2D dVec) override;
	virtual Edge GetFeatureEdge(Vector2D dVec) override;
};
