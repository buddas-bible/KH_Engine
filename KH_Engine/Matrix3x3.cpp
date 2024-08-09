#include "Matrix3x3.h"
#include <cmath>


Matrix3x3::Matrix3x3() : 
	e{
	(1), (0), (0),
	(0), (1), (0),
	(0), (0), (1) }
{

}

Matrix3x3::Matrix3x3(
	float e00, float e01, float e02, 
	float e10, float e11, float e12, 
	float e20, float e21, float e22) :
	e{
	(e00), (e01), (e02),
	(e10), (e11), (e12),
	(e20), (e21), (e22) }
{

}

Matrix3x3::~Matrix3x3()
{

}

D2D1_MATRIX_3X2_F Matrix3x3::TransformD2D1Matrix(float moveX, float moveY, float theta, float scaleX, float scaleY)
{
	D2D1_MATRIX_3X2_F d2d1Matrix;
	d2d1Matrix.m[0][0] = (scaleX * cos(theta));
	d2d1Matrix.m[0][1] = (scaleX * sin(theta));
	d2d1Matrix.m[1][0] = (-scaleY * sin(theta));
	d2d1Matrix.m[1][1] = (scaleY * cos(theta));
	d2d1Matrix.m[2][0] = (moveX);
	d2d1Matrix.m[2][1] = (moveY);

	return d2d1Matrix;
}

Matrix3x3 Matrix3x3::TransformMatrix(float moveX, float moveY, float theta, float scaleX, float scaleY)
{
	return Matrix3x3(
		(scaleX * cos(theta)), (scaleX * sin(theta)), (0),
		(-scaleY * sin(theta)), (scaleY * cos(theta)), (0),
		(moveX), (moveY), (1)
	);
}

Matrix3x3 Matrix3x3::TranslateMatrix(float moveX, float moveY)
{
	return Matrix3x3(
		(1), (0), (0),
		(0), (1), (0),
		(moveX), (moveY), (1)
	);
}

Matrix3x3 Matrix3x3::RotateMatrix(float theta)
{
	return Matrix3x3(
		(cos(theta)), (sin(theta)), (0),
		(-sin(theta)), (cos(theta)), (0),
		(0), (0), (1)
	);
}

Matrix3x3 Matrix3x3::ScaleMatrix(float scaleX, float scaleY)
{
	return Matrix3x3(
		(scaleX), (0), (0),
		(0), (scaleY), (0),
		(0), (0), (1)
	);
}

Matrix3x3 Matrix3x3::ScaleRotate(float scaleX, float scaleY, float theta)
{
	return Matrix3x3(
		(scaleX * cos(theta)), (scaleX * sin(theta)), (0),
		(-scaleY * sin(theta)), (scaleY * cos(theta)), (0),
		(0), (0), (1)
	);
}

Matrix3x3 Matrix3x3::operator * (const Matrix3x3& other) const
{
	return Matrix3x3(
		(this->e[0][0] * other.e[0][0] + this->e[0][1] * other.e[1][0] + this->e[0][2] * other.e[2][0]),
		(this->e[0][0] * other.e[0][1] + this->e[0][1] * other.e[1][1] + this->e[0][2] * other.e[2][1]),
		(this->e[0][0] * other.e[0][2] + this->e[0][1] * other.e[1][2] + this->e[0][2] * other.e[2][2]),

		(this->e[1][0] * other.e[0][0] + this->e[1][1] * other.e[1][0] + this->e[1][2] * other.e[2][0]),
		(this->e[1][0] * other.e[0][1] + this->e[1][1] * other.e[1][1] + this->e[1][2] * other.e[2][1]),
		(this->e[1][0] * other.e[0][2] + this->e[1][1] * other.e[1][2] + this->e[1][2] * other.e[2][2]),

		(this->e[2][0] * other.e[0][0] + this->e[2][1] * other.e[1][0] + this->e[2][2] * other.e[2][0]),
		(this->e[2][0] * other.e[0][1] + this->e[2][1] * other.e[1][1] + this->e[2][2] * other.e[2][1]),
		(this->e[2][0] * other.e[0][2] + this->e[2][1] * other.e[1][2] + this->e[2][2] * other.e[2][2]));
}

Matrix3x3 Matrix3x3::operator + (const Matrix3x3& other) const
{
	return Matrix3x3(
		(this->e[0][0] + other.e[0][0]), (this->e[0][1] + other.e[0][1]), (this->e[0][2] + other.e[0][2]),
		(this->e[1][0] + other.e[1][0]), (this->e[1][1] + other.e[1][1]), (this->e[1][2] + other.e[1][2]),
		(this->e[2][0] + other.e[2][0]), (this->e[2][1] + other.e[2][1]), (this->e[2][2] + other.e[2][2]));
}

Matrix3x3 Matrix3x3::operator - (const Matrix3x3& other) const
{
	return Matrix3x3(
		(this->e[0][0] - other.e[0][0]), (this->e[0][1] - other.e[0][1]), (this->e[0][2] - other.e[0][2]),
		(this->e[1][0] - other.e[1][0]), (this->e[1][1] - other.e[1][1]), (this->e[1][2] - other.e[1][2]),
		(this->e[2][0] - other.e[2][0]), (this->e[2][1] - other.e[2][1]), (this->e[2][2] - other.e[2][2]));
}

