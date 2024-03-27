#pragma once

#include <xutility>
#include "Types.h"
#include "Maths.h"

namespace TV
{
	namespace Maths
	{
		class Colour
		{
		public:
			union
			{
				struct
				{
					uint8 B, G, R, A;
				};
				uint8 Raw[4];
				uint32 PackedData;
			};

			Colour() : PackedData(0) {}
			explicit Colour(uint32 inPacked) : PackedData(inPacked) {}
			Colour(const uint8* bytes, int numBytes)
			{
				PackedData = 0;
				for (int index = 0; index != std::min(numBytes, 4); ++index)
				{
					Raw[index] = bytes[index];
				}
			}
			constexpr Colour(uint8 r, uint8 g, uint8 b, uint8 a = 255) : B(b), G(g), R(r), A(a) {}

			[[nodiscard]] uint32 GetRGBA() const
			{
				const uint32 packed = (uint32)(R << 24) | (uint32)(G << 16) | (uint32)(B << 8) | (uint32)A;
				return packed;
			}
			[[nodiscard]] uint32 GetARGB() const
			{
				const uint32 packed = (uint32)(A << 24) | (uint32)(R << 16) | (uint32)(G << 8) | (uint32)B;
				return packed;
			}

			[[nodiscard]] Colour Scaled(double scale, bool bScaleAlpha = false) const
			{
				Colour newColour(*this);
				for (int32 index = 0; index != (bScaleAlpha ? 4 : 3); ++index)
				{
					double val = GetClamped(Raw[index] * scale, 0.0, 255.0);
					newColour.Raw[index] = (uint8)val;
				}
				return newColour;
			}

			static [[nodiscard]] Colour MakeRandomColour()
			{
				return Colour(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
			}

			static [[nodiscard]] Colour MakeFromRGBA(uint32 rgba)
			{
				const uint32 mask = 0xFF;
				const Colour newColour((rgba >> 24) & mask, (rgba >> 16) & mask, (rgba >> 8) & mask, rgba & mask);
				return newColour;
			}

			static [[nodiscard]] Colour MakeFromARGB(uint32 rgba)
			{
				const uint32 mask = 0xFF;
				const Colour newColour((rgba >> 16) & mask, (rgba >> 8) & mask, rgba & mask, (rgba >> 24) & mask);
				return newColour;
			}
		};
	}
}

