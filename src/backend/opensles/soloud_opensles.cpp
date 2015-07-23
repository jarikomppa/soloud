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
#include <stdlib.h>
#include <math.h>

#include "soloud.h"
#include "soloud_thread.h"

#if !defined(WITH_OPENSLES)

namespace SoLoud
{
	result opensles_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		return NOT_IMPLEMENTED;
	}
}
#else

#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Platform.h"

#if defined(__ANDROID__ )
#include "SLES/OpenSLES_Android.h"
#endif

#define NUM_BUFFERS 2


static SLObjectItf engineObj = NULL;
static SLEngineItf engine = NULL;
static SLObjectItf outputMixObj = NULL;
static SLVolumeItf outputMixVol = NULL;
static SLDataLocator_OutputMix outLocator;
static SLDataSink dstDataSink;
static unsigned int bufferSize = 4096;
static unsigned int channels = 2;

static volatile int threadrun = 0;
static short *outputBuffer = NULL;

#if defined( __ANDROID__ )
static SLDataLocator_AndroidSimpleBufferQueue inLocator;
#endif

namespace SoLoud
{
	void soloud_opensles_deinit(SoLoud::Soloud *aSoloud)
	{
		threadrun++;
		while (threadrun == 1)
		{
			Thread::sleep(10);
		}

		(*outputMixObj)->Destroy( outputMixObj );
		outputMixObj = NULL;
		outputMixVol = NULL;

		(*engineObj)->Destroy( engineObj );
		engineObj = NULL;
		engine = NULL;

		delete [] outputBuffer;
	}

	static void opensles_iterate(SoLoud::Soloud *aSoloud)
	{
		// TODO: Grab how many are queued from OpenSL ES.
		int buffersQueued = NUM_BUFFERS;

		if( buffersQueued < NUM_BUFFERS )
		{
			aSoloud->mix_s16(outputBuffer,bufferSize);
		}
	}

	static void opensles_thread(void *aParam)
	{
		Soloud *soloud = (Soloud *)aParam;
		while (threadrun == 0)
		{
			opensles_iterate(soloud);
			Thread::sleep(1);
		}
		threadrun++;
	}

	result opensles_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
	{
		// Allocate output buffer to mix into.
		outputBuffer = new short[ aBuffer * aChannels ];
		bufferSize = aBuffer;
		channels = aChannels;

		// Create engine.
		// TODO: Use channels, samplerate, etc..
		slCreateEngine( &engineObj, 0, NULL, 0, NULL, NULL );
		if(!engineObj)
		{
			return UNKNOWN_ERROR;
		}

		// Realize and get engine interfaxce.	
		(*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
		if((*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine) != SL_RESULT_SUCCESS)
		{
			return UNKNOWN_ERROR;
		}

		// Create output mix.
		const SLInterfaceID ids[] = { SL_IID_VOLUME };
		const SLboolean req[] = { SL_BOOLEAN_FALSE };

		(*engine)->CreateOutputMix(engine, &outputMixObj, 1, ids, req);

		(*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);
		if( (*outputMixObj)->GetInterface(outputMixObj, SL_IID_VOLUME, &outputMixVol) != SL_RESULT_SUCCESS )
		{
			return UNKNOWN_ERROR;
		}

		// Create android buffer queue.
#if defined( __ANDROID__ )
		inLocator.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
		inLocator.numBuffers = NUM_BUFFERS;
#endif

		// Setup data format.
		SLDataFormat_PCM format;
		format.formatType = SL_DATAFORMAT_PCM;
		format.numChannels = aChannels;
		format.samplesPerSec = aSamplerate * 1000; //mHz
		format.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
		format.containerSize = 16;
		format.channelMask = SL_SPEAKER_FRONT_CENTER;
		format.endianness = SL_BYTEORDER_LITTLEENDIAN;
		 
		SLDataSource src;
		src.pLocator = &inLocator;
		src.pFormat = &format;

		// Output mix.
		outLocator.locatorType = SL_DATALOCATOR_OUTPUTMIX;
		outLocator.outputMix = outputMixObj;
		 
		dstDataSink.pLocator = &outLocator;
		dstDataSink.pFormat = NULL;

		//
		aSoloud->postinit(aSamplerate,aBuffer,aFlags,2);
		aSoloud->mBackendCleanupFunc = soloud_opensles_deinit;

		Thread::createThread(opensles_thread, (void*)aSoloud);

        aSoloud->mBackendString = "OpenSLES";
		return 0;
	}	
};
#endif
