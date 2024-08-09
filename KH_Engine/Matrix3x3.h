#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

class Matrix3x3
{
public:
	Matrix3x3();
	Matrix3x3(
		float e00, float e01, float e02,
		float e10, float e11, float e12, 
		float e20, float e21, float e22);

	~Matrix3x3();

	float e[3][3];

	static D2D1_MATRIX_3X2_F TransformD2D1Matrix(
		float moveX, float moveY, 
		float theta, float scaleX, 
		float scaleY);

	static Matrix3x3 TransformMatrix(								// ��ȯ���
		float moveX, float moveY, 
		float theta, float scaleX, 
		float scaleY);
	static Matrix3x3 TranslateMatrix(float moveX, float moveY);		// �̵� ��ȯ ���
	static Matrix3x3 RotateMatrix(float theta);						// ȸ�� ��ȯ ���
	static Matrix3x3 ScaleMatrix(float scaleX, float scaleY);		// ��ô ��ȯ ���
	static Matrix3x3 ScaleRotate(float scaleX, float scaleY, float theta);


	Matrix3x3 operator * (const Matrix3x3& other) const;
	Matrix3x3 operator + (const Matrix3x3& other) const;
	Matrix3x3 operator - (const Matrix3x3& other) const;
};