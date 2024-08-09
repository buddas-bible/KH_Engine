#pragma once
#include "ObjectCollider.h"


class CircleCollider :
	public ObjectCollider
{
public:
	CircleCollider() : radius(40.f) {}
	CircleCollider(float radius, Vector2D pos);
	CircleCollider(
		float radius,
		Vector2D pos,
		RigidBody* rigid);

	virtual ~CircleCollider();

private:
	float radius;

public:
	float GetRadius() const;


private:
	/// AABB 체크를 위한 임시 배열
	std::vector<Vector2D> temp;
	std::vector<Vector2D> LTRB;

public:
	virtual void Render() override;
	virtual void AABBDebugRender() override;
	virtual void Update(float dT) override;
public:
	virtual ColliderType	GetType() override;
	virtual void			ComputeMass(float density) override;				// 질량
	virtual Vector2D GetFurthestPoint(Vector2D dVec) override;
	virtual Edge GetFeatureEdge(Vector2D dVec) override;
};
