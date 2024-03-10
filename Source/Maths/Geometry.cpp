#include "Geometry.h"

TV::Maths::Vec3f TV::Maths::ComputeBarycentricCoordinate(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	// https://en.wikipedia.org/wiki/Barycentric_coordinate_system

#if 0
	// https://youtu.be/HYAgJN3x4GA
	const float s1 = c.Y - a.Y;
	const float s2 = c.X - a.X;
	const float s3 = b.Y - a.Y;
	const float s4 = p.Y - a.Y;

	Vec3f barycentric;
	barycentric.Y = (a.X * s1 + s4 * s2 - p.X * s1) / (s3 * s2 - (b.X - a.X) * s1);
	barycentric.Z = (s4 - barycentric.X * s3) / s1;
	barycentric.X = 1.f - barycentric.Y - barycentric.Z;
	return barycentric;
#else
	// https://ceng2.ktu.edu.tr/~cakir/files/grafikler/Texture_Mapping.pdf
	const Vec2f v0 = b - a;
	const Vec2f v1 = c - a;
	const Vec2f v2 = p - a;

	const double d00 = DotProduct(v0, v0);
	const double d01 = DotProduct(v0, v1);
	const double d11 = DotProduct(v1, v1);
	const double d20 = DotProduct(v2, v0);
	const double d21 = DotProduct(v2, v1);

	const double denom = d00 * d11 - d01 * d01;

	Vec3f barycentric;
	barycentric.Y = (float)((d11 * d20 - d01 * d21) / denom);
	barycentric.Z = (float)((d00 * d21 - d01 * d20) / denom);
	barycentric.X = 1.f - barycentric.Y - barycentric.Z;
	return barycentric;
#endif
}

bool TV::Maths::PointInPoly2D(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	const Vec3f barycentric = ComputeBarycentricCoordinate(p, a, b, c);
	return barycentric.Min() >= 0.f;
}
