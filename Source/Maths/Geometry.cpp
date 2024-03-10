#include "Geometry.h"

TV::Maths::Vec3f TV::Maths::ComputeBarycentricCoordinate(const Vec2f& point, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	// https://en.wikipedia.org/wiki/Barycentric_coordinate_system
	// https://youtu.be/HYAgJN3x4GA

	Vec3f barycentric;
	barycentric.X =
		((c.Y - b.Y) * (point.X - c.X) + (c.X - b.X) * (point.Y - c.Y)) /
		((b.Y - c.Y) * (a.X - c.X) + (c.X - b.X) * (a.Y - c.Y));
	barycentric.Y =
		((c.Y - a.Y) * (point.X - c.X) + (a.X - c.X) * (point.Y - c.Y)) /
		((b.Y - c.Y) * (a.X - c.X) + (c.X - b.X) * (a.Y - c.Y));
	barycentric.Z = 1.f - barycentric.X - barycentric.Y;
	return barycentric;
}

bool TV::Maths::PointInPoly2D(const Vec2f& point, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	const Vec3f barycentric = ComputeBarycentricCoordinate(point, a, b, c);
	return barycentric.Min() >= 0.f;
}
