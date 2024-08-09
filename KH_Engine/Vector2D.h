#pragma once
#include <iostream>


class Matrix3x3;
class Vector3D;

class Vector2D
{
public:
	Vector2D();
	Vector2D(const Vector2D& other);			// ���� ������
	Vector2D(const Vector3D& other);			// ���� ������
	Vector2D(Vector2D&& other) noexcept;		// �̵� ������
	Vector2D(float, float);
	~Vector2D();

public:
	float X, Y;

public:
	/// ���� ���� �Լ�
	float Size() const;									// ������ ũ�⸦ ��ȯ
	float SizeSquare() const;							// ������ ũ�� ������ ��ȯ
	float Cross(const Vector2D&) const;					// ���� ���� ��ȯ
	float Cross(const float&) const;					// ���� ���� ��ȯ
	float Dot(const Vector2D&) const;					// ���� ���� ��ȯ
	// float TripleProduct(const Vector2D&) const;					// 

	float FastInvSqrt(float number) const;				// ��� �� ������

	float Projection(const Vector2D&);					// ���� ���� (��Į�� �� ��ȯ)
	float FindTheta(const Vector2D&) const;				// other ���Ϳ��� radian ��ȯ
	Vector2D Normalize() const;							// ����ȭ�� ���͸� ��ȯ

public:
	Vector2D& operator = (const Vector2D&);				// ���� ���� ������
	Vector2D& operator = (const Vector3D&);				// ���� ���� ������
	Vector2D& operator = (Vector2D&&) noexcept;			// �̵� ���� ������

public:
	/// ������ �����ε�
	Vector2D operator + (const Vector2D&) const;		// ���� ����
	Vector2D& operator += (const Vector2D&);			
	Vector2D operator - (const Vector2D&) const;		// ���� ����
	// Vector2D operator - () const;					// ���� ���̳ʽ� �ٿ��ֱ�
	Vector2D& operator -= (const Vector2D&);			
	bool operator == (const Vector2D&) const;			// ���� �����ΰ�?
	bool operator < (const Vector2D&) const;			// 
	bool operator > (const Vector2D&) const;			// 

	/// ������ �Ű澲�� �ʰ� �ϱ� ���� ������� �����ε�
	Vector2D operator* (float n) const;						// ���� ��Į�� ��
	Vector2D operator*= (float n);						// ���� ��Į�� ��
	Vector2D operator/ (float n) const;						// ���� ��Į�� ��
	Vector2D operator/= (float n);						// ���� ��Į�� ��
	friend Vector2D operator* (float n, Vector2D v);	// ���� ��Į�� ��

	Vector2D operator*(const Matrix3x3& other) const;	// ���� * ��� ������
	
	Vector3D GetVector3D();


public:
	/// ������� ���� << ������ �����ε�
	friend std::ostream& operator<< (std::ostream& os, const Vector2D& v);
};

Vector2D Cross(const Vector2D& vec, float a);

Vector2D Cross(float a, const Vector2D& vec);