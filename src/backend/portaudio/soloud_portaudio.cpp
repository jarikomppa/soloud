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
#include <stdlib.h>
#include <math.h>

#include "soloud.h"
#include "soloud_thread.h"

#if !defined(WITH_PORTAUDIO)

namespace SoLoud
{
	result portaudio_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		return NOT_IMPLEMENTED;
	}
}
#else

#include "portaudio.h"


extern "C"
{
	int dll_Pa_found();
	PaError dll_Pa_Initialize( void );
	PaError dll_Pa_Terminate( void );
	PaError dll_Pa_CloseStream( PaStream *stream );
	PaError dll_Pa_StartStream( PaStream *stream );
	PaError dll_Pa_OpenDefaultStream( PaStream** stream,
                              int numInputChannels,
                              int numOutputChannels,
                              PaSampleFormat sampleFormat,
                              double sampleRate,
                              unsigned long framesPerBuffer,
                              PaStreamCallback *streamCallback,
                              void *userData );
};



namespace SoLoud
{
	static PaStream *gStream;

	static int portaudio_callback( 
				const void *input,
				void *output,
				unsigned long frameCount,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData ) 
	{
		SoLoud::Soloud *soloud = (SoLoud::Soloud *)userData;
		float *mixdata = (float*)(soloud->mBackendData);
		soloud->mix((float*)output, frameCount);

		return 0;
	}

	static void portaudio_mutex_lock(void * mutex)
	{
		Thread::lockMutex(mutex);
	}

	static void portaudio_mutex_unlock(void * mutex)
	{
		Thread::unlockMutex(mutex);
	}

	void soloud_portaudio_deinit(SoLoud::Soloud *aSoloud)
	{
		dll_Pa_CloseStream(gStream);
		dll_Pa_Terminate();
	}

	result portaudio_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
	{
		if (!dll_Pa_found())
			return DLL_NOT_FOUND;

		aSoloud->postinit(aSamplerate, aBuffer * 2, aFlags, 2);
		aSoloud->mBackendCleanupFunc = soloud_portaudio_deinit;
		dll_Pa_Initialize();
		dll_Pa_OpenDefaultStream(&gStream, 0, 2, paFloat32, aSamplerate, paFramesPerBufferUnspecified, portaudio_callback, (void*)aSoloud);
		dll_Pa_StartStream(gStream);
        aSoloud->mBackendString = "PortAudio";
		return 0;
	}
	
};
#endif