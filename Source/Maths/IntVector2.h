#pragma once

#include <cstdlib>

class IntVector2
{
public:
	int X = 0;
	int Y = 0;

	IntVector2() {}
	IntVector2(int InX, int InY) : X(InX), Y(InY) {}
	
	bool operator == (const IntVector2& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	IntVector2& operator += (const IntVector2& Other)
	{
		X += Other.X;
		Y += Other.Y;
		return *this;
	}

	IntVector2& operator -= (const IntVector2& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		return *this;
	}

	IntVector2& operator *= (int Factor)
	{
		X *= Factor;
		Y *= Factor;
		return *this;
	}

	[[nodiscard]] IntVector2 SwizzleYX() const { return IntVector2(Y, X); }

	[[nodiscard]] IntVector2 Abs() const { return IntVector2(std::abs(X), std::abs(Y)); }
};

inline IntVector2 operator + (const IntVector2& A, const IntVector2& B)
{
	return IntVector2(A.X + B.X, A.Y + B.Y);
}

inline IntVector2 operator - (const IntVector2& A, const IntVector2& B)
{
	return IntVector2(A.X - B.X, A.Y - B.Y);
}

