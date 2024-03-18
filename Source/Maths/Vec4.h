#pragma once

#include "Vec3.h"

namespace TV
{
	namespace Maths
	{
		template<class T>
		class TVec4
		{
		public:
			union
			{
				struct
				{
					T X;
					T Y;
					T Z;
					T W;
				};
				T Raw[4];
			};

			TVec4() : X(0), Y(0), Z(0), W(1) {}
			TVec4(T inVal) : X(inVal), Y(inVal), Z(inVal), W(1) {}
			TVec4(T inX, T inY, T inZ) : X(inX), Y(inY), Z(inZ), W(1) {}
			TVec4(const TVec3<T>& vec3, T inW = 1) : X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(inW) {}

			[[nodiscard]] TVec3<T> GetProjected() const
			{
				const T invW = 1 / W;
				TVec3<T> ret;
				ret.X = X * invW;
				ret.Y = Y * invW;
				ret.Z = Z * invW;
				return ret;
			}
			[[nodiscard]] TVec3<T> GetUnprojected() const
			{
				return TVec3<T>(X, Y, Z);
			}
			[[nodiscard]] bool IsClipped() const
			{
				return GetMax(X, Y, Z) > W;
			}

			TVec4<T>& operator += (const TVec4<T>& other)
			{
				X += other.X;
				Y += other.Y;
				Z += other.Z;
				W += other.W;
				return *this;
			}

			TVec4<T>& operator *= (T factor)
			{
				X *= factor;
				Y *= factor;
				Z *= factor;
				W *= factor;
				return *this;
			}
		};

		template<class T>
		inline [[nodiscard]] TVec4<T> operator * (const TVec4<T>& vector, T factor)
		{
			TVec4<T> vec(vector);
			vec *= factor;
			return vec;
		}

		using Vec4f = TVec4<float>;
		using Vec4d = TVec4<double>;
	}
}