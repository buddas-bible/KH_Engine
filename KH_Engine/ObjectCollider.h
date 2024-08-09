#pragma once
#include "KH_Define.h"
#include "Vector2D.h"
#include "ColliderList.h"
#include "RigidBody.h"
#include "Edge.h"
#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")


class ColliderList;

class ObjectCollider abstract
{
protected:
	ColliderList* pList = ColliderList::GetInstance();
	Graphics* graphics = Graphics::GetInstance();

public:
	ObjectCollider()
	{}
	virtual ~ObjectCollider()
	{}


public:
	RigidBody* pRigidBody;
	ColliderType Ctype;
	bool isCollision = false;

public:
	Vector2D m_Position;		// ��ġ
	float m_Angle;				// ȸ��
	Vector2D m_Scale;			// ��ô

public:
	virtual void Render() abstract;
	virtual void AABBDebugRender() abstract;
	virtual void Update(float dT) abstract;

public:
	virtual ColliderType GetType() abstract;					// �ݸ��� Ÿ��
	virtual void ComputeMass(float density) abstract;			// ����
	virtual Vector2D GetFurthestPoint(Vector2D dVec) abstract;	// �𷺼Ǻ��ͷκ��� �� ���� ��ȯ
	virtual Edge GetFeatureEdge(Vector2D dVec) abstract;
};
