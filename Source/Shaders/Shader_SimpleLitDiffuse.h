#pragma once

#include "../Renderer/Rasterizer.h"

namespace TV
{
	namespace Shaders
	{
		using namespace Maths;
		using namespace Renderer;

		class Shader_SimpleLitDiffuse
		{
		public:
			Vec3f LightDirection; // camera space
			const class ICanvas* Diffuse = nullptr;
			Colour BaseColour;

			struct VertexOutput
			{
				Vec4f Position; // clip space
				Vec3f Normal; // camera space
				Vec2f TexCoord;
			};
			VertexOutput VertexShader(const IRasterizer& shader, const Vertex& input) const;
			VertexOutput Interpolate(const Vec3f& barycentricCoord, const VertexOutput& a, const VertexOutput& b, const VertexOutput& c) const;
			Colour FragmentShader(const IRasterizer& shader, const VertexOutput& input) const;
		};

		using Rasterizer_SimpleLitDiffuse = TRasterizer<Shader_SimpleLitDiffuse>;
	}
}