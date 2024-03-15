#pragma once

#include "Types.h"
#include <cmath>

namespace TV
{
	namespace Maths
	{
		template<class T>
		inline [[nodiscard]] T GetSign(const T& a)
		{
			return (a >= T(0)) ? T(1) : T(-1);
		}

		template<class T>
		inline [[nodiscard]] T GetAbs(const T& a)
		{
			return GetSign(a) * a;
		}

		template<class T>
		inline [[nodiscard]] T GetFloor(T a)
		{
			const T sign = GetSign(a);
			const T abs = sign * a;
			const T absFloor = (T)((int32)abs);
			return absFloor * sign;
		}

		template<class T>
		inline [[nodiscard]] int32 GetFloorToInt(T a) { return (int32)GetFloor(a); }

		template<class T>
		inline [[nodiscard]] T GetCeil(T a)
		{
			const T sign = GetSign(a);
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
		inline [[nodiscard]] int32 GetCeilToInt(T a) { return (int32)GetCeil(a); }

		template<class T>
		inline [[nodiscard]] int32 GetRoundToInt(T a)
		{
			const T sign = GetSign(a);
			const T abs = a * sign;
			const int32 base = (int32)abs;
			const T frac = abs - (T)base;
			const int32 absRounded = (frac >= 0.5f) ? base + 1 : base;
			return absRounded * (int32)sign;
		}

		inline [[nodiscard]] float GetSqrt(float val) { return std::sqrt(val); }
		inline [[nodiscard]] double GetSqrt(double val) { return std::sqrt(val); }

		template<class T>
		inline [[nodiscard]] T GetMin(const T& a, const T& b)
		{
			return (a > b) ? b : a;
		}

		template<class T>
		inline [[nodiscard]] T GetMin(const T& a, const T& b, const T& c)
		{
			return GetMin(GetMin(a, b), c);
		}

		template<class T>
		inline [[nodiscard]] T GetMax(const T& a, const T& b)
		{
			return (a > b) ? a : b;
		}

		template<class T>
		inline [[nodiscard]] T GetMax(const T& a, const T& b, const T& c)
		{
			return GetMax(GetMax(a, b), c);
		}

		template<class T>
		inline [[nodiscard]] T GetClamped(const T& a, const T& lowerBound, const T& upperBound)
		{
			return GetMin(upperBound, GetMax(lowerBound, a));
		}

		template<class T, class U>
		inline [[nodiscard]] T GetLerp(const T& a, const T& b, U alpha)
		{
			return T(a + (b - a) * alpha);
		}

		template<class T>
		inline [[nodiscard]] T GetRangePct(const T& a, const T& b, const T& value)
		{
			const T range = b - a;
			if (GetAbs(range) < C_SmallNumber)
			{
				return value > b ? (T)1 : (T)0;
			}
			return (value - a) / range;
		}
	}
}