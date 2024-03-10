#pragma once

#include "Types.h"
#include <cmath>

namespace TV
{
	namespace Maths
	{
		template<class T>
		inline [[nodiscard]] T Sign(const T& a)
		{
			return (a >= T(0)) ? T(1) : T(-1);
		}

		template<class T>
		inline [[nodiscard]] T Abs(const T& a)
		{
			return Sign(a) * a;
		}

		template<class T>
		inline [[nodiscard]] T Floor(T a)
		{
			const T sign = Sign(a);
			const T abs = sign * a;
			const T absFloor = (T)((int32)abs);
			return absFloor * sign;
		}

		template<class T>
		inline [[nodiscard]] int32 FloorToInt(T a) { return (int32)Floor(a); }

		template<class T>
		inline [[nodiscard]] T Ceil(T a)
		{
			const T sign = Sign(a);
			const T abs = sign * a;
			const T absFloor = (T)((int32)abs);
			if (abs == absFloor)
			{
				return a;
			}
			else
			{
				return (absFloor + 1) * sign;
			}
		}

		template<class T>
		inline [[nodiscard]] int32 CeilToInt(T a) { return (int32)Ceil(a); }

		template<class T>
		inline [[nodiscard]] int32 RoundToInt(T a)
		{
			const T sign = Sign(a);
			const T abs = a * sign;
			const int32 base = (int32)abs;
			const T frac = abs - (T)base;
			const int32 absRounded = (frac >= 0.5f) ? base + 1 : base;
			return absRounded * (int32)sign;
		}

		inline [[nodiscard]] float Sqrt(float val) { return std::sqrt(val); }
		inline [[nodiscard]] double Sqrt(double val) { return std::sqrt(val); }

		template<class T>
		inline [[nodiscard]] T Min(const T& a, const T& b)
		{
			return (a > b) ? b : a;
		}

		template<class T>
		inline [[nodiscard]] T Min(const T& a, const T& b, const T& c)
		{
			return Min(Min(a, b), c);
		}

		template<class T>
		inline [[nodiscard]] T Max(const T& a, const T& b)
		{
			return (a > b) ? a : b;
		}

		template<class T>
		inline [[nodiscard]] T Max(const T& a, const T& b, const T& c)
		{
			return Max(Max(a, b), c);
		}

		template<class T, class U>
		inline [[nodiscard]] T Lerp(const T& a, const T& b, U alpha)
		{
			return T(a + (b - a) * alpha);
		}
	}
}