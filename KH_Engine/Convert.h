
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include "Vector2D.h"
#include "vector"


namespace Convert
{
	/// Vector2D�� �׸��׸��� ���� POINT2F�� ��ȯ���ֱ� ���� �Լ�
	D2D1_POINT_2F Point2D(const Vector2D& v1);

	/// Vector2D�� vector�� �׸��׸��� ���� POINT2F�� vector�� ��ȯ���ֱ� ���� �Լ�
	std::vector<D2D1_POINT_2F> Point2fArray(const std::vector<Vector2D>& points);
};
