#pragma once
#include <iostream>

class Vector2D;

class Vector3D
{
public:
	Vector3D();
	Vector3D(const Vector3D& other);			// 복사 생성자
	Vector3D(const Vector2D& other);					// 복사 생성자
	Vector3D(Vector3D&& other) noexcept;		// 이동 생성자
	Vector3D(float, float, float);
	~Vector3D();

public:
	float X, Y, Z;

public:
	/// 벡터 관련 함수
	float Size() const;									// 벡터의 크기를 반환
	float SizeSquare() const;							// 벡터의 크기 제곱을 반환
	Vector3D Cross(const Vector3D&) const;				// 외적 값을 반환
	float Dot(const Vector3D&) const;					// 내적 값을 반환

	float FastInvSqrt(float number) const;				// 고속 역 제곱근

	float Projection(const Vector3D&);					// 벡터 투영 (스칼라 값 반환)
	float FindTheta(const Vector3D&) const;				// other 벡터와의 radian 반환
	Vector3D Normalize() const;							// 정규화된 벡터를 반환

public:
	Vector3D& operator = (const Vector3D&);				// 복사 대입 연산자
	Vector3D& operator = (const Vector2D&);				// 복사 대입 연산자
	Vector3D& operator = (Vector3D&&) noexcept;			// 이동 대입 연산자

public:
	/// 연산자 오버로딩
	Vector3D operator + (const Vector3D&) const;		// 벡터 덧셈
	Vector3D& operator += (const Vector3D&);
	Vector3D operator - (const Vector3D&) const;		// 벡터 뺄셈
	// Vector3D operator - () const;					// 벡터 마이너스 붙여주기
	Vector3D& operator -= (const Vector3D&);
	bool operator == (const Vector3D&) const;			// 같은 벡터인가?
	bool operator < (const Vector3D&) const;			// 
	bool operator > (const Vector3D&) const;			// 

	/// 순서를 신경쓰지 않게 하기 위해 프렌드로 오버로딩
	Vector3D operator* (float n) const;					// 벡터 스칼라 곱
	friend Vector3D operator* (float n, Vector3D v);	// 벡터 스칼라 곱

	// Vector3D operator*(const Matrix3x3& other) const;	// 벡터 * 행렬 연산자

	Vector2D GetVector2D();

public:
	/// 디버깅을 위한 << 연산자 오버로딩
	friend std::ostream& operator<< (std::ostream& os, const Vector3D& v);
};


Vector3D TripleProduct(const Vector3D& a, const Vector3D& b, const Vector3D& c);