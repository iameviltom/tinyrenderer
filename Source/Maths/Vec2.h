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

			TVec2<T>& operator += (const TVec2<T>& other)
			{
				X += other.X;
				Y += other.Y;
				return *this;
			}

			TVec2<T>& operator -= (const TVec2<T>& other)
			{
				X -= other.X;
				Y -= other.Y;
				return *this;
			}

			TVec2<T>& operator *= (T factor)
			{
				X *= factor;
				Y *= factor;
				return *this;
			}

			TVec2<T>& operator *= (TVec2<T> factor)
			{
				X *= factor.X;
				Y *= factor.Y;
				return *this;
			}

			[[nodiscard]] TVec2<T> GetSwizzledYX() const { return TVec2(Y, X); }

			[[nodiscard]] TVec2<T> GetAbs() const { return TVec2(TV::Maths::GetAbs(X), TV::Maths::GetAbs(Y)); }

			[[nodiscard]] TVec2<T> GetClamped(const TVec2<T>& min, const TVec2& max) const { return TVec2(TV::Maths::GetClamped(X, min.X, max.X), TV::Maths::GetClamped(Y, min.Y, max.Y)); }
		};

		template<class T, class U>
		inline [[nodiscard]] TVec2<T> GetLerp(const TVec2<T>& a, const TVec2<T>& b, U alpha)
		{
			TVec2<T> vec;
			vec.X = GetLerp(a.X, b.X, alpha);
			vec.Y = GetLerp(a.Y, b.Y, alpha);
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec2<T> operator + (const TVec2<T>& a, const TVec2<T>& b)
		{
			return TVec2<T>(a.X + b.X, a.Y + b.Y);
		}

		template<class T>
		inline [[nodiscard]] TVec2<T> operator - (const TVec2<T>& a, const TVec2<T>& b)
		{
			return TVec2<T>(a.X - b.X, a.Y - b.Y);
		}

		template<class T>
		inline [[nodiscard]] TVec2<T> operator * (const TVec2<T>& vector, float factor)
		{
			TVec2<T> vec(vector);
			vec *= factor;
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec2<T> operator * (const TVec2<T>& vector, const TVec2<T>& factor)
		{
			TVec2<T> vec(vector);
			vec *= factor;
			return vec;
		}

		template<class T>
		inline [[nodiscard]] double GetDotProduct(const TVec2<T>& a, const TVec2<T>& b)
		{
			return (a.X * b.X) + (a.Y * b.Y);
		}

		using Vec2i = TVec2<int32>;
		using Vec2f = TVec2<float>;
		using Vec2d = TVec2<double>;

		inline [[nodiscard]] Vec2f ToFloat(const Vec2i& vec)
		{
			return Vec2f((float)vec.X, (float)vec.Y);
		}

		inline [[nodiscard]] Vec2i GetRoundToInt(const Vec2f& vec)
		{
			return Vec2i(GetRoundToInt(vec.X), GetRoundToInt(vec.Y));
		}
	}
}