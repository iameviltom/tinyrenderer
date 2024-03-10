#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Colour.h"
#include "../Maths/Vec3.h"

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class Model;
		class ICanvas;

		void DrawLine(Vec2i start, Vec2i end, ICanvas& canvas, const Colour& colour);
		void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, ICanvas& canvas, const Colour& colour);
		void DrawLine(Vec3f start, Vec3f end, ICanvas& canvas, const Colour& colour);

		void DrawModelWireframe(const Model& model, ICanvas& canvas, const Colour& colour);

		void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour);

		void DrawModel(const Model& model, ICanvas& canvas);
	}
}