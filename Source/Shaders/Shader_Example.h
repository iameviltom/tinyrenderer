#pragma once

#include "../Renderer/Shader.h"

namespace TV
{
	namespace Shaders
	{
		using namespace Maths;
		using namespace Renderer;

		class ShaderImplementation_Example
		{
		public:
			struct VertexOutput
			{
				Vec4f Position;
			};
			VertexOutput VertexShader(const IShader& shader, const Vertex& input) const;
			VertexOutput Interpolate(const Vec3f& barycentricCoord, const VertexOutput& a, const VertexOutput& b, const VertexOutput& c) const;
			Colour FragmentShader(const IShader& shader, const VertexOutput& input) const;
		};

		using Shader_Example = TShader<ShaderImplementation_Example>;
	}
}