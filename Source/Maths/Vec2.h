#pragma once

#include <cstdlib>
#include "Types.h"

namespace TV
{
	namespace Maths
	{
		template<class T>
		class TVec2
		{
		public:
			union
			{
				struct
				{
					T X;
					T Y;
				};
				T Raw[2];
			};

			TVec2() : X(0), Y(0) {}
			TVec2(T inX, T inY) : X(inX), Y(inY) {}

			bool operator == (const TVec2& other) const
			{
				return X == other.X && Y == other.Y;
			}

			TVec2& operator += (const TVec2& other)
			{
				X += other.X;
				Y += other.Y;
				return *this;
			}

			TVec2& operator -= (const TVec2& other)
			{
				X -= other.X;
				Y -= other.Y;
				return *this;
			}

			TVec2& operator *= (T factor)
			{
				X *= factor;
				Y *= factor;
				return *this;
			}

			[[nodiscard]] TVec2 SwizzleYX() const { return TVec2(Y, X); }

			[[nodiscard]] TVec2 Abs() const { return TVec2(std::abs(X), std::abs(Y)); }
		};

		template<class T>
		inline TVec2<T> operator + (const TVec2<T>& a, const TVec2<T>& b)
		{
			return TVec2(a.X + b.X, a.Y + b.Y);
		}

		template<class T>
		inline TVec2<T> operator - (const TVec2<T>& a, const TVec2<T>& b)
		{
			return TVec2<T>(a.X - b.X, a.Y - b.Y);
		}

		using Vec2i = TVec2<int32>;
		using Vec2f = TVec2<float>;
		using Vec2d = TVec2<double>;
	}
}