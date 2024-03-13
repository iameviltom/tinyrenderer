#pragma once

#include "../Maths/Vec2.h"
#include "../Maths/Assert.h"

#include <cstring>

namespace TV
{
	namespace Renderer
	{
		using namespace Maths;

		class DepthBuffer
		{
		public:
			using BufferType = float;

			DepthBuffer(const Vec2i& size) : Size(size), Buffer(new BufferType[size.X * size.Y])
			{
				ClearBuffer();
			}
			~DepthBuffer() { delete Buffer; }

			void Set(const Vec2i& point, BufferType value)
			{
				ValidatePoint(point);
				Buffer[point.X + point.Y * Size.X] = value;
			}
			BufferType Get(const Vec2i& point) const
			{
				ValidatePoint(point);
				return Buffer[point.X + point.Y * Size.X];
			}

			void ClearBuffer()
			{
				std::memset(Buffer, 0, Size.X * Size.Y);
			}

			void ValidatePoint(const Vec2i& point) const
			{
				check(point.X >= 0);
				check(point.Y >= 0);
				check(point.X < Size.X);
				check(point.Y < Size.Y);
			}

		private:
			const Vec2i Size;
			BufferType* const Buffer;
		};
	}
}
