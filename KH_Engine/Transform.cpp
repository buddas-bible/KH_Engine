#include "Transform.h"

#include <vector>
#include "Vector2D.h"
#include "Matrix3x3.h"


std::vector<Vector2D> WorldTransform(
	const std::vector<Vector2D>& points, 
	const Vector2D move, 
	float theta, 
	const Vector2D scale)
{
	std::vector<Vector2D> transform(points.size());

	for (size_t i = 0; i < points.size(); i++)
	{
		transform[i] = { points[i] * Matrix3x3::TransformMatrix(move.X, move.Y, theta, scale.X, scale.Y) };
	}

	return transform;
}

Vector2D WorldTransform(
	const Vector2D& points,
	const Vector2D& move,
	float theta,
	const Vector2D& scale)
{
	Vector2D temp{ points * Matrix3x3::TransformMatrix(move.X, move.Y, theta, scale.X, scale.Y) };

	return temp;
}