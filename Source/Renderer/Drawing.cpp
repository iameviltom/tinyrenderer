#include "Drawing.h"

#include "ICanvas.h"
#include "../Model/Model.h"
#include "../Maths/Types.h"
#include "../Maths/Geometry.h"
#include "DepthBuffer.h"
#include "../Image/TgaImage.h"
#include "../Maths/Matrix4x4.h"
#include "../Maths/Vec4.h"
#include "../Maths/Maths.h"
#include "Shader.h"

void TV::Renderer::DrawLine(Vec2i start, Vec2i end, ICanvas& canvas, const Colour& colour)
{
	if (start == end)
	{
		return;
	}

	Vec2i delta(end - start);

	Vec2i absDelta = delta.GetAbs();
	const bool bSteep = absDelta.Y > absDelta.X;
	if (bSteep)
	{
		start = start.GetSwizzledYX();
		end = end.GetSwizzledYX();
		delta = delta.GetSwizzledYX();
	}

	// make sure we draw left to right
	if (delta.X < 0)
	{
		std::swap(start, end);
		delta *= -1;
	}

	int32 deltaError = std::abs(delta.Y) * 2;
	int32 error = 0;

	int32 y = start.Y;
	int32 ySign = delta.Y > 0 ? 1 : -1;

	if (bSteep)
	{
		for (int32 x = start.X; x != end.X; ++x)
		{
			canvas.SetPixel(Vec2i(y, x), colour);

			error += deltaError;
			if (error > delta.X)
			{
				y += ySign;
				error -= delta.X * 2;
			}
		}
	}
	else
	{
		for (int32 x = start.X; x != end.X; ++x)
		{
			canvas.SetPixel(Vec2i(x, y), colour);

			error += deltaError;
			if (error > delta.X)
			{
				y += ySign;
				error -= delta.X * 2;
			}
		}
	}
}

void TV::Renderer::DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, ICanvas& canvas, const Colour& colour)
{
	DrawLine(Vec2i(x0, y0), Vec2i(x1, y1), canvas, colour);
}

void TV::Renderer::DrawModelWireframe(const Model& model, const RenderContext& context, IShader& shader, const Colour& colour)
{
	if (!context.IsValid())
	{
		return;
	}

	shader.Init(model.NumVertices());

	for (int32 vertexIndex = 0; vertexIndex != model.NumVertices(); ++vertexIndex)
	{
		shader.ExecuteVertexShader(model.GetVertex(vertexIndex));
	}

	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = model.GetTri(triIndex);

		Vec3f normalisedDeviceCoordPositions[3];
		Vec2i screenPositions[3];
		{
			const Vec2f canvasHalfSize = ToFloat(context.Canvas->GetSize()) * 0.5f;
			for (int32 index = 0; index != 3; ++index)
			{
				// todo: need to solve the divide by zero case here via proper clipping
				normalisedDeviceCoordPositions[index] = shader.GetClipSpaceVertexPosition(tri.VertexIndex[index]).GetProjected();
				screenPositions[index] = GetRoundToInt(canvasHalfSize + canvasHalfSize * normalisedDeviceCoordPositions[index].GetXY());
			}
		}

		DrawLine(screenPositions[0], screenPositions[1], *context.Canvas, colour);
		DrawLine(screenPositions[0], screenPositions[2], *context.Canvas, colour);
		DrawLine(screenPositions[1], screenPositions[2], *context.Canvas, colour);
	}
}

namespace TV
{
	namespace Renderer
	{
		void DrawTriangle_LineFill(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour)
		{
			// sort vertices in ascending order in y
			if (a.Y > b.Y) { std::swap(a, b); }
			if (a.Y > c.Y) { std::swap(a, c); }
			if (b.Y > c.Y) { std::swap(b, c); }

			const int32 totalHeight = c.Y - a.Y;
			const int32 topSegmentHeight = c.Y - b.Y;
			const int32 bottomSegmentHeight = b.Y - a.Y;

			// draw top and bottom halves
			for (int32 yOffset = 0; yOffset != totalHeight; ++yOffset)
			{
				const bool bTopSegment = yOffset > bottomSegmentHeight || bottomSegmentHeight == 0;
				const int32 thisOffset = bTopSegment ? yOffset - bottomSegmentHeight : yOffset;

				const int32 segmentHeight = bTopSegment ? topSegmentHeight : bottomSegmentHeight;

				const int32 y = bTopSegment ? c.Y - thisOffset : a.Y + thisOffset;

				const float alpha = thisOffset / (float)totalHeight;
				const float beta = thisOffset / (float)segmentHeight;

				Vec2i start = GetLerp(a, c, bTopSegment ? 1.f - alpha : alpha);
				Vec2i end = GetLerp(bTopSegment ? c : a, b, beta);

				// sort left to right
				if (start.X > end.X)
				{
					std::swap(start, end);
				}
				for (int32 x = start.X; x <= end.X; ++x)
				{
					canvas.SetPixel(Vec2i(x, y), colour);
				}
			}

			// and also fill in the middle line
			if (totalHeight > 0)
			{
				const int32 y = b.Y;
				const float alpha = (y - a.Y) / (float)totalHeight;
				const int32 otherX = GetLerp(a, c, alpha).X;
				const int32 minX = GetMin(b.X, otherX);
				const int32 maxX = GetMax(b.X, otherX);
				for (int32 x = minX; x != maxX; ++x)
				{
					canvas.SetPixel(Vec2i(x, y), colour);
				}
			}
		}

		void DrawTriangle_Barycentric(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour)
		{
			// get bounding box of points
			Vec2i min, max;
			min.X = GetMin(a.X, b.X, c.X);
			min.Y = GetMin(a.Y, b.Y, c.Y);
			max.X = GetMax(a.X, b.X, c.X);
			max.Y = GetMax(a.Y, b.Y, c.Y);

			Vec2f af = ToFloat(a);
			Vec2f bf = ToFloat(b);
			Vec2f cf = ToFloat(c);

			for (int32 x = min.X; x <= max.X; ++x)
			{
				for (int32 y = min.Y; y != max.Y; ++y)
				{
					if (PointInPoly2D(ToFloat(Vec2i(x, y)), af, bf, cf))
					{
						canvas.SetPixel(Vec2i(x, y), colour);
					}
				}
			}
		}
	}
}

void TV::Renderer::DrawTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour)
{
	DrawTriangle_LineFill(a, b, c, canvas, colour);
	//DrawTriangle_Barycentric(a, b, c, canvas, colour);
}

void TV::Renderer::DrawTriangle(const RenderContext& context, IShader& shader, int32 vertexIndexA, int32 vertexIndexB, int32 vertexIndexC)
{
	check(context.IsValid());

	Vec3f normalisedDeviceCoordPositions[3];
	Vec2f screenPositions[3];
	{
		const Vec2f canvasHalfSize = ToFloat(context.Canvas->GetSize()) * 0.5f;
		const int32 indices[3] = { vertexIndexA, vertexIndexB, vertexIndexC };
		for (int32 index = 0; index != 3; ++index)
		{
			// todo: need to solve the divide by zero case here via proper clipping
			normalisedDeviceCoordPositions[index] = shader.GetClipSpaceVertexPosition(indices[index]).GetProjected();
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
			const float depth = ComputeValueFromBarycentric(barycentric, normalisedDeviceCoordPositions[0].Z, normalisedDeviceCoordPositions[1].Z, normalisedDeviceCoordPositions[2].Z);
			if (depth > 1.f || depth < -1.f)
			{
				continue;
			}
			// remap value for depth buffer such that 0 = far clip, 1 = near clip
			float depthBufferVal = 1.f - ((depth * 0.5f) + 0.5f);
			if (context.DepthBuffer->Get(point2D) > depthBufferVal)
			{
				continue;
			}

			const Colour output = shader.ExecuteFragmentShader(barycentric, vertexIndexA, vertexIndexB, vertexIndexC);
			if (output.A > 0)
			{
				context.Canvas->SetPixel(point2D, output);
				context.DepthBuffer->Set(point2D, depthBufferVal);
			}
		}
	}
}

void TV::Renderer::DrawModel(const Model& model, const RenderContext& context, IShader& shader)
{
	if (!context.IsValid())
	{
		return;
	}

	shader.Init(model.NumVertices());

	for (int32 vertexIndex = 0; vertexIndex != model.NumVertices(); ++vertexIndex)
	{
		shader.ExecuteVertexShader(model.GetVertex(vertexIndex));
	}

	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = model.GetTri(triIndex);
		DrawTriangle(context, shader, tri.VertexIndex[0], tri.VertexIndex[1], tri.VertexIndex[2]);
	}
}
