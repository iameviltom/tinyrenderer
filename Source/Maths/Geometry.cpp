#include "Geometry.h"

TV::Maths::Vec3f TV::Maths::ComputeBarycentricCoordinate(const Vec2f& point, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	// https://en.wikipedia.org/wiki/Barycentric_coordinate_system
	// https://youtu.be/HYAgJN3x4GA

	const float s1 = c.Y - a.Y;
	const float s2 = c.X - a.X;
	const float s3 = b.Y - a.Y;
	const float s4 = point.Y - a.Y;

	Vec3f barycentric;
	barycentric.X = (a.X * s1 + s4 * s2 - point.X * s1) / (s3 * s2 - (b.X - a.X) * s1);
	barycentric.Y = (s4 - barycentric.X * s3) / s1;
	barycentric.Z = 1.f - barycentric.X - barycentric.Y;
	return barycentric;
}

bool TV::Maths::PointInPoly2D(const Vec2f& point, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	const Vec3f barycentric = ComputeBarycentricCoordinate(point, a, b, c);
	return barycentric.Min() >= 0.f;
}
