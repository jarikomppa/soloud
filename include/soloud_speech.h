/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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
#ifndef SOLOUD_SPEECH_H
#define SOLOUD_SPEECH_H

#include "soloud.h"
#include "../src/audiosource/speech/darray.h"
#include "../src/audiosource/speech/klatt.h"
#include "../src/audiosource/speech/tts.h"

namespace SoLoud
{
	class Speech;

	class Speech : public AudioSource
	{
	public:
		int mFrames;
		darray mElement;
		Speech();
		result setText(const char *aText);
		virtual ~Speech();
		virtual AudioSourceInstance *createInstance();
	};

	class SpeechInstance : public AudioSourceInstance
	{
		klatt mSynth;
		Speech *mParent;
		short *mSample;
		int mSampleCount;
		int mOffset;
	public:
		SpeechInstance(Speech *aParent);
        virtual ~SpeechInstance();
		virtual void getAudio(float *aBuffer, unsigned int aSamples);
		virtual result rewind();
		virtual bool hasEnded();
	};
};

#endif
