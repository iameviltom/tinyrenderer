#pragma once

namespace TV
{
	namespace Maths
	{
		template<class T>
		[[nodiscard]] T Sign(const T& a)
		{
			return (a >= T(0)) ? T(1) : T(-1);
		}

		template<class T>
		[[nodiscard]] T Abs(const T& a)
		{
			return Sign(a) * a;
		}

		template<class T>
		[[nodiscard]] T Min(const T& a, const T& b)
		{
			return (a > b) ? b : a;
		}

		template<class T>
		[[nodiscard]] T Min(const T& a, const T& b, const T& c)
		{
			return Min(Min(a, b), c);
		}

		template<class T>
		[[nodiscard]] T Max(const T& a, const T& b)
		{
			return (a > b) ? a : b;
		}

		template<class T>
		[[nodiscard]] T Max(const T& a, const T& b, const T& c)
		{
			return Max(Max(a, b), c);
		}

		template<class T, class U>
		[[nodiscard]] T Lerp(const T& a, const T& b, U alpha)
		{
			return T(a + (b - a) * alpha);
		}
	}
}