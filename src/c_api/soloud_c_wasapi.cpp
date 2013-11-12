/* **************************************************
 *  WARNING: this is a generated file. Do not edit. *
 *  Any edits will be overwritten by the generator. *
 ************************************************** */

/*
SoLoud audio engine
Copyright (c) 2013 Jari Komppa

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

/* SoLoud C-Api Code Generator (c)2013 Jari Komppa http://iki.fi/sol/ */

#include "../include/soloud.h"
#include "../include/soloud_audiosource.h"
#include "../include/soloud_biquadresonantfilter.h"
#include "../include/soloud_bus.h"
#include "../include/soloud_echofilter.h"
#include "../include/soloud_fader.h"
#include "../include/soloud_fftfilter.h"
#include "../include/soloud_filter.h"
#include "../include/soloud_speech.h"
#include "../include/soloud_thread.h"
#include "../include/soloud_wav.h"
#include "../include/soloud_wavstream.h"

using namespace SoLoud;

extern "C"
{

int Soloud_wasapi_init(Soloud * aSoloud)
{
	return wasapi_init(aSoloud);
}

int Soloud_wasapi_initEx(Soloud * aSoloud, int aFlags, int aSamplerate, int aBuffer)
{
	return wasapi_init(aSoloud, aFlags, aSamplerate, aBuffer);
}

} // extern "C"

