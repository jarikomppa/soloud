/*
MONOTONE module for SoLoud audio engine
Copyright (c) 2015 Jari Komppa

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

#ifndef MONOTONE_H
#define MONOTONE_H

#include "soloud.h"

namespace SoLoud
{
	class Monotone;

	struct MonotoneSong
	{
		char *mTitle;
		char *mComment;
		unsigned char mVersion; // must be 1
		unsigned char mTotalPatterns;
		unsigned char mTotalTracks;
		unsigned char mCellSize; // must be 2 for version 1
		unsigned char mOrder[256];
		unsigned int *mPatternData; // 64 rows * mTotalPatterns * mTotalTracks
	};

	struct MonotoneChannel
	{
		int mEnabled; 
		int mActive;
		int mFreq[3];
		int mPortamento;
	};

	class MonotoneInstance : public AudioSourceInstance
	{
		Monotone *mParent;		
	public:
		MonotoneChannel mChannel[12];
		int mTempo; // ticks / row. Tick = 60hz. Default 4.
		int mOrder;
		int mRow;
		int mSampleCount;
		int mPeriodInSamples;
		int mTick;

		MonotoneInstance(Monotone *aParent);
		virtual void getAudio(float *aBuffer, unsigned int aSamples);
		virtual bool hasEnded();
	};

	class Monotone : public AudioSource
	{
	public:
		int mNotesHz[800];
		int mHardwareChannels;
		MonotoneSong mSong;
		void clear();
		Monotone();
		~Monotone();
		result load(const char *aFilename, int aHardwareChannels = 1);
		virtual AudioSourceInstance *createInstance();
	};
};

#endif