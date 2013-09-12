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

// Maximum number of filters per stream
#define FILTERS_PER_STREAM 4
//#define SOLOUD_INCLUDE_FFT

#ifdef SOLOUD_INCLUDE_FFT
#include "soloud_fft.h"
#endif

#include "soloud_filter.h"
#include "soloud_fader.h"
#include "soloud_audiosource.h"
#include "soloud_bus.h"

namespace SoLoud
{
	class Soloud;

	typedef void (*mutexCallFunction)(void *aMutexPtr);
	typedef void (*soloudCallFunction)(Soloud *aSoloud);


	// Soloud core class.
	class Soloud
	{
	public:
		// Back-end data; content is up to the back-end implementation.
		void * mBackendData;
		// Pointer for the mutex, usable by the back-end.
		void * mMutex;
		// Mutex lock for thread safety. Set by the back-end. If NULL, not called.
		mutexCallFunction mLockMutexFunc;
		// Mutex unlock for thread safety. Set by the back-end. If NULL, not called.
		mutexCallFunction mUnlockMutexFunc;
		// Called by SoLoud to shut down the back-end. If NULL, not called. Should be set by back-end.
		soloudCallFunction mBackendCleanupFunc;

		// CTor
		Soloud();
		// DTor
		~Soloud();

		enum FLAGS
		{
			// Use round-off clipper
			CLIP_ROUNDOFF = 1,
			ENABLE_FFT = 2
		};

		// Initialize SoLoud. Called by the back-end.
		void init(int aVoices, int aSamplerate, int aBufferSize, int aFlags);
		// Mix and return N stereo samples in the buffer. Called by the back-end.
		void mix(float *aBuffer, int aSamples);

		// Deinitialize SoLoud. Must be called before shutting down.
		void deinit();

		// Start playing a sound. Returns voice handle, which can be ignored or used to alter the playing sound's parameters.
		int play(AudioSource &aSound, float aVolume = 1.0f, float aPan = 0.0f, int aPaused = 0, int aBus = 0);
		// Seek the audio stream to certain point in time. Some streams can't seek backwards. Relative play speed affects time.
		void seek(int aVoiceHandle, float aSeconds);
		// Stop the sound.
		void stop(int aVoiceHandle);
		// Stop all voices.
		void stopAll();
		// Stop all voices that play this sound source
		void stopSound(AudioSource &aSound);

		// Set a live filter parameter. Use 0 for the global filters.
		void setFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId, float aValue);
		// Get a live filter parameter. Use 0 for the global filters.
		float getFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId);
		// Fade a live filter parameter. Use 0 for the global filters.
		void fadeFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime);
		// Oscillate a live filter parameter. Use 0 for the global filters.
		void oscillateFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime);

		// Get current play time, in seconds.
		float getStreamTime(int aVoiceHandle) const;
		// Get current pause state.
		int getPause(int aVoiceHandle) const;
		// Get current volume.
		float getVolume(int aVoiceHandle) const;
		// Get current pan.
		float getPan(int aVoiceHandle) const;
		// Get current sample rate.
		float getSamplerate(int aVoiceHandle) const;
		// Get current voice protection state.
		int getProtectVoice(int aVoiceHandle) const;
		// Get the current number of busy voices.
		int getActiveVoiceCount() const; 
		// Check if the handle is still valid, or if the sound has stopped.
		int isValidVoiceHandle(int aVoiceHandle) const;
		// Get current relative play speed.
		float getRelativePlaySpeed(int aVoiceHandle) const;
		// Get current post-clip scaler value.
		float getPostClipScaler() const;
		// Get current global volume
		float getGlobalVolume() const;

		// Set the global volume
		void setGlobalVolume(float aVolume);
		// Set the post clip scaler value
		void setPostClipScaler(float aScaler);
		// Set the pause state
		void setPause(int aVoiceHandle, int aPause);
		// Pause all voices
		void setPauseAll(int aPause);
		// Set the relative play speed
		void setRelativePlaySpeed(int aVoiceHandle, float aSpeed);
		// Set the voice protection state
		void setProtectVoice(int aVoiceHandle, int aProtect);
		// Set the sample rate
		void setSamplerate(int aVoiceHandle, float aSamplerate);
		// Set panning value; -1 is left, 0 is center, 1 is right
		void setPan(int aVoiceHandle, float aPan);
		// Set absolute left/right volumes
		void setPanAbsolute(int aVoiceHandle, float aLVolume, float aRVolume);
		// Set overall volume
		void setVolume(int aVoiceHandle, float aVolume);

		// Set up volume fader
		void fadeVolume(int aVoiceHandle, float aFrom, float aTo, float aTime);
		// Set up panning fader
		void fadePan(int aVoiceHandle, float aFrom, float aTo, float aTime);
		// Set up relative play speed fader
		void fadeRelativePlaySpeed(int aVoiceHandle, float aFrom, float aTo, float aTime);
		// Set up global volume fader
		void fadeGlobalVolume(float aFrom, float aTo, float aTime);
		// Schedule a stream to pause
		void schedulePause(int aVoiceHandle, float aTime);
		// Schedule a stream to stop
		void scheduleStop(int aVoiceHandle, float aTime);

		// Set up volume oscillator
		void oscillateVolume(int aVoiceHandle, float aFrom, float aTo, float aTime);
		// Set up panning oscillator
		void oscillatePan(int aVoiceHandle, float aFrom, float aTo, float aTime);
		// Set up relative play speed oscillator
		void oscillateRelativePlaySpeed(int aVoiceHandle, float aFrom, float aTo, float aTime);
		// Set up global volume oscillator
		void oscillateGlobalVolume(float aFrom, float aTo, float aTime);

		// Set global filters. Set to NULL to clear the filter.
		void setGlobalFilter(int aFilterId, Filter *aFilter);

#ifdef SOLOUD_INCLUDE_FFT
		// Calculate FFT
		float *calcFFT();
#endif

		// Rest of the stuff is used internally.
	public:
		// Perform mixing for a specific bus
		void mixBus(float *aBuffer, int aSamples, float *aScratch, int aBus);
		// Scratch buffer, used for resampling.
		float *mScratch;
		// Current size of the scratch, in samples.
		int mScratchSize;
		// Amount of scratch needed.
		int mScratchNeeded;
		// Audio voices.
		AudioSourceInstance **mVoice;
		// Number of concurrent voices.
		int mVoiceCount;
		// Output sample rate
		int mSamplerate;
		// Output channel count
		int mChannels;
		// Maximum size of output buffer; used to calculate needed scratch.
		int mBufferSize;
		// Flags; see Soloud::FLAGS
		int mFlags;
		// Global volume. Applied before clipping.
		float mGlobalVolume;
		// Post-clip scaler. Applied after clipping.
		float mPostClipScaler;
		// Current play index. Used to create audio handles.
		unsigned int mPlayIndex;
		// Current sound source index. Used to create sound source IDs.
		int mAudioSourceID;
		// Fader for the global volume.
		Fader mGlobalVolumeFader;
		// Global stream time, for the global volume fader. Re-set when global volume fader is set.
		float mStreamTime;
		// Global filter
		Filter *mFilter[FILTERS_PER_STREAM];
		// Global filter instance
		FilterInstance *mFilterInstance[FILTERS_PER_STREAM];
		// Find a free voice, stopping the oldest if no free voice is found.
		int findFreeVoice();
		// Converts handle to voice, if the handle is valid.
		int getVoiceFromHandle(int aVoiceHandle) const;
		// Converts voice + playindex into handle
		int getHandleFromVoice(int aVoice) const;
		// Stop voice (not handle).
		void stopVoice(int aVoice);
		// Set voice (not handle) pan.
		void setVoicePan(int aVoice, float aPan);
		// Set voice (not handle) relative play speed.
		void setVoiceRelativePlaySpeed(int aVoice, float aSpeed);
		// Set voice (not handle) volume.
		void setVoiceVolume(int aVoice, float aVolume);
		// Set voice (not handle) pause state.
		void setVoicePause(int aVoice, int aPause);
		// Clip the samples in the buffer
		void clip(float *aBuffer, float *aDestBuffer, int aSamples, float aVolume0, float aVolume1);
#ifdef SOLOUD_INCLUDE_FFT
		// FFT calculation code
		FFT mFFT;
		// Data gathered for FFT input
		float mFFTInput[512];
		// FFT output data
		float mFFTData[256];
#endif
	};

	// SDL back-end initialization call
	int sdl_init(SoLoud::Soloud *aSoloud, int aVoices = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// OpenAL back-end initialization call
	int openal_init(SoLoud::Soloud *aSoloud, int aVoices = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// PortAudio back-end initialization call
	int portaudio_init(SoLoud::Soloud *aSoloud, int aVoices = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// WinMM back-end initialization call
	int winmm_init(SoLoud::Soloud *aSoloud, int aVoices = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 4096);

	// Xaudio2 back-end initialization call
	int xaudio2_init(SoLoud::Soloud *aSoloud, int aVoices = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// Deinterlace samples in a buffer. From 12121212 to 11112222
	void deinterlace_samples(const float *aSourceBuffer, float *aDestBuffer, int aSamples, int aChannels);

	// Interlace samples in a buffer. From 11112222 to 12121212
	void interlace_samples(const float *aSourceBuffer, float *aDestBuffer, int aSamples, int aChannels);
};

#endif 