#pragma once
#include <iostream>

class Vector2D;

class Vector3D
{
public:
	Vector3D();
	Vector3D(const Vector3D& other);			// ���� ������
	Vector3D(const Vector2D& other);					// ���� ������
	Vector3D(Vector3D&& other) noexcept;		// �̵� ������
	Vector3D(float, float, float);
	~Vector3D();

public:
	float X, Y, Z;

public:
	/// ���� ���� �Լ�
	float Size() const;									// ������ ũ�⸦ ��ȯ
	float SizeSquare() const;							// ������ ũ�� ������ ��ȯ
	Vector3D Cross(const Vector3D&) const;				// ���� ���� ��ȯ
	float Dot(const Vector3D&) const;					// ���� ���� ��ȯ

	float FastInvSqrt(float number) const;				// ��� �� ������

	float Projection(const Vector3D&);					// ���� ���� (��Į�� �� ��ȯ)
	float FindTheta(const Vector3D&) const;				// other ���Ϳ��� radian ��ȯ
	Vector3D Normalize() const;							// ����ȭ�� ���͸� ��ȯ

public:
	Vector3D& operator = (const Vector3D&);				// ���� ���� ������
	Vector3D& operator = (const Vector2D&);				// ���� ���� ������
	Vector3D& operator = (Vector3D&&) noexcept;			// �̵� ���� ������

public:
	/// ������ �����ε�
	Vector3D operator + (const Vector3D&) const;		// ���� ����
	Vector3D& operator += (const Vector3D&);
	Vector3D operator - (const Vector3D&) const;		// ���� ����
	// Vector3D operator - () const;					// ���� ���̳ʽ� �ٿ��ֱ�
	Vector3D& operator -= (const Vector3D&);
	bool operator == (const Vector3D&) const;			// ���� �����ΰ�?
	bool operator < (const Vector3D&) const;			// 
	bool operator > (const Vector3D&) const;			// 

	/// ������ �Ű澲�� �ʰ� �ϱ� ���� ������� �����ε�
	Vector3D operator* (float n) const;					// ���� ��Į�� ��
	friend Vector3D operator* (float n, Vector3D v);	// ���� ��Į�� ��

	// Vector3D operator*(const Matrix3x3& other) const;	// ���� * ��� ������

	Vector2D GetVector2D();

public:
	/// ������� ���� << ������ �����ε�
	friend std::ostream& operator<< (std::ostream& os, const Vector3D& v);
};


Vector3D TripleProduct(const Vector3D& a, const Vector3D& b, const Vector3D& c);