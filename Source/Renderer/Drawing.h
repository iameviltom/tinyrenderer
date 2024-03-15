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
		struct Vertex;

		struct RenderContext
		{
			Matrix4x4f ViewMatrix; // inverse camera matrix
			Matrix4x4f ProjectionMatrix;
			Vec3f CameraSpaceLightDirection;
			DepthBuffer* DepthBuffer = nullptr;
			ICanvas* Canvas = nullptr;

			bool IsValid() const { return DepthBuffer != nullptr && Canvas != nullptr; }
		};

		struct RenderParams
		{
			Matrix4x4f Matrix;
			ICanvas* Diffuse = nullptr;
			Colour Colour;
		};

		void DrawLine(Vec2i start, Vec2i end, ICanvas& canvas, const Colour& colour);
		void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, ICanvas& canvas, const Colour& colour);
		void DrawLine(const RenderContext& context, const RenderParams& params, const Vec3f& start, const Vec3f& end);

		void DrawModelWireframe(const Model& model, const RenderContext& context, const RenderParams& params);

		void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, ICanvas& canvas, const Colour& colour);
		void DrawTriangle(const RenderContext& context, const RenderParams& params, const Vertex& a, const Vertex& b, const Vertex& c);

		void DrawModel(const Model& model, const RenderContext& context, const RenderParams& params);
	}
}