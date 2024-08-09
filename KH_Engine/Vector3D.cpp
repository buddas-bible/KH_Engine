#include "Vector3D.h"
#include "Matrix3x3.h"
#include <cmath>

#include "Vector2D.h"

Vector3D::Vector3D() :
	X(0.f), Y(0.f), Z(0.f)
{

}

Vector3D::Vector3D(const Vector3D& other) :
	X(other.X), Y(other.Y), Z(other.Z)
{

}

Vector3D::Vector3D(Vector3D&& other) noexcept :
	X(other.X), Y(other.Y), Z(other.Z)
{

}

Vector3D::Vector3D(float x, float y, float z) : 
	X(x), Y(y), Z(z)
{

}

Vector3D::Vector3D(const Vector2D& other) :
	X(other.X), Y(other.Y), Z(0.f)
{

}

Vector3D::~Vector3D()
{

}


/// <summary>
/// 고속 역 제곱근
/// from.위키피디아
/// </summary>
/// <param name="number"></param>
/// <returns></returns>
float Vector3D::FastInvSqrt(float number) const
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;
	//	i = 0x5f3759df - (i >> 1);
	i = 0x5f375a86 - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	//	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return y;
}


/// <summary>
/// 벡터 크기 반환하는 함수
/// </summary>
/// <returns></returns>
float Vector3D::Size() const
{
	return powf((X * X) + (Y * Y) + (Z * Z), 0.5);
}

float Vector3D::SizeSquare() const
{
	return (X * X) + (Y * Y) + (Z * Z);
}

/// <summary>
/// 벡터 외적
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::Cross(const Vector3D& other) const
{
	return Vector3D(
		(this->Y * other.Z - this->Z * other.Y),
		(this->Z * other.X - this->X * other.Z),
		(this->X * other.Y - this->Y * other.X));
}

/// <summary>
/// 벡터 내적
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector3D::Dot(const Vector3D& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z;
}

/// <summary>
/// 벡터 투영
/// other 벡터에 투영
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector3D::Projection(const Vector3D& other)
{
	Vector3D v2 = other.Normalize();

	return Dot(v2);
}

/// <summary>
/// 사이각
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector3D::FindTheta(const Vector3D& other) const
{
	Vector3D v1 = Normalize();
	Vector3D v2 = other.Normalize();

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
Vector3D Vector3D::Normalize() const
{
	if (Size() == 0)
	{
		return Vector3D(0.f, 0.f, 0.f);
	}
	else
	{
		float temp = X * X + Y * Y + Z * Z;
		return Vector3D( X * FastInvSqrt(temp), Y * FastInvSqrt(temp), Z * FastInvSqrt(temp));
		// return Vector3D( X / Size(), Y / Size(), Z / Size());
	}
}

Vector3D& Vector3D::operator = (const Vector3D& other)
{
	if (this == &other)
	{
		return *this;
	}
	this->X = other.X;
	this->Y = other.Y;
	this->Z = other.Z;

	return *this;
}

Vector3D& Vector3D::operator = (Vector3D&& other) noexcept
{
	if (this != &other)
	{
		this->X = other.X;
		this->Y = other.Y;
		this->Z = other.Z;
	}
	return *this;
}


Vector3D& Vector3D::operator=(const Vector2D& other)
{
	this->X = other.X;
	this->Y = other.Y;
	this->Z = 0.f;

	return *this;
}

/// <summary>
/// 벡터 덧셈
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::operator + (const Vector3D& other) const
{
	return Vector3D(X + other.X, Y + other.Y, Z + other.Z);
}

Vector3D& Vector3D::operator+=(const Vector3D& other)
{
	this->X += other.X;
	this->Y += other.Y;
	this->Z += other.Z;
	return *this;
}

/// <summary>
/// 벡터 뺄셈
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::operator - (const Vector3D& other) const
{
	return Vector3D(X - other.X, Y - other.Y, Z - other.Z);
}

Vector3D& Vector3D::operator-=(const Vector3D& other)
{
	this->X -= other.X;
	this->Y -= other.Y;
	this->Z -= other.Z;
	return *this;
}

/// <summary>
/// 두 벡터가 같은지
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
bool Vector3D::operator == (const Vector3D& other) const
{
	return (X == other.X && Y == other.Y && Z == other.Z);
}

/// <summary>
/// 크기 비교
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
bool Vector3D::operator < (const Vector3D& other) const
{
	return (X < other.X && Y < other.Y && Z < other.Z);
}

bool Vector3D::operator>(const Vector3D& other) const
{
	return (X > other.X && Y > other.Y && Z > other.Z);
}

/// <summary>
/// 벡터 실수배
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
Vector3D Vector3D::operator* (float n) const
{
	return Vector3D(this->X * n, this->Y * n, this->Z  * n);
}

Vector3D operator*(float n, Vector3D v)
{
	Vector3D temp;
	temp.X = v.X * n;
	temp.Y = v.Y * n;
	temp.Z = v.Z * n;

	return temp;
}

Vector2D Vector3D::GetVector2D()
{
	return Vector2D(X, Y);
}

/*
/// <summary>
/// 벡터 * 행렬
/// </summary>
/// <param name="v"></param>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::operator*(const Matrix3x3& other) const
{
	return Vector3D(
		(this->X * other.e[0][0] + this->Y * other.e[1][0] + 1 * other.e[2][0]),
		(this->X * other.e[0][1] + this->Y * other.e[1][1] + 1 * other.e[2][1])
	);
}
*/


std::ostream& operator<< (std::ostream& os, const Vector3D& v)
{
	os << v.X << ", " << v.Y << ", " << v.Z;
	return os;
}

Vector3D TripleProduct(const Vector3D& a, const Vector3D& b, const Vector3D& c)
{
	return a.Cross(b).Cross(c);
}

