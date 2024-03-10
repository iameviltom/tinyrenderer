#pragma once

#include <xutility>
#include "Types.h"

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
			Colour(uint8 r, uint8 g, uint8 b, uint8 a) : B(b), G(g), R(r), A(a) {}
		};
	}
}

