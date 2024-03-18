#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Colour.h"
#include "../Maths/Vec3.h"
#include "../Maths/Matrix4x4.h"

class TGAImage;

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class Model;
		class ICanvas;
		class DepthBuffer;
		class IShader;
		struct Vertex;

		struct RenderContext
		{
			DepthBuffer* DepthBuffer = nullptr;
			ICanvas* Canvas = nullptr;

			bool IsValid() const { return DepthBuffer != nullptr && Canvas != nullptr; }
		};

		void DrawLine(Vec2i start, Vec2i end, ICanvas& canvas, const Colour& colour);
		void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, ICanvas& canvas, const Colour& colour);

		void DrawModelWireframe(const Model& model, const RenderContext& context, IShader& shader, const Colour& colour);

		void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour);
		void DrawTriangle(const RenderContext& context, IShader& shader, int32 vertexIndexA, int32 vertexIndexB, int32 vertexIndexC);

		void DrawModel(const Model& model, const RenderContext& context, IShader& shader);
	}
}