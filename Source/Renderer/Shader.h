#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Vec4.h"
#include "../Maths/Vec3.h"
#include "../Maths/Matrix4x4.h"
#include "../Maths/Colour.h"
#include "Vertex.h"
#include <vector>

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class IShader
		{
		public:
			Matrix4x4f ModelMatrix;
			Matrix4x4f ViewMatrix;
			Matrix4x4f ProjectionMatrix;

		public:
			virtual void Init(int32 numVertices) = 0;
			virtual void ExecuteVertexShader(const Vertex& input) = 0;
			virtual Vec4f GetClipSpaceVertexPosition(int32 vertexIndex) const = 0;
			virtual Colour ExecuteFragmentShader(const Vec3f& barycentricCoord, int32 vertexIndexA, int32 vertexIndexB, int32 vertexIndexC) = 0;
		};

		template<class VertexOutput>
		class TShader : public IShader
		{
		public:
			virtual void Init(int32 numVertices) override
			{
				VertexData.clear();
				VertexData.reserve(numVertices);
			}
			virtual void ExecuteVertexShader(const Vertex& input) final
			{
				const VertexOutput output = VertexShader(input);
				VertexData.push_back(output);
			}
			virtual Vec4f GetClipSpaceVertexPosition(int32 vertexIndex) const override { return VertexData[vertexIndex].Position; }
			virtual Colour ExecuteFragmentShader(const Vec3f& barycentricCoord, int32 vertexIndexA, int32 vertexIndexB, int32 vertexIndexC) final
			{
				const VertexOutput input = VertexOutput::Interpolate(barycentricCoord, VertexData[vertexIndexA], VertexData[vertexIndexB], VertexData[vertexIndexC]);
				return FragmentShader(input);
			}

			virtual VertexOutput VertexShader(const Vertex& input) = 0;
			virtual Colour FragmentShader(const VertexOutput& input) = 0;

		private:
			std::vector<VertexOutput> VertexData;
		};

		struct VertexOutput_SimpleLitDiffuse
		{
			Vec4f Position; // clip space
			Vec3f Normal; // camera space
			Vec2f TexCoord;

			static VertexOutput_SimpleLitDiffuse Interpolate(const Vec3f& barycentricCoord, const VertexOutput_SimpleLitDiffuse& a, const VertexOutput_SimpleLitDiffuse& b, const VertexOutput_SimpleLitDiffuse& c);
		};
		class Shader_SimpleLitDiffuse : public TShader<VertexOutput_SimpleLitDiffuse>
		{
		public:
			Vec3f LightDirection; // camera space
			const class ICanvas* Diffuse = nullptr;
			Colour BaseColour;

			virtual VertexOutput_SimpleLitDiffuse VertexShader(const Vertex& input) override;
			virtual Colour FragmentShader(const VertexOutput_SimpleLitDiffuse& input) override;
		};
	}
}

