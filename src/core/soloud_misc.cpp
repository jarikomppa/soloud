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
			}
		}
	}
};
