/*
libmodplug module for SoLoud audio engine
Copyright (c) 2014 Jari Komppa

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

#include <stdlib.h>
#include <stdio.h>
#include "soloud_modplug.h"
#ifdef WITH_MODPLUG
#include "../ext/libmodplug/src/modplug.h"
#endif
#include "soloud_file.h"

namespace SoLoud
{

	ModplugInstance::ModplugInstance(Modplug *aParent)
	{
#ifdef WITH_MODPLUG
		mParent = aParent;
		ModPlugFile* mpf = ModPlug_Load((const void*)mParent->mData, mParent->mDataLen);
		mModplugfile = (void*)mpf;
		mPlaying = mpf != NULL;		
#endif
	}

	void ModplugInstance::getAudio(float *aBuffer, unsigned int aSamples)
	{
#ifdef WITH_MODPLUG
		if (mModplugfile == NULL)
			return;
		int buf[1024];
		int s = aSamples;
		unsigned int outofs = 0;
		
		while (s && mPlaying)
		{
			int samples = 512;
			if (s < samples) s = samples;
			int res = ModPlug_Read((ModPlugFile *)mModplugfile, (void*)&buf[0], sizeof(int) * 2 * samples);
			int samples_in_buffer = res / (sizeof(int) * 2);
			if (s != samples_in_buffer) mPlaying = 0;

			int i;
			for (i = 0; i < samples_in_buffer; i++)
			{
				aBuffer[outofs] = buf[i*2+0] / (float)0x7fffffff;
				aBuffer[outofs + aSamples] = buf[i*2+1] / (float)0x7fffffff;
				outofs++;
			}
			s -= samples_in_buffer;
		}

		if (outofs < aSamples)
		{
			// TODO: handle looping
			unsigned int i;
			for (i = outofs; i < aSamples; i++)
				aBuffer[i] = aBuffer[i + aSamples] = 0;
		}
#endif		
	}

	bool ModplugInstance::hasEnded()
	{
#ifdef WITH_MODPLUG
		return !mPlaying;
#else
		return 1;
#endif
	}

	ModplugInstance::~ModplugInstance()
	{
#ifdef WITH_MODPLUG
		if (mModplugfile)
		{
			ModPlug_Unload((ModPlugFile*)mModplugfile);
		}
		mModplugfile = 0;
#endif
	}

	result Modplug::loadMem(unsigned char *aMem, unsigned int aLength, bool aCopy, bool aTakeOwnership)
	{
		MemoryFile mf;
		int res = mf.openMem(aMem, aLength, aCopy, aTakeOwnership);
		if (res != SO_NO_ERROR)
			return res;
		return loadFile(&mf);
	}

	result Modplug::load(const char *aFilename)
	{
		DiskFile df;
		int res = df.open(aFilename);
		if (res != SO_NO_ERROR)
			return res;
		return loadFile(&df);
	}

	result Modplug::loadFile(File *aFile)
	{
#ifdef WITH_MODPLUG
		
		if (mData)
		{
			delete[] mData;
		}

		mDataLen = aFile->length();
		mData = new char[mDataLen];
		if (!mData)
		{
			mData = 0;
			mDataLen = 0;
			return OUT_OF_MEMORY;
		}
		aFile->read((unsigned char*)mData, mDataLen);

		ModPlugFile* mpf = ModPlug_Load((const void*)mData, mDataLen);
		if (!mpf)
		{
			delete[] mData;
			mDataLen = 0;
			return FILE_LOAD_FAILED;
		}
		ModPlug_Unload(mpf);
		return 0;
#else
		return NOT_IMPLEMENTED;
#endif
	}

	Modplug::Modplug()
	{
#ifdef WITH_MODPLUG
		mBaseSamplerate = 44100;
		mChannels = 2;
		mData = 0;
		mDataLen = 0;

		ModPlug_Settings mps;
		ModPlug_GetSettings(&mps);
		mps.mChannels = 2;
		mps.mBits = 32;
		mps.mFrequency = 44100;
		mps.mResamplingMode = MODPLUG_RESAMPLE_LINEAR;
		mps.mStereoSeparation = 128;
		mps.mMaxMixChannels = 64;
		mps.mLoopCount = -1;
		mps.mFlags = MODPLUG_ENABLE_OVERSAMPLING;
		ModPlug_SetSettings(&mps);
#endif
	}

	Modplug::~Modplug()
	{
		stop();
#ifdef WITH_MODPLUG
		delete[] mData;
		mData = 0;
		mDataLen = 0;
#endif
	}

	AudioSourceInstance * Modplug::createInstance() 
	{
		return new ModplugInstance(this);
	}

};