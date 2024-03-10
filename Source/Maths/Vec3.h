#pragma once

#include "Types.h"
#include "Maths.h"
#include "Vec2.h"

namespace TV
{
	namespace Maths
	{
		template<class T>
		class TVec3
		{
		public:
			union
			{
				struct
				{
					T X;
					T Y;
					T Z;
				};
				T Raw[3];
			};

			TVec3() : X(0), Y(0), Z(0) {}
			explicit TVec3(T inVal) : X(inVal), Y(inVal), Z(inVal) {}
			TVec3(T inX, T inY, T inZ) : X(inX), Y(inY), Z(inZ) {}

			bool operator == (const TVec3& other) const
			{
				return X == other.X && Y == other.Y && Z == other.Z;
			}

			TVec3& operator += (const TVec3& other)
			{
				X += other.X;
				Y += other.Y;
				Z += other.Z;
				return *this;
			}

			TVec3& operator -= (const TVec3& other)
			{
				X -= other.X;
				Y -= other.Y;
				Z -= other.Z;
				return *this;
			}

			TVec3& operator *= (T factor)
			{
				X *= factor;
				Y *= factor;
				Z *= factor;
				return *this;
			}

			TVec3& operator *= (TVec3 factor)
			{
				X *= factor.X;
				Y *= factor.Y;
				Z *= factor.Z;
				return *this;
			}

			TVec3& operator /= (T divisor)
			{
				*this *= (1 / divisor);
				return *this;
			}

			TVec3& operator /= (const TVec3& divisor)
			{
				*this *= divisor.Reciprocal();
				return *this;
			}

			[[nodiscard]] TVec3 Abs() const { return TVec3(Abs(X), Abs(Y), Abs(Z)); }

			[[nodiscard]] TVec3 Floor() const { return TVec3(Floor(X), Floor(Y), Floor(Z)); }

			[[nodiscard]] TVec3 Ceil() const { return TVec3(Ceil(X), Ceil(Y), Ceil(Z)); }

			[[nodiscard]] TVec2<T> XY() const { return TVec2<T>(X, Y); }

			[[nodiscard]] TVec3 Reciprocal() const { return TVec3(1 / X, 1 / Y, 1 / Z); }

			[[nodiscard]] T Min() const { return TV::Maths::Min(X, Y, Z); }
			[[nodiscard]] T Max() const { return TV::Maths::Max(X, Y, Z); }

			[[nodiscard]] double Length() const { return Sqrt(X * X + Y * Y + Z * Z); }

			[[nodiscard]] TVec3 GetSafeNormal() const
			{
				const double size = Length();
				if (size < C_KindaSmallNumber)
				{
					return TVec3();
				}
				TVec3 vec(*this);
				vec *= T(1.0 / size);
				return vec;
			}
		};

		template<class T>
		inline [[nodiscard]] TVec3<T> Min(const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(Min(a.X, b.X), Min(a.Y, b.Y), Min(a.Z, b.Z));
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> Max(const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(Max(a.X, b.X), Max(a.Y, b.Y), Max(a.Z, b.Z));
		}

		template<class T, class U>
		inline [[nodiscard]] TVec3<T> Lerp(const TVec3<T>& a, const TVec3<T>& b, U alpha)
		{
			TVec3<T> vec;
			vec.X = Lerp(a.X, b.X, alpha);
			vec.Y = Lerp(a.Y, b.Y, alpha);
			vec.Z = Lerp(a.Z, b.Z, alpha);
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> operator + (const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> operator - (const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> operator * (const TVec3<T>& vector, float factor)
		{
			TVec3<T> vec(vector);
			vec *= factor;
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> operator * (const TVec3<T>& vector, const TVec3<T>& factor)
		{
			TVec3<T> vec(vector);
			vec *= factor;
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> operator / (const TVec3<T>& vector, float divisor)
		{
			TVec3<T> vec(vector);
			vec /= divisor;
			return vec;
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> operator / (const TVec3<T>& vector, const TVec3<T>& divisor)
		{
			TVec3<T> vec(vector);
			vec /= divisor;
			return vec;
		}

		template<class T>
		inline [[nodiscard]] double DotProduct(const TVec3<T>& a, const TVec3<T>& b)
		{
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> CrossProduct(const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(
				a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
		}

		using Vec3i = TVec3<int32>;
		using Vec3f = TVec3<float>;
		using Vec3d = TVec3<double>;
	}
}
