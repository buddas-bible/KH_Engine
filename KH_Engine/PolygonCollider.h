#pragma once
#include "ObjectCollider.h"
#include "vector"

class PolygonCollider :
	public ObjectCollider
{
public:
	PolygonCollider(float radius);
	PolygonCollider(
		size_t vertex,
		float radius,
		Vector2D pos,
		RigidBody* rigid);

	virtual ~PolygonCollider();

public:
	std::vector<Vector2D> points;

private:
	/// AABB 체크를 위한 임시 배열
	std::vector<Vector2D> temp;
	std::vector<Vector2D> LTRB;
	void Initialize(size_t n, float radius);

	size_t m_index;

public:
	virtual void Render() override;
	virtual void AABBDebugRender() override;
	virtual void Update(float dT) override;

public:
	virtual ColliderType	GetType() override;
	virtual void ComputeMass(float density) override;				// 질량
	virtual Vector2D GetFurthestPoint(Vector2D dVec) override;
	virtual Edge GetFeatureEdge(Vector2D dVec) override;
};