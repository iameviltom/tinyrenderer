#pragma once

#include "../Maths/Vec3.h"

#include <vector>

namespace TV
{
	using namespace Maths;

	namespace Renderer
	{
		class Model
		{
		public:
			Model() {}

			bool IsValid() const { return !Vertices.empty() && !Tris.empty(); }

			bool LoadWavefrontFile(const char* fileName);

			size_t NumTris() const { return Tris.size(); }
			size_t NumVerts() const { return Vertices.size(); }

			struct Tri
			{
				int VertIndex[3];
			};
			const Tri& GetTri(int index) const { return Tris[index]; }

			const Vec3f& GetVertex(int index) const { return Vertices[index]; }

			const Vec3f& GetBoundsMin() const { return Min; }
			const Vec3f& GetBoundsMax() const { return Max; }

			Vec3f GetBoundsOrigin() const { return (Max + Min) * 0.5f; }
			Vec3f GetBoundsExtents() const { return (Max - Min) * 0.5f; }

		private:
			std::vector<Vec3f> Vertices;
			std::vector<Tri> Tris;

			Vec3f Min;
			Vec3f Max;
		};
	}
}
