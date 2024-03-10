#pragma once

#include "Types.h"

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

		inline [[nodiscard]] int32 RoundToInt(float a)
		{
			const float sign = Sign(a);
			const float abs = a * sign;
			const int32 base = (int32)abs;
			const float frac = abs - (float)base;
			const int32 absRounded = (frac >= 0.5f) ? base + 1 : base;
			return absRounded * (int32)sign;
		}

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