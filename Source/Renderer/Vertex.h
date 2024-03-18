#pragma once

#include "../Maths/Vec3.h"
#include "../Maths/Vec2.h"

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		struct Vertex
		{
			Vec3f Position;
			Vec2f TexCoord;
			Vec3f Normal;
		};

		inline Vec3f CalculateNormal(const Vertex& a, const Vertex& b, const Vertex& c)
		{
			const Vec3f ba = b.Position - a.Position;
			const Vec3f ca = c.Position - a.Position;

			const Vec3f crossProduct = GetCrossProduct(ca, ba);
			return crossProduct.GetSafeNormal();
		}
	}
}