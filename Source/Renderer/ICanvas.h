#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Colour.h"

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class ICanvas
		{
		public:
			virtual ~ICanvas() {}

			virtual Vec2i GetSize() const = 0;
			virtual void SetPixel(const Vec2i& coord, const Colour& colour) = 0;
			virtual Colour GetPixel(const Vec2i& coord) const = 0;
		};
	}
}