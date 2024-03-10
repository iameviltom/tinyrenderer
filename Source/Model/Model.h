#pragma once

#include "../Maths/Vec3.h"

#include <vector>
#include "../Maths/Types.h"

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

		Vec3f CalculateNormal(const Vertex& a, const Vertex& b, const Vertex& c);

		class Model
		{
		public:
			Model() {}

			bool IsValid() const { return !Tris.empty(); }

			bool LoadWavefrontFile(const char* fileName);

			size_t NumTris() const { return Tris.size(); }

			struct Tri
			{
				Vertex Vertices[3];
			};
			Tri GetTri(int32 index) const;

			Vec3f CalculateNormal(int32 index) const;

			const Vec3f& GetBoundsMin() const { return _Min; }
			const Vec3f& GetBoundsMax() const { return _Max; }

			Vec3f GetBoundsOrigin() const { return (_Max + _Min) * 0.5f; }
			Vec3f GetBoundsExtents() const { return (_Max - _Min) * 0.5f; }

		private:
			std::vector<Vec3f> Positions;
			std::vector<Vec2f> TexCoords;
			std::vector<Vec3f> Normals;

			struct VertexRef
			{
				int32 PosIndex = 0;
				int32 TexCoordIndex = 0;
				int32 NormalIndex = 0;
			};
			struct TriRef
			{
				VertexRef Vertices[3];
			};
			std::vector<TriRef> Tris;

			Vec3f _Min;
			Vec3f _Max;
		};
	}
}
