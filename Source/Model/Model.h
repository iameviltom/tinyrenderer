#pragma once

#include "../Maths/Vec3.h"

#include <vector>
#include "../Maths/Types.h"
#include "../Renderer/Vertex.h"

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class Model
		{
		public:
			Model() {}

			bool IsValid() const { return !Triangles.empty() && !Vertices.empty(); }

			bool LoadWavefrontFile(const char* fileName);

			int32 NumTris() const { return (int32)Triangles.size(); }
			int32 NumVertices() const { return (int32)Vertices.size(); }

			struct Tri
			{
				int32 VertexIndex[3];
			};
			const Tri& GetTri(int32 index) const { return Triangles[index]; }

			const Vertex& GetVertex(int32 index) const { return Vertices[index]; }

			Vec3f CalculateNormal(int32 triIndex) const;

			const Vec3f& GetBoundsMin() const { return _Min; }
			const Vec3f& GetBoundsMax() const { return _Max; }

			Vec3f GetBoundsOrigin() const { return (_Max + _Min) * 0.5f; }
			Vec3f GetBoundsExtents() const { return (_Max - _Min) * 0.5f; }

		private:
			std::vector<Vertex> Vertices;
			std::vector<Tri> Triangles;

			Vec3f _Min;
			Vec3f _Max;
		};
	}
}
