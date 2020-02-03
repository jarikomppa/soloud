/*
SoLoud audio engine
Copyright (c) 2020 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include "soloud_misc.h"
#include <math.h>

namespace SoLoud
{
	namespace Misc
	{
		float generateWaveform(int aWaveform, float p)
		{
			switch (aWaveform)
			{
			default:
			case WAVE_SQUARE:
				return p > 0.5f ? 0.5f : -0.5f;
			case WAVE_SAW:
				return p - 0.5f;
			case WAVE_SIN:
				return (float)sin(p * M_PI * 2.0f) * 0.5f;
			case WAVE_TRIANGLE:
				return (p > 0.5f ? (1.0f - (p - 0.5f) * 2) : p * 2.0f) - 0.5f;
			case WAVE_BOUNCE:
				return (p < 0.5f ? (float)sin(p * M_PI * 2.0f) * 0.5f : -(float)sin(p * M_PI * 2.0f) * 0.5f) - 0.5f;
			case WAVE_JAWS:
				return (p < 0.25f ? (float)sin(p * M_PI * 2.0f) * 0.5f : 0) - 0.5f;
			case WAVE_HUMPS:
				return (p < 0.5f ? (float)sin(p * M_PI * 2.0f) * 0.5f : 0) - 0.5f;
			case WAVE_FSQUARE:
				{
					float f = 0;
					for (int i = 1; i < 22; i += 2)
					{
						f += (4.0f / (M_PI * i)) * (float)sin(2 * M_PI * i * p);
					}
					return f * 0.5f;
				}
			case WAVE_FSAW:
				{
					float f = 0;
					for (int i = 1; i < 15; i++)
					{
						if (i & 1)
							f += (1.0f / (M_PI * i)) * sin(p * 2 * M_PI * i);
						else
							f -= (1.0f / (M_PI * i)) * sin(p * 2 * M_PI * i);
					}
					return f;
				}
			}
		}
	}
};
