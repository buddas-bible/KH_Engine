#pragma once

#include <vector>

class Vector2D;
class Matrix3x3;

std::vector<Vector2D> WorldTransform(
		const std::vector<Vector2D>& points, 
		const Vector2D move, 
		float theta, 
		const Vector2D scale );

Vector2D WorldTransform(
	const Vector2D& points,
	const Vector2D& move,
	float theta,
	const Vector2D& scale);