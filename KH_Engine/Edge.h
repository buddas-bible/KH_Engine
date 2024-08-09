#pragma once
#include "Vector2D.h"

class Edge
{
public:
	Edge(const Vector2D& p1, const Vector2D& p2, size_t i1, size_t i2) :
		Point1(p1), Point2(p2), index1(i1), index2(i2)
	{
		Init();
	}

	Edge(const Vector2D& p1, const Vector2D& p2) :
		Point1(p1), Point2(p2), index1(-1), index2(-1)
	{
		Init();
	}

	~Edge()
	{}

	void Init() 
	{
		if (Point1 == Point2)
		{
			directionVector = {};
		}
		else
		{
			directionVector = (Point2 - Point1).Normalize();
		}
		normalVector = { -1.f * directionVector.Y, directionVector.X };
	}
	float GetLength() const
	{
		return (Point1 - Point2).Size();
	}
	float GetLengthSquare() const
	{
		return (Point1 - Point2).SizeSquare();
	}

	Vector2D Point1;
	Vector2D Point2;

	size_t index1;
	size_t index2;

	Vector2D directionVector;
	Vector2D normalVector;
};