/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

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

#ifndef PADSYNTH_H
#define PADSYNTH_H

#include "soloud_wav.h"

namespace SoLoud
{
	result generatePadsynth(
		SoLoud::Wav &aTarget,
		unsigned int aHarmonicCount,
		float *aHarmonics,
		float aBandwidth = 0.25f,
		float aBandwidthScale = 1.0f,
		float aPrincipalFreq = 440.0f,
		float aSampleRate = 44100.0f,
		int aSizePow = 18);
}
#endif