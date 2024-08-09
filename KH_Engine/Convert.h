
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include "Vector2D.h"
#include "vector"


namespace Convert
{
	/// Vector2D를 그림그리기 위한 POINT2F로 변환해주기 위한 함수
	D2D1_POINT_2F Point2D(const Vector2D& v1);

	/// Vector2D의 vector를 그림그리기 위한 POINT2F의 vector로 변환해주기 위한 함수
	std::vector<D2D1_POINT_2F> Point2fArray(const std::vector<Vector2D>& points);
};
