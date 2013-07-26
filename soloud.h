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

#ifndef SOLOUD_H
#define SOLOUD_H

#include <stdlib.h> // rand
#include <math.h> // sin

#ifndef M_PI
#define M_PI 3.14159265359
#endif


namespace SoLoud
{

	class AudioProducer
	{
	public:
		enum FLAGS
		{
			PROTECTED = 1,
			LOOPING = 2
		};
		virtual ~AudioProducer() {};
		unsigned int mPlayIndex;
		int mFlags;
		float mLVolume;
		float mRVolume;
		float mVolume;
		float mBaseSamplerate;
		float mSamplerate;
		float mRelativePlaySpeed;
		void init(int aPlayIndex, float aBaseSamplerate, int aFactoryFlags);
		virtual void getAudio(float *aBuffer, int aSamples) = 0;
		virtual int hasEnded() = 0;
	};

	class AudioFactory
	{
	public:
		enum FLAGS
		{
			SHOULD_LOOP = 1
		};
		int mFlags;
		float mBaseSamplerate;
		AudioFactory() { mFlags = 0; mBaseSamplerate = 44100; }
		void setLooping(int aLoop);
		virtual ~AudioFactory() {};
		virtual AudioProducer *createProducer() = 0;
	};


	class Soloud
	{
		float *mScratch;
		int mScratchSize;
		int mScratchNeeded;
		AudioProducer **mChannel;
		int mChannelCount;
		int mSamplerate;
		int mBufferSize;
		int mFlags;
		float mGlobalVolume;
		float mPostClipScaler;
		unsigned int mPlayIndex;
	public:
		void * mMixerData;

		Soloud();
		~Soloud();

		enum FLAGS
		{
			CLIP_ROUNDOFF = 1
		};

		void init(int aChannels, int aSamplerate, int aBufferSize, int aFlags);
		void setVolume(float aVolume);
		int findFreeChannel();
		int play(AudioFactory &aSound, float aVolume, float aPan);
		int getAbsoluteChannelFromHandle(int aChannel);
		float getVolume(int aChannel);
		float getSamplerate(int aChannel);
		int getProtectChannel(int aChannel);
		int getActiveVoices();
		int isValidChannelHandle(int aChannel);
		float getPostClipScaler();
		float getRelativePlaySpeed(int aChannel);
		void setRelativePlaySpeed(int aChannel, float aSpeed);
		void setPostClipScaler(float aScaler);
		void setProtectChannel(int aChannel, int aProtect);
		void setSamplerate(int aChannel, float aSamplerate);
		void setPan(int aChannel, float aPan);
		void setPanAbsolute(int aChannel, float aLVolume, float aRVolume);
		void setVolume(int aChannel, float aVolume);
		void stop(int aChannel);
		void stopAbsolute(int aAbsoluteChannel);
		void stopAll();
		void mix(float *aBuffer, int aSamples);
	};

	int sdl_init(SoLoud::Soloud *aSoloud);
	void sdl_deinit(SoLoud::Soloud *aSoloud);
};

#endif 