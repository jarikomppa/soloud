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

#ifndef SOLOUD_H
#define SOLOUD_H

#include <stdlib.h> // rand
#include <math.h> // sin

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifdef _MSC_VER
#define WINDOWS_VERSION
#endif

#define SOLOUD_VERSION 100

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Configuration defines

// Maximum number of filters per stream
#define FILTERS_PER_STREAM 4

// Number of samples to process on one go
#define SAMPLE_GRANULARITY 512

// Maximum number of concurrent voices (hard limit is 4095)
#define VOICE_COUNT 64

// Use linear resampler
#define RESAMPLER_LINEAR

//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Typedefs have to be made before the includes, as the 
// includes depend on them.
namespace SoLoud
{
	class Soloud;
	typedef void (*mutexCallFunction)(void *aMutexPtr);
	typedef void (*soloudCallFunction)(Soloud *aSoloud);
	typedef int result;
	typedef unsigned int handle;
	typedef double time;
};

#include "soloud_filter.h"
#include "soloud_fader.h"
#include "soloud_audiosource.h"
#include "soloud_bus.h"
#include "soloud_error.h"

namespace SoLoud
{
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

		enum BACKENDS
		{
			AUTO = 0,
			SDL,
			SDL2,
			PORTAUDIO,
			WINMM,
			XAUDIO2,
			WASAPI,
			OSS,
			OPENAL,
			BACKEND_MAX,
		};

		enum FLAGS
		{
			// Use round-off clipper
			CLIP_ROUNDOFF = 1,
			ENABLE_VISUALIZATION = 2,
		};

		// Initialize SoLoud. Must be called before SoLoud can be used.
		result init(int aFlags = Soloud::CLIP_ROUNDOFF, int aBackend = Soloud::AUTO, int aSamplerate = Soloud::AUTO, int aBufferSize = Soloud::AUTO);

		// Deinitialize SoLoud. Must be called before shutting down.
		void deinit();

		// Query SoLoud version number (should equal to SOLOUD_VERSION macro)
		int getVersion() const;

		// Translate error number to an asciiz string
		const char * getErrorString(result aErrorCode) const;

		// Start playing a sound. Returns voice handle, which can be ignored or used to alter the playing sound's parameters.
		handle play(AudioSource &aSound, float aVolume = 1.0f, float aPan = 0.0f, int aPaused = 0, int aBus = 0);

		// Start playing a sound delayed in relation to other sounds called via this function.
		handle playClocked(time aSoundTime, AudioSource &aSound, float aVolume = 1.0f, float aPan = 0.0f, int aBus = 0);

		// Seek the audio stream to certain point in time. Some streams can't seek backwards. Relative play speed affects time.
		void seek(handle aVoiceHandle, time aSeconds);
		// Stop the sound.
		void stop(handle aVoiceHandle);
		// Stop all voices.
		void stopAll();
		// Stop all voices that play this sound source
		void stopAudioSource(AudioSource &aSound);

		// Set a live filter parameter. Use 0 for the global filters.
		void setFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId, float aValue);
		// Get a live filter parameter. Use 0 for the global filters.
		float getFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId);
		// Fade a live filter parameter. Use 0 for the global filters.
		void fadeFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId, float aTo, time aTime);
		// Oscillate a live filter parameter. Use 0 for the global filters.
		void oscillateFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, time aTime);

		// Get current play time, in seconds.
		time getStreamTime(handle aVoiceHandle) const;
		// Get current pause state.
		bool getPause(handle aVoiceHandle) const;
		// Get current volume.
		float getVolume(handle aVoiceHandle) const;
		// Get current pan.
		float getPan(handle aVoiceHandle) const;
		// Get current sample rate.
		float getSamplerate(handle aVoiceHandle) const;
		// Get current voice protection state.
		bool getProtectVoice(handle aVoiceHandle) const;
		// Get the current number of busy voices.
		int getActiveVoiceCount() const; 
		// Check if the handle is still valid, or if the sound has stopped.
		bool isValidVoiceHandle(handle aVoiceHandle) const;
		// Get current relative play speed.
		float getRelativePlaySpeed(handle aVoiceHandle) const;
		// Get current post-clip scaler value.
		float getPostClipScaler() const;
		// Get current global volume
		float getGlobalVolume() const;

		// Set the global volume
		void setGlobalVolume(float aVolume);
		// Set the post clip scaler value
		void setPostClipScaler(float aScaler);
		// Set the pause state
		void setPause(handle aVoiceHandle, bool aPause);
		// Pause all voices
		void setPauseAll(bool aPause);
		// Set the relative play speed
		void setRelativePlaySpeed(handle aVoiceHandle, float aSpeed);
		// Set the voice protection state
		void setProtectVoice(handle aVoiceHandle, bool aProtect);
		// Set the sample rate
		void setSamplerate(handle aVoiceHandle, float aSamplerate);
		// Set panning value; -1 is left, 0 is center, 1 is right
		void setPan(handle aVoiceHandle, float aPan);
		// Set absolute left/right volumes
		void setPanAbsolute(handle aVoiceHandle, float aLVolume, float aRVolume);
		// Set overall volume
		void setVolume(handle aVoiceHandle, float aVolume);
		// Set delay, in samples, before starting to play samples. Calling this on a live sound will cause glitches.
		void setDelaySamples(handle aVoiceHandle, int aSamples);

		// Set up volume fader
		void fadeVolume(handle aVoiceHandle, float aTo, time aTime);
		// Set up panning fader
		void fadePan(handle aVoiceHandle, float aTo, time aTime);
		// Set up relative play speed fader
		void fadeRelativePlaySpeed(handle aVoiceHandle, float aTo, time aTime);
		// Set up global volume fader
		void fadeGlobalVolume(float aTo, time aTime);
		// Schedule a stream to pause
		void schedulePause(handle aVoiceHandle, time aTime);
		// Schedule a stream to stop
		void scheduleStop(handle aVoiceHandle, time aTime);

		// Set up volume oscillator
		void oscillateVolume(handle aVoiceHandle, float aFrom, float aTo, time aTime);
		// Set up panning oscillator
		void oscillatePan(handle aVoiceHandle, float aFrom, float aTo, time aTime);
		// Set up relative play speed oscillator
		void oscillateRelativePlaySpeed(handle aVoiceHandle, float aFrom, float aTo, time aTime);
		// Set up global volume oscillator
		void oscillateGlobalVolume(float aFrom, float aTo, time aTime);

		// Set global filters. Set to NULL to clear the filter.
		void setGlobalFilter(int aFilterId, Filter *aFilter);

		// Enable or disable visualization data gathering
		void setVisualizationEnable(bool aEnable);
		
		// Calculate and get 256 floats of FFT data for visualization. Visualization has to be enabled before use.
		float *calcFFT();

		// Get 256 floats of wave data for visualization. Visualization has to be enabled before use.
		float *getWave();

		// Get current loop count. Returns 0 if handle is not valid. (All audio sources may not update loop count)
		int getLoopCount(handle aVoiceHandle);

		// Create a voice group. Returns 0 if unable (out of voice groups / out of memory)
		handle createVoiceGroup();
		// Destroy a voice group. 
		result destroyVoiceGroup(handle aVoiceGroupHandle);
		// Add a voice handle to a voice group
		result addVoiceToGroup(handle aVoiceGroupHandle, handle aVoiceHandle);
		// Is this handle a valid voice group?
		bool isVoiceGroup(handle aVoiceGroupHandle);
		// Is this voice group empty?
		bool isVoiceGroupEmpty(handle aVoiceGroupHandle);

		// Rest of the stuff is used internally.
	public:
		// Mix and return N stereo samples in the buffer. Called by the back-end.
		void mix(float *aBuffer, int aSamples);
		// Handle rest of initialization (called from backend)
		void postinit(int aSamplerate, int aBufferSize, int aFlags);

		// Perform mixing for a specific bus
		void mixBus(float *aBuffer, int aSamples, float *aScratch, int aBus, float aSamplerate);
		// Scratch buffer, used for resampling.
		float *mScratch;
		// Current size of the scratch, in samples.
		int mScratchSize;
		// Amount of scratch needed.
		int mScratchNeeded;
		// Audio voices.
		AudioSourceInstance *mVoice[VOICE_COUNT];
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
		// Global stream time, for the global volume fader. 
		time mStreamTime;
		// Last time seen by the playClocked call
		time mLastClockedTime;
		// Global filter
		Filter *mFilter[FILTERS_PER_STREAM];
		// Global filter instance
		FilterInstance *mFilterInstance[FILTERS_PER_STREAM];
		// Find a free voice, stopping the oldest if no free voice is found.
		int findFreeVoice();
		// Converts handle to voice, if the handle is valid.
		int getVoiceFromHandle(handle aVoiceHandle) const;
		// Converts voice + playindex into handle
		handle getHandleFromVoice(int aVoice) const;
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
		// Mono-mixed wave data for visualization and for visualization FFT input
		float mVisualizationWaveData[256];
		// FFT output data
		float mFFTData[256];
		// Snapshot of wave data for visualization
		float mWaveData[256];

		// For each voice group, first int is number of ints alocated.
		unsigned int **mVoiceGroup;
		int mVoiceGroupCount;

		// Remove all non-active voices from group
		void trimVoiceGroup(handle aVoiceGroupHandle);
	};
};

#endif 
