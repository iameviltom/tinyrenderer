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
			Colour(uint32 inPacked) : PackedData(inPacked) {}
			Colour(const uint8* bytes, int numBytes)
			{
				PackedData = 0;
				for (int index = 0; index != std::min(numBytes, 4); ++index)
				{
					Raw[index] = bytes[index];
				}
			}
			Colour(uint8 r, uint8 g, uint8 b, uint8 a = 255) : B(b), G(g), R(r), A(a) {}

			[[nodiscard]] Colour Scaled(double scale, bool bScaleAlpha = false) const
			{
				Colour newColour(*this);
				for (int32 index = 0; index != (bScaleAlpha ? 4 : 3); ++index)
				{
					double val = Clamp(Raw[index] * scale, 0.0, 255.0);
					newColour.Raw[index] = (uint8)val;
				}
				return newColour;
			}

			static [[nodiscard]] Colour MakeRandomColour()
			{
				return Colour(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
			}
		};
	}
}

