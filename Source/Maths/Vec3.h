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

			TVec3<T>& operator += (const TVec3<T>& other)
			{
				X += other.X;
				Y += other.Y;
				Z += other.Z;
				return *this;
			}

			TVec3<T>& operator -= (const TVec3<T>& other)
			{
				X -= other.X;
				Y -= other.Y;
				Z -= other.Z;
				return *this;
			}

			TVec3<T>& operator *= (T factor)
			{
				X *= factor;
				Y *= factor;
				Z *= factor;
				return *this;
			}

			TVec3<T>& operator *= (TVec3<T> factor)
			{
				X *= factor.X;
				Y *= factor.Y;
				Z *= factor.Z;
				return *this;
			}

			TVec3<T>& operator /= (T divisor)
			{
				*this *= (1 / divisor);
				return *this;
			}

			TVec3<T>& operator /= (const TVec3& divisor)
			{
				*this *= divisor.GetReciprocal();
				return *this;
			}

			[[nodiscard]] TVec3<T> GetAbs() const { return TVec3<T>(GetAbs(X), GetAbs(Y), GetAbs(Z)); }

			[[nodiscard]] TVec3<T> GetFloor() const { return TVec3<T>(GetFloor(X), GetFloor(Y), GetFloor(Z)); }

			[[nodiscard]] TVec3<T> GetCeil() const { return TVec3<T>(GetCeil(X), GetCeil(Y), GetCeil(Z)); }

			[[nodiscard]] TVec2<T> GetXY() const { return TVec2<T>(X, Y); }

			[[nodiscard]] TVec3<T> GetReciprocal() const { return TVec3<T>(1 / X, 1 / Y, 1 / Z); }

			[[nodiscard]] T GetMin() const { return TV::Maths::GetMin(X, Y, Z); }
			[[nodiscard]] T GetMax() const { return TV::Maths::GetMax(X, Y, Z); }

			[[nodiscard]] double GetLength() const { return GetSqrt(X * X + Y * Y + Z * Z); }

			[[nodiscard]] TVec3<T> GetSafeNormal() const
			{
				const double size = GetLength();
				if (size < C_KindaSmallNumber)
				{
					return TVec3<T>();
				}
				TVec3<T> vec(*this);
				vec *= T(1.0 / size);
				return vec;
			}
		};

		template<class T>
		inline [[nodiscard]] TVec3<T> GetMin(const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(GetMin(a.X, b.X), GetMin(a.Y, b.Y), GetMin(a.Z, b.Z));
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> GetMax(const TVec3<T>& a, const TVec3<T>& b)
		{
			return TVec3<T>(GetMax(a.X, b.X), GetMax(a.Y, b.Y), GetMax(a.Z, b.Z));
		}

		template<class T, class U>
		inline [[nodiscard]] TVec3<T> GetLerp(const TVec3<T>& a, const TVec3<T>& b, U alpha)
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
		inline [[nodiscard]] double GetDotProduct(const TVec3<T>& a, const TVec3<T>& b)
		{
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
		}

		template<class T>
		inline [[nodiscard]] TVec3<T> GetCrossProduct(const TVec3<T>& a, const TVec3<T>& b)
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
