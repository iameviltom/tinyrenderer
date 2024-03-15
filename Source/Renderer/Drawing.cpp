#include "Drawing.h"

#include "ICanvas.h"
#include "../Model/Model.h"
#include "../Maths/Types.h"
#include "../Maths/Geometry.h"
#include "DepthBuffer.h"
#include "../Image/TgaImage.h"
#include "../Maths/Matrix4x4.h"
#include "../Maths/Vec4.h"

void TV::Renderer::DrawLine(Vec2i start, Vec2i end, ICanvas& canvas, const Colour& colour)
{
	if (start == end)
	{
		return;
	}

	Vec2i delta(end - start);

	Vec2i absDelta = delta.Abs();
	const bool bSteep = absDelta.Y > absDelta.X;
	if (bSteep)
	{
		start = start.SwizzleYX();
		end = end.SwizzleYX();
		delta = delta.SwizzleYX();
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

void TV::Renderer::DrawLine(Vec3f start, Vec3f end, ICanvas& canvas, const Colour& colour)
{
	DrawLine((int32)start.X, (int32)start.Y, (int32)end.X, (int32)end.Y, canvas, colour);
}

void TV::Renderer::DrawModelWireframe(const Model& model, ICanvas& canvas, const Colour& colour)
{
	const Vec3f offset = model.GetBoundsMin() * -1.f;
	const float scale = Min((float)canvas.GetSize().X / model.GetBoundsExtents().X, (float)canvas.GetSize().Y / model.GetBoundsExtents().Y) * 0.5f;

	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = model.GetTri(triIndex);
		const Vec3f a = (tri.Vertices[0].Position + offset) * scale;
		const Vec3f b = (tri.Vertices[1].Position + offset) * scale;
		const Vec3f c = (tri.Vertices[2].Position + offset) * scale;

		DrawLine(a, b, canvas, colour);
		DrawLine(a, c, canvas, colour);
		DrawLine(b, c, canvas, colour);
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

				Vec2i start = Lerp(a, c, bTopSegment ? 1.f - alpha : alpha);
				Vec2i end = Lerp(bTopSegment ? c : a, b, beta);

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
				const int32 otherX = Lerp(a, c, alpha).X;
				const int32 minX = Min(b.X, otherX);
				const int32 maxX = Max(b.X, otherX);
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
			min.X = Min(a.X, b.X, c.X);
			min.Y = Min(a.Y, b.Y, c.Y);
			max.X = Max(a.X, b.X, c.X);
			max.Y = Max(a.Y, b.Y, c.Y);

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

void TV::Renderer::DrawTriangle(const Matrix4x4f& modelViewMatrix, const Matrix4x4f& projectionMatrix, const Vertex& a, const Vertex& b, const Vertex& c, const TGAImage* diffuse, ICanvas& canvas, DepthBuffer& depthBuffer, const Colour& colour, const Vec3f& cameraSpaceLightDirection)
{
	// project vertices to clip space
	Vec3f cameraSpacePositions[3];
	Vec4f clipSpacePositions[3];
	Vec3f normalisedDeviceCoordPositions[3];
	Vec2f screenPositions[3];
	{
		const Vertex* const points[3] = { &a, &b, &c };
		const Vec2f canvasHalfSize = ToFloat(canvas.GetSize()) * 0.5f;

		for (int32 index = 0; index != 3; ++index)
		{
			cameraSpacePositions[index] = modelViewMatrix.TransformPosition(points[index]->Position);

			clipSpacePositions[index] = projectionMatrix.TransformVector4(Vec4f(cameraSpacePositions[index], 1.f));

			normalisedDeviceCoordPositions[index] = clipSpacePositions[index].GetProjected();

			screenPositions[index] = canvasHalfSize + canvasHalfSize * normalisedDeviceCoordPositions[index].XY();
		}
	}

	// get 2D bounding box of points
	Vec2f min, max;
	min.X = Min(screenPositions[0].X, screenPositions[1].X, screenPositions[2].X);
	min.X = Min(screenPositions[0].Y, screenPositions[1].Y, screenPositions[2].Y);
	min.X = Max(screenPositions[0].X, screenPositions[1].X, screenPositions[2].X);
	min.X = Max(screenPositions[0].Y, screenPositions[1].Y, screenPositions[2].Y);

	// clamp to bounds of canvas
	min.X = Max(min.X, 0.f);
	min.Y = Max(min.Y, 0.f);
	max.X = Min(max.X, canvas.GetSize().X - 1.f);
	max.Y = Min(max.Y, canvas.GetSize().Y - 1.f);

	const Vec2i minInt(FloorToInt(min.X), FloorToInt(min.Y));
	const Vec2i maxInt(CeilToInt(max.X), CeilToInt(max.Y));

	for (int32 x = minInt.X; x <= maxInt.X; ++x)
	{
		for (int32 y = minInt.Y; y != maxInt.Y; ++y)
		{
			const Vec2i point2D(x, y);
			const Vec3f barycentric = ComputeBarycentricCoordinate(ToFloat(point2D), a.Position.XY(), b.Position.XY(), c.Position.XY());
			if (barycentric.Min() <= 0.f)
			{
				// outside of poly
				continue;
			}

			const float depth = ComputeValueFromBarycentric(barycentric, cameraSpacePositions[0].Z, cameraSpacePositions[1].Z, cameraSpacePositions[2].Z);
			if (depthBuffer.Get(point2D) >= depth)
			{
				continue;
			}

			Colour pointColour(colour);
			if (diffuse != nullptr)
			{
				const Vec2f texCoord = ComputeValueFromBarycentric(barycentric, a.TexCoord, b.TexCoord, c.TexCoord);
				const Vec2i texSample(RoundToInt(texCoord.X * diffuse->GetSize().X), RoundToInt(texCoord.Y * diffuse->GetSize().Y));
				pointColour = diffuse->GetPixel(texSample);
			}

			Vec3f normal = ComputeValueFromBarycentric(barycentric, a.Normal, b.Normal, c.Normal);
			normal = modelViewMatrix.TransformVector(normal); // normal to camera space
			normal = normal.GetSafeNormal();

			const double lightIntensity = DotProduct(normal, cameraSpaceLightDirection);
			if (lightIntensity <= 0.0)
			{
				continue;
			}

			pointColour = pointColour.Scaled(lightIntensity);

			canvas.SetPixel(point2D, pointColour);
			depthBuffer.Set(point2D, depth);
		}
	}
}

void TV::Renderer::DrawModel(const Matrix4x4f& modelViewMatrix, const Matrix4x4f& projectionMatrix, const Model& model, const TGAImage* diffuse, ICanvas& canvas, DepthBuffer* depthBuffer, const Vec3f& cameraSpaceLightDirection)
{
	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		Model::Tri tri = model.GetTri(triIndex);

		if (depthBuffer != nullptr)
		{
			DrawTriangle(modelViewMatrix, projectionMatrix, tri.Vertices[0], tri.Vertices[1], tri.Vertices[2], diffuse, canvas, *depthBuffer, Colour(255, 255, 255), cameraSpaceLightDirection);
		}
		else
		{
			const Vec3f normal = model.CalculateNormal(triIndex);
			const double lightIntensity = DotProduct(normal, cameraSpaceLightDirection) * -1.0;
			if (lightIntensity <= 0.0)
			{
				continue;
			}

			const uint8 colourIntensity = RoundToInt(lightIntensity * 255.0);
			const Colour colour(colourIntensity, colourIntensity, colourIntensity);
			//const Colour colour = Colour::MakeRandomColour();
			//const Colour colour(255, 0, 0);

			DrawTriangle(
				Vec2i(RoundToInt(tri.Vertices[0].Position.X), RoundToInt(tri.Vertices[0].Position.Y)),
				Vec2i(RoundToInt(tri.Vertices[1].Position.X), RoundToInt(tri.Vertices[1].Position.Y)),
				Vec2i(RoundToInt(tri.Vertices[2].Position.X), RoundToInt(tri.Vertices[2].Position.Y)),
				canvas, colour);
		}
	}
}
