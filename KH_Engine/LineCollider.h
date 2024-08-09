#pragma once
#include "ObjectCollider.h"
#include "vector"

class LineCollider :
	public ObjectCollider
{
public:
	LineCollider(float length);
	LineCollider(
		float length,
		Vector2D pos,
		RigidBody* rigid);


	LineCollider(Vector2D p1, Vector2D p2);
	virtual ~LineCollider();

	std::vector<Vector2D> points;

private:
	/// AABB 체크를 위한 임시 배열
	std::vector<Vector2D> temp;
	std::vector<Vector2D> LTRB;
	size_t m_index;

public:
	virtual void Render() override;
	virtual void AABBDebugRender() override;
	virtual void Update(float dT) override;

public:
	virtual ColliderType	GetType() override;
	virtual void			ComputeMass(float density) override;
	virtual Vector2D GetFurthestPoint(Vector2D dVec) override;
	virtual Edge GetFeatureEdge(Vector2D dVec) override;
};

