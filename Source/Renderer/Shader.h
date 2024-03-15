#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Vec4.h"
#include "../Maths/Vec3.h"
#include "../Maths/Matrix4x4.h"
#include "../Maths/Colour.h"

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		struct VertexShaderConstants
		{
			Matrix4x4f ModelMatrix;
			Matrix4x4f ViewMatrix;
			Matrix4x4f ProjectionMatrix;
		};
		struct VertexShaderInput
		{
			Vec3f Position;
			Vec3f Normal;
			Vec2f TexCoord;
		};
		struct VertexShaderOutput
		{
			Vec4f Position;
			Vec3f Normal;
			Vec2f TexCoord;
		};

		struct PixelShaderConstants
		{
			Vec3f LightDirection;
			const class ICanvas* Diffuse = nullptr;
		};
		struct PixelShaderInput
		{
			Vec3f Normal;
			Vec2f TexCoord;
			Colour Colour;
		};
		struct PixelShaderOutput
		{
			Colour Colour;
		};

		class Shader
		{
		public:
			virtual ~Shader() {}

			virtual VertexShaderOutput VertexShader(const VertexShaderInput& input, const VertexShaderConstants& constants) const;
			virtual PixelShaderOutput PixelShader(const PixelShaderInput& input, const PixelShaderConstants& constants) const;
		};

	}
}

