/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

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

#ifndef SOLOUD_BUS_H
#define SOLOUD_BUS_H

#include "soloud.h"

namespace SoLoud
{
	class Bus;

	class BusInstance : public AudioSourceInstance
	{
		Bus *mParent;
		int mScratchSize;
		float *mScratch;
	public:
		BusInstance(Bus *aParent);
		virtual void getAudio(float *aBuffer, int aSamples);
		virtual int hasEnded();
		virtual ~BusInstance();
	};

	class Bus : public AudioSource
	{
	public:
		BusInstance *mInstance;
		int mChannelHandle;
	public:
		Bus();
		virtual BusInstance *createInstance();
		// Set filter. Set to NULL to clear the filter.
		virtual void setFilter(int aFilterId, Filter *aFilter);
		// Play sound through the bus
		int play(AudioSource &aSound, float aVolume = 1.0f, float aPan = 0.0f, int aPaused = 0);
	};
};

#endif