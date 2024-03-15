#pragma once

#include "Types.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Assert.h"
#include <numbers>

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
				// indices are row then column
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

			[[nodiscard]] TVec3<T> GetAxisX() const { return TVec3<T>(M00, M01, M02); }
			[[nodiscard]] TVec3<T> GetAxisY() const { return TVec3<T>(M10, M11, M12); }
			[[nodiscard]] TVec3<T> GetAxisZ() const { return TVec3<T>(M20, M21, M22); }
			[[nodiscard]] TVec3<T> GetTranslation() const { return TVec3<T>(M30, M31, M32); }

			TMatrix4x4<T>& operator *= (const TMatrix4x4<T>& other);

			[[nodiscard]] TVec4<T> TransformVector4(const TVec4<T>& vector) const;
			[[nodiscard]] TVec3<T> TransformVector(const TVec3<T>& vector) const;
			[[nodiscard]] TVec3<T> TransformPosition(const TVec3<T>& vector) const;

			static [[nodiscard]] TMatrix4x4<T> MakeScale(const TVec3<T>& scale);
			static [[nodiscard]] TMatrix4x4<T> MakeTranslation(const TVec3<T>& translation);
			static [[nodiscard]] TMatrix4x4<T> MakePerspectiveProjection(T verticalFieldOfViewRadians, T aspectRatio, T nearClip, T farClip);
			static [[nodiscard]] TMatrix4x4<T> MakeOrthographicProjection(T orthographicWidth, T aspectRatio, T nearClip, T farClip);
			static [[nodiscard]] TMatrix4x4<T> MakeLookAt(const TVec3<T>& lookOrigin, const TVec3<T>& lookTarget, const TVec3<T>& upVector);
		};

		template<class T>
		[[nodiscard]] TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::MakeLookAt(const TVec3<T>& lookOrigin, const TVec3<T>& lookTarget, const TVec3<T>& upVector)
		{
			const TVec3<T> z = (lookOrigin - lookTarget).GetSafeNormal();
			const TVec3<T> x = GetCrossProduct(upVector, z).GetSafeNormal();
			const TVec3<T> y = GetCrossProduct(z, x).GetSafeNormal();

			TMatrix4x4<T> mtx(MakeTranslation(lookOrigin));
			for (int32 i = 0; i != 3; ++i)
			{
				mtx.M[i][0] = x.Raw[i];
				mtx.M[i][1] = y.Raw[i];
				mtx.M[i][2] = z.Raw[i];
			}

			return mtx;
		}

		template<class T>
		[[nodiscard]] TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::MakeOrthographicProjection(T orthographicWidth, T aspectRatio, T nearClip, T farClip)
		{
			// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix.html

			check(farClip > nearClip);
			check(orthographicWidth > 0);
			check(aspectRatio > 0);

			const T clipRange = farClip - nearClip;

			TMatrix4x4<T> ret;

			ret.M00 = 2.f / orthographicWidth;
			ret.M11 = ret.M00 * aspectRatio;
			ret.M22 = -2.f / clipRange; // mapping z to [-1,1]
			ret.M23 = (-1.f * farClip + nearClip) / clipRange;
			return ret;
		}

		template<class T>
		[[nodiscard]] TMatrix4x4<T> TV::Maths::TMatrix4x4<T>::MakePerspectiveProjection(T verticalFieldOfViewRadians, T aspectRatio, T nearClip, T farClip)
		{
			check(farClip > nearClip);
			check(verticalFieldOfViewRadians > 0);
			check(verticalFieldOfViewRadians < std::numbers::pi);
			check(aspectRatio > 0);

			const T clipRange = farClip - nearClip;
			const T tanHalfFov = (T)GetTanRadians(verticalFieldOfViewRadians * 0.5);

			TMatrix4x4<T> ret;

			// to account for fov
			ret.M00 = (T)(1.0 / tanHalfFov);
			ret.M11 = ret.M00 * aspectRatio;

			// to create perspective division (i.e. to propagate division by z)
			ret.M32 = -1.f;

			// negative to invert z, because camera looks down negative z axis.
			// Value is to map z to range [-1,1] i.e. normalised depth
			ret.M22 = (-1.f * farClip + nearClip) / clipRange;
			ret.M23 = (-2.f * nearClip * farClip) / clipRange; // to map z to range [0,1] i.e. normalised depth
			return ret;
		}

		template<class T>
		TV::Maths::TVec4<T> TV::Maths::TMatrix4x4<T>::TransformVector4(const TVec4<T>& vector) const
		{
			TVec4<T> ret;
			ret.Raw[0] = vector.Raw[0] * M[0][0] + vector.Raw[1] * M[0][1] + vector.Raw[2] * M[0][2] + vector.Raw[3] * M[0][3];
			ret.Raw[1] = vector.Raw[0] * M[1][0] + vector.Raw[1] * M[1][1] + vector.Raw[2] * M[1][2] + vector.Raw[3] * M[1][3];
			ret.Raw[2] = vector.Raw[0] * M[2][0] + vector.Raw[1] * M[2][1] + vector.Raw[2] * M[2][2] + vector.Raw[3] * M[2][3];
			ret.Raw[3] = vector.Raw[0] * M[3][0] + vector.Raw[1] * M[3][1] + vector.Raw[2] * M[3][2] + vector.Raw[3] * M[3][3];
			return ret;
		}

		template<class T>
		TV::Maths::TVec3<T> TV::Maths::TMatrix4x4<T>::TransformPosition(const TVec3<T>& vector) const
		{
			return TransformVector4(TVec4<T>(vector, 1)).GetProjected();
		}

		template<class T>
		TV::Maths::TVec3<T> TV::Maths::TMatrix4x4<T>::TransformVector(const TVec3<T>& vector) const
		{
			return TransformVector4(TVec4<T>(vector, 0)).GetUnprojected();
		}

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
			*this = result;
			return *this;
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
			M30 = M31 = M32 = 0;
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
			M30 = M31 = M32 = 0;
		}

		template<class T>
		void TV::Maths::TMatrix4x4<T>::SetIdentity()
		{
			M00 = M11 = M22 = M33 = 1;
			M01 = M02 = M03 = 0;
			M10 = M12 = M13 = 0;
			M20 = M21 = M23 = 0;
			M30 = M31 = M32 = 0;
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

			det = (T)1.0 / det;

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
					for (int32 k = 0; k != 4; ++k)
					{
						result.M[i][j] += a.M[i][k] * b.M[k][j];
					}
				}
			}
			return result;
		}
	}
}

