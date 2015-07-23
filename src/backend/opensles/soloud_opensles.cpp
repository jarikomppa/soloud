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

#if defined( __ANDROID__ )
#include "SLES/OpenSLES_Android.h"
#endif

// Error logging.
#if defined( __ANDROID__ ) 
#  include <android/log.h>
#  define LOG_ERROR( _msg ) \
   __android_log_print( ANDROID_LOG_ERROR, "SoLoud", _msg )

#else
   printf( _msg )
#endif

#define NUM_BUFFERS 2

// Engine.
static SLObjectItf engineObj = NULL;
static SLEngineItf engine = NULL;

// Output mix.
static SLObjectItf outputMixObj = NULL;
static SLVolumeItf outputMixVol = NULL;

// Data.
static SLDataLocator_OutputMix outLocator;
static SLDataSink dstDataSink;

// Player.
static SLObjectItf playerObj = NULL;
static SLPlayItf player = NULL;
static SLVolumeItf playerVol = NULL;
#if defined( __ANDROID__ )
static SLAndroidSimpleBufferQueueItf playerBufferQueue = NULL;
#endif

static unsigned int bufferSize = 4096;
static unsigned int channels = 2;
static short *outputBuffer = NULL;
int buffersQueued = 0;

static volatile int threadrun = 0;

#if defined( __ANDROID__ )
static SLDataLocator_AndroidSimpleBufferQueue inLocator;
#endif

namespace SoLoud
{
	static void SLAPIENTRY soloud_opensles_play_callback( SLPlayItf player, void *context, SLuint32 event )
	{
		//TODO: SL_PLAYEVENT_HEADATMARKER so we get a callback mid track.
		if( event & SL_PLAYEVENT_HEADATEND )
		{
			--buffersQueued;
		}
	}
 
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
		// If we haven't got atleast NUM_BUFFERS queued, make sure we have another one waiting.
		if( buffersQueued < NUM_BUFFERS )
		{
			aSoloud->mix_s16(outputBuffer,bufferSize);
#if defined( __ANDROID__ )
			const int bufferSizeBytes = bufferSize * channels * sizeof(short);
			(*playerBufferQueue)->Enqueue( playerBufferQueue, outputBuffer, bufferSizeBytes );
#endif
			++buffersQueued;
		}
	}

	static void opensles_thread(void *aParam)
	{
		Soloud *soloud = (Soloud *)aParam;
		while (threadrun == 0)
		{
			opensles_iterate(soloud);

			// TODO: Condition var?
			Thread::sleep(1);
		}
		threadrun++;
	}

	result opensles_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
	{
		// Allocate output buffer to mix into.
		bufferSize = aBuffer;
		channels = aChannels;
		outputBuffer = new short[ bufferSize * channels ];

		// Create engine.
		// TODO: Use channels, samplerate, etc..
		if(slCreateEngine( &engineObj, 0, NULL, 0, NULL, NULL ) != SL_RESULT_SUCCESS)
		{
			LOG_ERROR( "Failed to create engine." );
			return UNKNOWN_ERROR;
		}

		// Realize and get engine interfaxce.	
		(*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
		if((*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine) != SL_RESULT_SUCCESS)
		{
			LOG_ERROR( "Failed to get engine interface." );
			return UNKNOWN_ERROR;
		}

		// Create output mix.
		const SLInterfaceID ids[] = { SL_IID_VOLUME };
		const SLboolean req[] = { SL_BOOLEAN_FALSE };

		if((*engine)->CreateOutputMix(engine, &outputMixObj, 1, ids, req) != SL_RESULT_SUCCESS)
		{
			LOG_ERROR( "Failed to create output mix object." );
			return UNKNOWN_ERROR;
		}
		(*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);

		if((*outputMixObj)->GetInterface(outputMixObj, SL_IID_VOLUME, &outputMixVol) != SL_RESULT_SUCCESS)
		{
			LOG_ERROR( "Failed to get output mix volume interface." );
		}


		// Create android buffer queue.
#if defined( __ANDROID__ )
		inLocator.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
		inLocator.numBuffers = NUM_BUFFERS;
#endif

		// Setup data format.
		SLDataFormat_PCM format;
		format.formatType = SL_DATAFORMAT_PCM;
		format.numChannels = channels;
		format.samplesPerSec = aSamplerate * 1000; //mHz
		format.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
		format.containerSize = 16;
		format.endianness = SL_BYTEORDER_LITTLEENDIAN;

		// Determine channel mask.
		if(channels == 2)
		{
			format.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		}
		else
		{
			format.channelMask = SL_SPEAKER_FRONT_CENTER;
		}
		 
		SLDataSource src;
		src.pLocator = &inLocator;
		src.pFormat = &format;

		// Output mix.
		outLocator.locatorType = SL_DATALOCATOR_OUTPUTMIX;
		outLocator.outputMix = outputMixObj;
		 
		dstDataSink.pLocator = &outLocator;
		dstDataSink.pFormat = NULL;

		// Setup player.
		{
#if defined( __ANDROID__ )
			const SLInterfaceID ids[] = { SL_IID_VOLUME, SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
			const SLboolean req[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
#else
			const SLInterfaceID ids[] = { SL_IID_VOLUME };
			const SLboolean req[] = { SL_BOOLEAN_TRUE };
#endif
 
			(*engine)->CreateAudioPlayer( engine, &playerObj, &src, &dstDataSink, sizeof( ids ) / sizeof( ids[0] ), ids, req );
		 
			(*playerObj)->Realize( playerObj, SL_BOOLEAN_FALSE );
	 
			(*playerObj)->GetInterface( playerObj, SL_IID_PLAY, &player );
			(*playerObj)->GetInterface( playerObj, SL_IID_VOLUME, &playerVol );
 
#if defined( __ANDROID__ )
			(*playerObj)->GetInterface( playerObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &playerBufferQueue );
#endif
		}

		// Begin playing.
		{
			const int bufferSizeBytes = bufferSize * channels * sizeof(short);
			memset(outputBuffer, 0, bufferSizeBytes);
#if defined( __ANDROID__ )
			(*playerBufferQueue)->Enqueue( playerBufferQueue, outputBuffer, bufferSizeBytes );
#endif

			(*player)->RegisterCallback( player, soloud_opensles_play_callback, aSoloud );
			(*player)->SetCallbackEventsMask( player, SL_PLAYEVENT_HEADATEND );

			(*player)->SetPlayState( player, SL_PLAYSTATE_PLAYING );
		}
		
		//
		aSoloud->postinit(aSamplerate,bufferSize,aFlags,2);
		aSoloud->mBackendCleanupFunc = soloud_opensles_deinit;

		LOG_ERROR( "Creating audio thread." );
		Thread::createThread(opensles_thread, (void*)aSoloud);

		aSoloud->mBackendString = "OpenSLES";
		return SO_NO_ERROR;
	}	
};
#endif
