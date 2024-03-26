#pragma once

#include "../Renderer/Rasterizer.h"

namespace TV
{
	namespace Shaders
	{
		using namespace Maths;
		using namespace Renderer;

		class Shader_Example
		{
		public:
			struct VertexOutput
			{
				Vec4f Position;
			};
			VertexOutput VertexShader(const IRasterizer& shader, const Vertex& input) const;
			VertexOutput Interpolate(const Vec3f& barycentricCoord, const VertexOutput& a, const VertexOutput& b, const VertexOutput& c) const;
			Colour FragmentShader(const IRasterizer& shader, const VertexOutput& input) const;
		};

		using Rasterizer_Example = TRasterizer<Shader_Example>;
	}
}