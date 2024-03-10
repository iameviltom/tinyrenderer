#pragma once

#include <cstdlib>

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
	TVec2(T InX, T InY) : X(InX), Y(InY) {}

	bool operator == (const TVec2& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	TVec2& operator += (const TVec2& Other)
	{
		X += Other.X;
		Y += Other.Y;
		return *this;
	}

	TVec2& operator -= (const TVec2& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		return *this;
	}

	TVec2& operator *= (T Factor)
	{
		X *= Factor;
		Y *= Factor;
		return *this;
	}

	[[nodiscard]] TVec2 SwizzleYX() const { return TVec2(Y, X); }

	[[nodiscard]] TVec2 Abs() const { return TVec2(std::abs(X), std::abs(Y)); }
};

template<class T>
inline TVec2<T> operator + (const TVec2<T>& A, const TVec2<T>& B)
{
	return TVec2(A.X + B.X, A.Y + B.Y);
}

template<class T>
inline TVec2<T> operator - (const TVec2<T>& A, const TVec2<T>& B)
{
	return TVec2<T>(A.X - B.X, A.Y - B.Y);
}

using Vec2i = TVec2<int>;
using Vec2f = TVec2<float>;
using Vec2d = TVec2<double>;
