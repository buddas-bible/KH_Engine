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


	/// �� ������
	std::vector<Vector2D> points;

private:
	/// AABB ����� ������ ���� �ӽ� �迭
	std::vector<Vector2D> temp;
	std::vector<Vector2D> LTRB;
	size_t m_index;

public:
	/// �Ź� ��ȯ�ؼ� �׸��� �׸�
	virtual void Render() override;
	virtual void AABBDebugRender() override;
	virtual void Update(float dT) override;

public:
	virtual ColliderType	GetType() override;
	virtual void			ComputeMass(float density) override;				// ����
	virtual Vector2D GetFurthestPoint(Vector2D dVec) override;
	virtual Edge GetFeatureEdge(Vector2D dVec) override;
};
