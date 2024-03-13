#pragma once
#include "Vec3.h"
#include "Types.h"

namespace TV
{
	namespace Maths
	{
		// Matrix uses column major convention
		template<class T>
		class TMatrix4x4
		{
		public:
			union
			{
				struct
				{
					T M00;
					T M01;
					T M02;
					T M03;

					T M10;
					T M11;
					T M12;
					T M13;

					T M20;
					T M21;
					T M22;
					T M23;

					T M30;
					T M31;
					T M32;
					T M33;
				};

				struct
				{
					T M[4][4];
				};

				T Raw[16];
			};

			TMatrix4x4() { SetIdentity(); }

			void SetIdentity();
			void SetScale(const TVec3<T>& scale);
			void SetTranslation(const TVec3<T>& translation);
			void SetTranspose();
			void SetInverse();

			[[nodiscard]] TMatrix4x4<T> GetTranspose() const;
			[[nodiscard]] TMatrix4x4<T> GetInverse() const;

			TMatrix4x4<T>& operator *= (const TMatrix4x4<T>& other);

			static [[nodiscard]] TMatrix4x4 MakeScale(const TVec3<T>& scale);
			static [[nodiscard]] TMatrix4x4 MakeTranslation(const TVec3<T>& translation);
		};

		using Matrix4x4f = TMatrix4x4<float>;
		using Matrix4x4d = TMatrix4x4<double>;

		template<class T>
		[[nodiscard]] TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::MakeTranslation(const TVec3<T>& translation)
		{
			TMatrix4x4<T> ret;
			ret.SetTranslation(translation);
			return ret;
		}

		template<class T>
		[[nodiscard]] TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::MakeScale(const TVec3<T>& scale)
		{
			TMatrix4x4<T> ret;
			ret.SetScale(scale);
			return ret;
		}

		template<class T>
		TV::Maths::TMatrix4x4<T>& TV::Maths::TMatrix4x4<T>::operator*=(const TMatrix4x4<T>& other)
		{
			const TMatrix4x4 result = *this * other;
		}

		template<class T>
		TV::Maths::TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::GetTranspose() const
		{
			TMatrix4x4<T> transposed;
			for (int32 i = 0; i != 4; ++i)
			{
				for (int32 j = 0; j != 4; ++j)
				{
					transposed.M[i][j] = M[j][i];
				}
			}
		}

		template<class T>
		void TV::Maths::TMatrix4x4<T>::SetInverse()
		{
			*this = GetInverse();
		}

		template<class T>
		void TV::Maths::TMatrix4x4<T>::SetTranspose()
		{
			*this = GetTranspose();
		}

		template<class T>
		void TV::Maths::TMatrix4x4<T>::SetTranslation(const TVec3<T>& translation)
		{
			M03 = translation.X;
			M13 = translation.Y;
			M23 = translation.Z;
			M00 = M11 = M22 = M33 = 1;
			M01 = M02 = 0;
			M10 = M12 = 0;
			M20 = M21 = 0;
		}

		template<class T>
		void TV::Maths::TMatrix4x4<T>::SetScale(const TVec3<T>& scale)
		{
			M00 = scale.X;
			M11 = scale.Y;
			M22 = scale.Z;
			M33 = 1;
			M01 = M02 = M03 = 0;
			M10 = M12 = M13 = 0;
			M20 = M21 = M23 = 0;
		}

		template<class T>
		void TV::Maths::TMatrix4x4<T>::SetIdentity()
		{
			M00 = M11 = M22 = M33 = 1;
			M01 = M02 = M03 = 0;
			M10 = M12 = M13 = 0;
			M20 = M21 = M23 = 0;
		}

		template<class T>
		TV::Maths::TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::GetInverse() const
		{
			// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

			TMatrix4x4<T> inv;

			inv.M[0][0] = M[1][1] * M[2][2] * M[3][3] -
				M[1][1] * M[2][3] * M[3][2] -
				M[2][1] * M[1][2] * M[3][3] +
				M[2][1] * M[1][3] * M[3][2] +
				M[3][1] * M[1][2] * M[2][3] -
				M[3][1] * M[1][3] * M[2][2];

			inv.M[1][0] = -M[1][0] * M[2][2] * M[3][3] +
				M[1][0] * M[2][3] * M[3][2] +
				M[2][0] * M[1][2] * M[3][3] -
				M[2][0] * M[1][3] * M[3][2] -
				M[3][0] * M[1][2] * M[2][3] +
				M[3][0] * M[1][3] * M[2][2];

			inv.M[2][0] = M[1][0] * M[2][1] * M[3][3] -
				M[1][0] * M[2][3] * M[3][1] -
				M[2][0] * M[1][1] * M[3][3] +
				M[2][0] * M[1][3] * M[3][1] +
				M[3][0] * M[1][1] * M[2][3] -
				M[3][0] * M[1][3] * M[2][1];

			inv.M[3][0] = -M[1][0] * M[2][1] * M[3][2] +
				M[1][0] * M[2][2] * M[3][1] +
				M[2][0] * M[1][1] * M[3][2] -
				M[2][0] * M[1][2] * M[3][1] -
				M[3][0] * M[1][1] * M[2][2] +
				M[3][0] * M[1][2] * M[2][1];

			inv.M[0][1] = -M[0][1] * M[2][2] * M[3][3] +
				M[0][1] * M[2][3] * M[3][2] +
				M[2][1] * M[0][2] * M[3][3] -
				M[2][1] * M[0][3] * M[3][2] -
				M[3][1] * M[0][2] * M[2][3] +
				M[3][1] * M[0][3] * M[2][2];

			inv.M[1][1] = M[0][0] * M[2][2] * M[3][3] -
				M[0][0] * M[2][3] * M[3][2] -
				M[2][0] * M[0][2] * M[3][3] +
				M[2][0] * M[0][3] * M[3][2] +
				M[3][0] * M[0][2] * M[2][3] -
				M[3][0] * M[0][3] * M[2][2];

			inv.M[2][1] = -M[0][0] * M[2][1] * M[3][3] +
				M[0][0] * M[2][3] * M[3][1] +
				M[2][0] * M[0][1] * M[3][3] -
				M[2][0] * M[0][3] * M[3][1] -
				M[3][0] * M[0][1] * M[2][3] +
				M[3][0] * M[0][3] * M[2][1];

			inv.M[3][1] = M[0][0] * M[2][1] * M[3][2] -
				M[0][0] * M[2][2] * M[3][1] -
				M[2][0] * M[0][1] * M[3][2] +
				M[2][0] * M[0][2] * M[3][1] +
				M[3][0] * M[0][1] * M[2][2] -
				M[3][0] * M[0][2] * M[2][1];

			inv.M[0][2] = M[0][1] * M[1][2] * M[3][3] -
				M[0][1] * M[1][3] * M[3][2] -
				M[1][1] * M[0][2] * M[3][3] +
				M[1][1] * M[0][3] * M[3][2] +
				M[3][1] * M[0][2] * M[1][3] -
				M[3][1] * M[0][3] * M[1][2];

			inv.M[1][2] = -M[0][0] * M[1][2] * M[3][3] +
				M[0][0] * M[1][3] * M[3][2] +
				M[1][0] * M[0][2] * M[3][3] -
				M[1][0] * M[0][3] * M[3][2] -
				M[3][0] * M[0][2] * M[1][3] +
				M[3][0] * M[0][3] * M[1][2];

			inv.M[2][2] = M[0][0] * M[1][1] * M[3][3] -
				M[0][0] * M[1][3] * M[3][1] -
				M[1][0] * M[0][1] * M[3][3] +
				M[1][0] * M[0][3] * M[3][1] +
				M[3][0] * M[0][1] * M[1][3] -
				M[3][0] * M[0][3] * M[1][1];

			inv.M[3][2] = -M[0][0] * M[1][1] * M[3][2] +
				M[0][0] * M[1][2] * M[3][1] +
				M[1][0] * M[0][1] * M[3][2] -
				M[1][0] * M[0][2] * M[3][1] -
				M[3][0] * M[0][1] * M[1][2] +
				M[3][0] * M[0][2] * M[1][1];

			inv.M[0][3] = -M[0][1] * M[1][2] * M[2][3] +
				M[0][1] * M[1][3] * M[2][2] +
				M[1][1] * M[0][2] * M[2][3] -
				M[1][1] * M[0][3] * M[2][2] -
				M[2][1] * M[0][2] * M[1][3] +
				M[2][1] * M[0][3] * M[1][2];

			inv.M[1][3] = M[0][0] * M[1][2] * M[2][3] -
				M[0][0] * M[1][3] * M[2][2] -
				M[1][0] * M[0][2] * M[2][3] +
				M[1][0] * M[0][3] * M[2][2] +
				M[2][0] * M[0][2] * M[1][3] -
				M[2][0] * M[0][3] * M[1][2];

			inv.M[2][3] = -M[0][0] * M[1][1] * M[2][3] +
				M[0][0] * M[1][3] * M[2][1] +
				M[1][0] * M[0][1] * M[2][3] -
				M[1][0] * M[0][3] * M[2][1] -
				M[2][0] * M[0][1] * M[1][3] +
				M[2][0] * M[0][3] * M[1][1];

			inv.M[3][3] = M[0][0] * M[1][1] * M[2][2] -
				M[0][0] * M[1][2] * M[2][1] -
				M[1][0] * M[0][1] * M[2][2] +
				M[1][0] * M[0][2] * M[2][1] +
				M[2][0] * M[0][1] * M[1][2] -
				M[2][0] * M[0][2] * M[1][1];

			T det = M[0][0] * inv.M[0][0] + M[0][1] * inv.M[1][0] + M[0][2] * inv.M[2][0] + M[0][3] * inv.M[3][0];

			if (det == 0)
			{
				return TMatrix4x4<T>();
			}

			det = 1.0 / det;

			for (int32 i = 0; i < 16; i++)
			{
				inv.Raw[i] *= det;
			}

			return inv;
		}

		template<class T>
		inline [[nodiscard]] TV::Maths::TMatrix4x4<T> operator * (const TV::Maths::TMatrix4x4<T>& a, const TV::Maths::TMatrix4x4<T>& b)
		{
			TMatrix4x4<T> result;
			for (int32 i = 0; i != 4; ++i)
			{
				for (int32 j = 0; j != 4; ++j)
				{
					result.M[i][j] = 0;
					for (int32 k = 0; k != 4; ++i)
					{
						result.M[i][j] += a.M[i][k] + b.M[k][j];
					}
				}
			}
		}
	}
}

