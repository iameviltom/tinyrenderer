#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Vec3.h"
#include "../Maths/Matrix4x4.h"
#include "../Maths/Colour.h"
#include "../Maths/Geometry.h"
#include "ICanvas.h"
#include "DepthBuffer.h"
#include "Drawing.h"
#include "../Model/Model.h"
#include <vector>

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class ICanvas;
		class DepthBuffer;

		struct RenderContext
		{
			DepthBuffer* DepthBuffer = nullptr;
			ICanvas* Canvas = nullptr;

			bool IsValid() const { return Canvas != nullptr; }
		};

		class IShader
		{
		public:
			Matrix4x4f ModelMatrix;
			Matrix4x4f ViewMatrix;
			Matrix4x4f ProjectionMatrix;

		public:
			virtual void DrawModel(const Model& model, const RenderContext& context) = 0;
		};

		template<class TShaderImplementation>
		class TShader : public IShader, public TShaderImplementation
		{
		public:
			typedef typename TShaderImplementation::VertexOutput VertexOutput;

			virtual void DrawModel(const Model& model, const RenderContext& context) final;
			virtual void DrawModelWireframe(const Model& model, const RenderContext& context, const Colour& colour) final;

			void DrawTriangle(const RenderContext& context, const VertexOutput& vertexA, const VertexOutput& vertexB, const VertexOutput& vertexC);
		};
	}
}

template<class TShaderImplementation>
void TV::Renderer::TShader<TShaderImplementation>::DrawModel(const Model& model, const RenderContext& context)
{
	if (!context.IsValid())
	{
		return;
	}

	std::vector<VertexOutput> vertexData;
	vertexData.reserve(model.NumVertices());

	for (int32 vertexIndex = 0; vertexIndex != model.NumVertices(); ++vertexIndex)
	{
		vertexData.push_back(TShaderImplementation::VertexShader(*this, model.GetVertex(vertexIndex)));
	}

	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = model.GetTri(triIndex);

		// todo: here we need to do clipping

		DrawTriangle(context, vertexData[tri.VertexIndex[0]], vertexData[tri.VertexIndex[1]], vertexData[tri.VertexIndex[2]]);
	}
}

template<class TShaderImplementation>
void TV::Renderer::TShader<TShaderImplementation>::DrawModelWireframe(const Model& model, const RenderContext& context, const Colour& colour)
{
	if (!context.IsValid())
	{
		return;
	}

	std::vector<TShaderImplementation::VertexOutput> vertexData;
	vertexData.reserve(model.NumVertices());

	for (int32 vertexIndex = 0; vertexIndex != model.NumVertices(); ++vertexIndex)
	{
		vertexData.push_back(TShaderImplementation::VertexShader(*this, model.GetVertex(vertexIndex)));
	}

	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = model.GetTri(triIndex);

		// todo: here we need to do clipping

		Vec3f normalisedDeviceCoordPositions[3];
		Vec2i screenPositions[3];
		{
			const Vec2f canvasHalfSize = ToFloat(context.Canvas->GetSize()) * 0.5f;
			for (int32 index = 0; index != 3; ++index)
			{
				normalisedDeviceCoordPositions[index] = vertexData[tri.VertexIndex[index]].Position.GetProjected();
				screenPositions[index] = GetRoundToInt(canvasHalfSize + canvasHalfSize * normalisedDeviceCoordPositions[index].GetXY());
			}
		}

		TV::Renderer::DrawLine(screenPositions[0], screenPositions[1], *context.Canvas, colour);
		TV::Renderer::DrawLine(screenPositions[0], screenPositions[2], *context.Canvas, colour);
		TV::Renderer::DrawLine(screenPositions[1], screenPositions[2], *context.Canvas, colour);
	}
}

template<class TShaderImplementation>
void TV::Renderer::TShader<TShaderImplementation>::DrawTriangle(const RenderContext& context, const VertexOutput& vertexA, const VertexOutput& vertexB, const VertexOutput& vertexC)
{
	check(context.IsValid());

	Vec3f normalisedDeviceCoordPositions[3];
	Vec2f screenPositions[3];
	{
		const Vec2f canvasHalfSize = ToFloat(context.Canvas->GetSize()) * 0.5f;
		const VertexOutput vertices[3] = { vertexA, vertexB, vertexC };
		for (int32 index = 0; index != 3; ++index)
		{
			normalisedDeviceCoordPositions[index] = vertices[index].Position.GetProjected();
			screenPositions[index] = canvasHalfSize + canvasHalfSize * normalisedDeviceCoordPositions[index].GetXY();
		}
	}

	// get 2D bounding box of points
	Vec2f min, max;
	min.X = GetMin(screenPositions[0].X, screenPositions[1].X, screenPositions[2].X);
	min.Y = GetMin(screenPositions[0].Y, screenPositions[1].Y, screenPositions[2].Y);
	max.X = GetMax(screenPositions[0].X, screenPositions[1].X, screenPositions[2].X);
	max.Y = GetMax(screenPositions[0].Y, screenPositions[1].Y, screenPositions[2].Y);

	// clamp to bounds of canvas
	min = min.GetClamped(Vec2f(), ToFloat(context.Canvas->GetSize()));
	max = max.GetClamped(Vec2f(), ToFloat(context.Canvas->GetSize()));

	const Vec2i minInt(GetFloorToInt(min.X), GetFloorToInt(min.Y));
	const Vec2i maxInt(GetMin(GetCeilToInt(max.X), context.Canvas->GetSize().X - 1), GetMin(GetCeilToInt(max.Y), context.Canvas->GetSize().Y - 1));

	for (int32 x = minInt.X; x <= maxInt.X; ++x)
	{
		for (int32 y = minInt.Y; y <= maxInt.Y; ++y)
		{
			const Vec2i point2D(x, y);
			const Vec3f barycentric = ComputeBarycentricCoordinate(ToFloat(point2D), screenPositions[0], screenPositions[1], screenPositions[2]);
			if (barycentric.GetMin() <= 0.f)
			{
				// outside of poly
				continue;
			}

			// result should be in range [-1,1] where -1 = near clip, 1 = far clip
			float depthBufferVal = 0.f;
			if (context.DepthBuffer != nullptr)
			{
				const float depth = ComputeValueFromBarycentric(barycentric, normalisedDeviceCoordPositions[0].Z, normalisedDeviceCoordPositions[1].Z, normalisedDeviceCoordPositions[2].Z);
				if (depth > 1.f || depth < -1.f)
				{
					continue;
				}
				// remap value for depth buffer such that 0 = far clip, 1 = near clip
				depthBufferVal = 1.f - ((depth * 0.5f) + 0.5f);
				if (context.DepthBuffer->Get(point2D) > depthBufferVal)
				{
					continue;
				}
			}

			const VertexOutput input = TShaderImplementation::Interpolate(barycentric, vertexA, vertexB, vertexC);
			const Colour output = TShaderImplementation::FragmentShader(*this, input);
			if (output.A > 0)
			{
				context.Canvas->SetPixel(point2D, output);

				if (context.DepthBuffer != nullptr)
				{
					context.DepthBuffer->Set(point2D, depthBufferVal);
				}
			}
		}
	}
}

