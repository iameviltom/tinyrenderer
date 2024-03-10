#pragma once

#include "Vec3.h"
#include "Vec2.h"

namespace TV
{
	namespace Maths
	{
		Vec3f ComputeBarycentricCoordinate(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c);

		bool PointInPoly2D(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c);
	}
}