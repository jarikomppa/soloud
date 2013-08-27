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
#include <stdlib.h>
#include <math.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "soloud.h"

#define NUM_BUFFERS 4
#define BUFFER_SIZE 4096

static ALCdevice* device = NULL;
static ALCcontext* context = NULL;
static ALenum format = 0;
static ALuint source = 0;
static int frequency = 0;

namespace SoLoud
{
	void soloud_openal_deinit(SoLoud::Soloud *aSoloud)
	{
	}
	

	int openal_init(SoLoud::Soloud *aSoloud, int aChannels, int aFlags, int aSamplerate, int aBuffer)
	{
		aSoloud->init(2,aSamplerate,aBuffer,aFlags);
		aSoloud->mBackendCleanupFunc = soloud_openal_deinit;

		device = alcOpenDevice(NULL);
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
		format = AL_FORMAT_STEREO16;
		ALuint buffers[NUM_BUFFERS];
		alGenBuffers(NUM_BUFFERS, buffers);
		alGenSources(1, &source);

		frequency = aSamplerate;

		int i;
		short data[BUFFER_SIZE * 2];
		for (i = 0; i < BUFFER_SIZE*2; i++)
			data[i] = 0;
		for (i = 0; i < NUM_BUFFERS; i++)
		{
			alBufferData(buffers[i], format, data, BUFFER_SIZE, frequency);
			alSourceQueueBuffers(source, 1, &buffers[i]);
		}

		alSourcePlay(source);

		return 0;
	}

	void soloud_openal_iterate(SoLoud::Soloud *aSoloud)
	{
		ALuint buffer = 0;
		ALint buffersProcessed = 0;
		ALint state;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

		float mixbuf[BUFFER_SIZE*2];
		short downbuf[BUFFER_SIZE*2];

		while (buffersProcessed--) 
		{
			aSoloud->mix(mixbuf,BUFFER_SIZE);
			int i;
			for (i = 0; i < BUFFER_SIZE*2; i++)
				downbuf[i] = mixbuf[i] * 0x7fff;

			alSourceUnqueueBuffers(source, 1, &buffer);

			alBufferData(buffer, format, downbuf, BUFFER_SIZE*4, frequency);

			alSourceQueueBuffers(source, 1, &buffer);
		}

		int val;
		alGetSourcei(source, AL_SOURCE_STATE, &val);
		if(val != AL_PLAYING)
			alSourcePlay(source);
	}
	
};
