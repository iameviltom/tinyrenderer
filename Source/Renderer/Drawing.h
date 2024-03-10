#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Colour.h"
#include "../Maths/Vec3.h"

class TGAImage;

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class Model;
		class ICanvas;
		class DepthBuffer;
		struct Vertex;

		void DrawLine(Vec2i start, Vec2i end, ICanvas& canvas, const Colour& colour);
		void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, ICanvas& canvas, const Colour& colour);
		void DrawLine(Vec3f start, Vec3f end, ICanvas& canvas, const Colour& colour);

		void DrawModelWireframe(const Model& model, ICanvas& canvas, const Colour& colour);

		void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour);
		void DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c, const TGAImage* diffuse, ICanvas& canvas, DepthBuffer& depthBuffer, const Colour& colour, const Vec3f& lightDirection);

		void DrawModel(const Model& model, const TGAImage* diffuse, ICanvas& canvas, DepthBuffer* depthBuffer, const Vec3f& lightDirection);
	}
}