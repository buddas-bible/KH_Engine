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
	Vector2D m_Position;			// ��ġ
	float m_Angle;				// ȸ��
	Vector2D m_Scale;				// ��ô
};