#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix3x3.h"
#include <cmath>


Vector2D::Vector2D() : 
	X(0.f), Y(0.f)
{

}

Vector2D::Vector2D(const Vector2D& other) : 
	X(other.X), Y(other.Y)
{
	
}

Vector2D::Vector2D(Vector2D&& other) noexcept : 
	X(other.X), Y(other.Y)
{

}

Vector2D::Vector2D(float x, float y) : X(x), Y(y)
{

}

Vector2D::Vector2D(const Vector3D& other) : 
	X(other.X), Y(other.Y)
{

}

Vector2D::~Vector2D()
{

}

/// <summary>
/// 고속 역 제곱근
/// from.위키피디아
/// </summary>
/// <param name="number"></param>
/// <returns></returns>
float Vector2D::FastInvSqrt(float number) const
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;
//	i = 0x5f3759df - (i >> 1);
	i = 0x5f375a86 - (i >> 1);				// 0x5fe6eb50c7b537a9 x64, double형에서의 매직넘버
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	//	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
	

	return y;
}

/// <summary>
/// 벡터 크기 반환하는 함수
/// </summary>
/// <returns></returns>
float Vector2D::Size() const
{
	return powf((X * X) + (Y * Y), 0.5);
}

float Vector2D::SizeSquare() const
{
	return (X * X) + (Y * Y);
}

/// <summary>
/// 벡터 외적
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector2D::Cross(const Vector2D& other) const
{
	return X * other.Y - Y * other.X;
}

/// <summary>
/// 벡터 내적
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector2D::Dot(const Vector2D& other) const
{
	return X * other.X + Y * other.Y;
}

/// <summary>
/// 벡터 투영
/// other 벡터를 정규화한 후 투영
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector2D::Projection(const Vector2D& other)
{
	Vector2D v2 = other.Normalize();

	return Dot(v2);
}

/// <summary>
/// 사이각
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector2D::FindTheta(const Vector2D& other) const
{
	Vector2D v1 = Normalize();
	Vector2D v2 = other.Normalize();

	float dot = v1.Dot(v2);
	
	if (dot > 1.0f)
		dot = 1.0f;
	if (dot < -1.0f)
		dot = -1.0f;

	dot = floor(dot * 10000000) / 10000000;

	return acos(dot);
}

/// <summary>
/// 정규화
/// </summary>
/// <returns></returns>
Vector2D Vector2D::Normalize() const
{
	if (Size() == 0)
	{
		return Vector2D(0.f, 0.f);
	}
	else
	{
		float temp = X * X + Y * Y;
		return Vector2D( X * FastInvSqrt(temp), Y * FastInvSqrt(temp));
	}
}

Vector2D& Vector2D::operator = (const Vector2D& other)
{
	if (this == &other)
	{
		return *this;
	}
	this->X = other.X;
	this->Y = other.Y;

	return *this;
}

Vector2D& Vector2D::operator = (Vector2D&& other) noexcept
{
	if (this != &other)
	{
		this->X = other.X;
		this->Y = other.Y;
	}
	return *this;
}


Vector2D& Vector2D::operator=(const Vector3D& other)
{
	this->X = other.X;
	this->Y = other.Y;

	return *this;
}

/// <summary>
/// 벡터 덧셈
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector2D Vector2D::operator + (const Vector2D& other) const
{
	return Vector2D(X + other.X, Y + other.Y);
}

Vector2D& Vector2D::operator+=(const Vector2D& other)
{
	this->X += other.X;
	this->Y += other.Y;
	return *this;
}

/// <summary>
/// 벡터 뺄셈
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector2D Vector2D::operator - (const Vector2D& other) const
{
	return Vector2D(X - other.X, Y - other.Y);
}

/*
Vector2D Vector2D::operator-() const
{
	return Vector2D( -1.f * this->X, -1.f * this->Y);
}
*/

Vector2D& Vector2D::operator-=(const Vector2D& other)
{
	this->X -= other.X;
	this->Y -= other.Y;
	return *this;
}

/// <summary>
/// 두 벡터가 같은지
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
bool Vector2D::operator == (const Vector2D& other) const
{
	return (X == other.X && Y == other.Y);
}

/// <summary>
/// 크기 비교
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
bool Vector2D::operator < (const Vector2D& other) const
{
	return (X < other.X && Y < other.Y);
}

bool Vector2D::operator>(const Vector2D& other) const
{
	return (X > other.X && Y > other.Y);
}

/// <summary>
/// 벡터 실수배
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
Vector2D Vector2D::operator* (float n) const
{
	return Vector2D(this->X * n, this->Y * n);
}

Vector2D operator*(float n, Vector2D v)
{
	Vector2D temp;
	temp.X = v.X * n;
	temp.Y = v.Y * n;

	return temp;
}

/// <summary>
/// 벡터 * 행렬
/// </summary>
/// <param name="v"></param>
/// <param name="other"></param>
/// <returns></returns>
Vector2D Vector2D::operator*(const Matrix3x3& other) const
{
	return Vector2D(
		(this->X * other.e[0][0] + this->Y * other.e[1][0] + 1 * other.e[2][0]), 
		(this->X * other.e[0][1] + this->Y * other.e[1][1] + 1 * other.e[2][1])
	);
}

Vector2D Vector2D::operator*=(float n)
{
	this->X *= n;
	this->Y *= n;
	return *this;
}

Vector2D Vector2D::operator/(float n) const
{
	return Vector2D(this->X / n, this->Y / n);
}

Vector2D Vector2D::operator/=(float n)
{
	this->X /= n;
	this->Y /= n;
	return *this;
}

Vector3D Vector2D::GetVector3D()
{
	return Vector3D(X, Y, 0.f);
}

std::ostream& operator<< (std::ostream& os, const Vector2D& v)
{
	os << v.X << ", " << v.Y;
	return os;
}

 Vector2D Cross(const Vector2D& vec, float a)
{
	return Vector2D(a * vec.Y, -a * vec.X);
}

 Vector2D Cross(float a, const Vector2D& vec)
{
	return Vector2D(-a * vec.Y, a * vec.X);
}
