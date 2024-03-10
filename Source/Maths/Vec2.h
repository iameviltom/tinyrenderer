#pragma once

#include "Types.h"
#include "Maths.h"

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
			explicit TVec2(T inVal) : X(inVal), Y(inVal) {}
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

			[[nodiscard]] TVec2 Abs() const { return TVec2(TV::Maths::Abs(X), TV::Maths::Abs(Y)); }
		};

		template<class T, class U>
		inline [[nodiscard]] TVec2<T> Lerp(const TVec2<T>& a, const TVec2<T>& b, U alpha)
		{
			TVec2<T> vec;
			vec.X = Lerp(a.X, b.X, alpha);
			vec.Y = Lerp(a.Y, b.Y, alpha);
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec2<T> operator + (const TVec2<T>& a, const TVec2<T>& b)
		{
			return TVec2(a.X + b.X, a.Y + b.Y);
		}

		template<class T>
		inline [[nodiscard]] TVec2<T> operator - (const TVec2<T>& a, const TVec2<T>& b)
		{
			return TVec2<T>(a.X - b.X, a.Y - b.Y);
		}

		using Vec2i = TVec2<int32>;
		using Vec2f = TVec2<float>;
		using Vec2d = TVec2<double>;

		inline [[nodiscard]] Vec2f ToFloat(const Vec2i& vec)
		{
			return Vec2f((float)vec.X, (float)vec.Y);
		}
	}
}