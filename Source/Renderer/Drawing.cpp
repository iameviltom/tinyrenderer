#include "Drawing.h"

#include "ICanvas.h"
#include "../Model/Model.h"
#include "../Maths/Types.h"

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
	const float scale = std::min((float)canvas.GetSize().X / model.GetBoundsExtents().X, (float)canvas.GetSize().Y / model.GetBoundsExtents().Y) * 0.5f;

	for (int triIndex = 0; triIndex != model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = model.GetTri(triIndex);
		const Vec3f a = (model.GetVertex(tri.VertIndex[0]) + offset) * scale;
		const Vec3f b = (model.GetVertex(tri.VertIndex[1]) + offset) * scale;
		const Vec3f c = (model.GetVertex(tri.VertIndex[2]) + offset) * scale;

		DrawLine(a, b, canvas, colour);
		DrawLine(a, c, canvas, colour);
		DrawLine(b, c, canvas, colour);
	}
}

namespace TV
{
	namespace Renderer
	{
		void DrawHalfTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour)
		{

		}
	}
}

void TV::Renderer::DrawTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour)
{
	// sort vertices, to draw from min y to max y
	if (a.Y > b.Y) { std::swap(a, b); }
	if (a.Y > c.Y) { std::swap(a, c); }
	if (b.Y > c.Y) { std::swap(b, c); }

	int32 totalHeight = c.Y - a.Y;
	if (totalHeight == 0)
	{
		// this is just a horizontal line
		const int32 minX = std::min(std::min(a.X, b.X), c.X);
		const int32 maxX = std::max(std::max(a.X, b.X), c.X);
		for (int32 x = minX; x <= maxX; ++x)
		{
			canvas.SetPixel(Vec2i(x, a.Y), colour);
		}
		return;
	}

	const int32 topSegmentHeight = c.Y - b.Y;
	const int32 bottomSegmentHeight = b.Y - a.Y;

	for (int32 yOffset = 0; yOffset != totalHeight; ++yOffset)
	{
		const bool bTopSegment = yOffset > bottomSegmentHeight;
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
		for (int32 x = start.X; x != end.X; ++x)
		{
			canvas.SetPixel(Vec2i(x, y), colour);
		}
	}

#if 0
	// draw bottom half of the triangle
	{
		const int32 bottomSegmentHeight = b.Y - a.Y;
		if (bottomSegmentHeight == 0)
		{
			const int32 minX = std::min(a.X, b.X);
			const int32 maxX = std::max(a.X, b.X);
			for (int32 x = minX; x <= maxX; ++x)
			{
				canvas.SetPixel(Vec2i(x, a.Y), colour);
			}
		}
		else
		{
			for (int32 yOffset = 0; yOffset != bottomSegmentHeight; ++yOffset)
			{
				const int32 y = a.Y + yOffset;

				const float alphaBottom = yOffset / (float)totalHeight;
				const float betaBottom = yOffset / (float)bottomSegmentHeight;
				Vec2i start = Lerp(a, c, alphaBottom);
				Vec2i end = Lerp(a, b, betaBottom);

				// sort left to right
				if (start.X > end.X)
				{
					std::swap(start, end);
				}
				for (int32 x = start.X; x != end.X; ++x)
				{
					canvas.SetPixel(Vec2i(x, y), colour);
				}
			}
		}
	}

	// draw top half of the triangle
	{
		const int32 topSegmentHeight = c.Y - b.Y;
		if (topSegmentHeight == 0)
		{
			const int32 minX = std::min(b.X, c.X);
			const int32 maxX = std::max(b.X, c.X);
			for (int32 x = minX; x <= maxX; ++x)
			{
				canvas.SetPixel(Vec2i(x, c.Y), colour);
			}
		}
		else
		{
			for (int32 yOffset = 0; yOffset != topSegmentHeight; ++yOffset)
			{
				const int32 y = c.Y - yOffset;

				const float alphaBottom = yOffset / (float)totalHeight;
				const float betaBottom = yOffset / (float)topSegmentHeight;
				Vec2i start = Lerp(c, a, alphaBottom);
				Vec2i end = Lerp(c, b, betaBottom);

				// sort left to right
				if (start.X > end.X)
				{
					std::swap(start, end);
				}
				for (int32 x = start.X; x != end.X; ++x)
				{
					canvas.SetPixel(Vec2i(x, y), colour);
				}
			}
		}
	}
#endif
#if 0
	DrawLine(a, b, canvas, colour);
	DrawLine(a, c, canvas, colour);
	DrawLine(b, c, canvas, colour);
#endif
}
