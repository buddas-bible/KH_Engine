#pragma once
#include "Vector2D.h"

class Object abstract
{
public:
	Object() : m_Position(), m_Angle(), m_Scale() {}
	~Object() {}

	virtual void Render() abstract;
	virtual void Update() abstract;

public:
	Vector2D m_Position;			// 위치
	float m_Angle;				// 회전
	Vector2D m_Scale;				// 축척
};