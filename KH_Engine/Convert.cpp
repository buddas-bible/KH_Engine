#include "Convert.h"

namespace Convert
{
	D2D1_POINT_2F Point2D(const Vector2D& v1)
	{
		return D2D1::Point2F(v1.X, v1.Y);
	}

	std::vector<D2D1_POINT_2F> Point2fArray(const std::vector<Vector2D>& points)
	{
		std::vector<D2D1_POINT_2F> pPoints;
		pPoints.clear();

		for (size_t i = 0; i < points.size(); i++)
		{
			pPoints.push_back(Convert::Point2D(points[i]));
		}

		return pPoints;
	}
};
