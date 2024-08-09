#pragma once

///	충돌체의 타입을 만듬
typedef unsigned int ColliderType;

/// 충돌체의 아이디 타입을 만듬
typedef unsigned int ColliderID;

typedef unsigned int CollisionType;

class eColliderType
{
public:
	enum
	{
		Type_None = 0,
		Type_Line = 1,
		Type_Circle = 2,
		Type_Capsule = 3,
		Type_Box = 4,
		Type_Polygon = 5,
	};
};


class eCollisionType
{
public:
	enum
	{
		Static_Collision = 0,
		Dynamic_Collision = 1,
	};
};


#define DELTATIME TimeManager::GetInstance().GetfDeltaTime()
#define fFPS TimeManager::GetInstance().GetFPS()
#define PI 3.141592f
#define RADIAN(degree) (degree * PI / 180.f)
#define DEGREE(radian) (radian * 180.f / PI)