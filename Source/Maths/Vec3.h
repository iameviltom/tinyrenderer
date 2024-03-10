#pragma once

#include <cstdlib>
#include <xutility>

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
	TVec3(T InVal) : X(InVal), Y(InVal), Z(InVal) {}
	TVec3(T InX, T InY, T InZ) : X(InX), Y(InY), Z(InZ) {}

	bool operator == (const TVec3& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}

	TVec3& operator += (const TVec3& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}

	TVec3& operator -= (const TVec3& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		Z -= Other.Z;
		return *this;
	}

	TVec3& operator *= (T Factor)
	{
		X *= Factor;
		Y *= Factor;
		Z *= Factor;
		return *this;
	}

	TVec3& operator *= (TVec3 Factor)
	{
		X *= Factor.X;
		Y *= Factor.Y;
		Z *= Factor.Z;
		return *this;
	}

	TVec3& operator /= (T Divisor)
	{
		*this *= (1 / Divisor);
		return *this;
	}

	TVec3& operator /= (const TVec3& Divisor)
	{
		*this *= Divisor.Reciprocal();
		return *this;
	}

	[[nodiscard]] TVec3 Abs() const { return TVec3(std::abs(X), std::abs(Y), std::abs(Z)); }

	[[nodiscard]] TVec3 Reciprocal() const { return TVec3(1 / X, 1 / Y, 1 / Z); }

	template<class T>
	static TVec3<T> Min(const TVec3<T>& A, const TVec3<T>& B)
	{
		return TVec3(std::min(A.X, B.X), std::min(A.Y, B.Y), std::min(A.Z, B.Z));
	}

	template<class T>
	static TVec3<T> Max(const TVec3<T>& A, const TVec3<T>& B)
	{
		return TVec3(std::max(A.X, B.X), std::max(A.Y, B.Y), std::max(A.Z, B.Z));
	}
};

template<class T>
inline TVec3<T> operator + (const TVec3<T>& A, const TVec3<T>& B)
{
	return TVec3<T>(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

template<class T>
inline TVec3<T> operator - (const TVec3<T>& A, const TVec3<T>& B)
{
	return TVec3<T>(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

template<class T>
inline TVec3<T> operator * (const TVec3<T>& Vector, float Factor)
{
	TVec3<T> vec(Vector);
	vec *= Factor;
	return vec;
}

template<class T>
inline TVec3<T> operator * (const TVec3<T>& Vector, const TVec3<T>& Factor)
{
	TVec3<T> vec(Vector);
	vec *= Factor;
	return vec;
}

template<class T>
inline TVec3<T> operator / (const TVec3<T>& Vector, float Divisor)
{
	TVec3<T> vec(Vector);
	vec /= Divisor;
	return vec;
}

template<class T>
inline TVec3<T> operator / (const TVec3<T>& Vector, const TVec3<T>& Divisor)
{
	TVec3<T> vec(Vector);
	vec /= Divisor;
	return vec;
}

using Vec3i = TVec3<int>;
using Vec3f = TVec3<float>;
using Vec3d = TVec3<double>;
