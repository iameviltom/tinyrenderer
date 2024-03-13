#pragma once

#include "Vec3.h"
#include "Vec2.h"

namespace TV
{
	namespace Maths
	{
		Vec3f ComputeBarycentricCoordinate(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c);

		bool PointInPoly2D(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c);

		template<class T>
		T ComputeValueFromBarycentric(const Vec3f& barycentricCoord, const T& a, const T& b, const T& c)
		{
			const T* const points[3] = { &a, &b, &c };
			T ret(0);
			for (int32 index = 0; index != 3; ++index)
			{
				ret += *points[index] * barycentricCoord.Raw[index];
			}
			return ret;
		}
	}
}