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
	Vector2D m_Position;		// 위치
	float m_Angle;				// 회전
	Vector2D m_Scale;			// 축척

public:
	virtual void Render() abstract;
	virtual void AABBDebugRender() abstract;
	virtual void Update(float dT) abstract;

public:
	virtual ColliderType GetType() abstract;					// 콜리전 타입
	virtual void ComputeMass(float density) abstract;			// 질량
	virtual Vector2D GetFurthestPoint(Vector2D dVec) abstract;	// 디렉션벡터로부터 먼 점을 반환
	virtual Edge GetFeatureEdge(Vector2D dVec) abstract;
};
