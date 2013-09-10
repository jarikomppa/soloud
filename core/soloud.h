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

namespace SoLoud
{
	class Soloud;

	typedef void (*mutexCallFunction)(void *aMutexPtr);
	typedef void (*soloudCallFunction)(Soloud *aSoloud);

	class AudioSource;

	class FilterInstance
	{
	public:
		virtual void filter(float *aBuffer, int aSamples, int aStereo, float aSamplerate, float aTime) = 0;
		virtual void setFilterParameter(int aAttributeId, float aValue);
		virtual void fadeFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime);
		virtual void oscillateFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime);

		virtual ~FilterInstance();
	};

	class Filter
	{
	public:
		virtual FilterInstance *createInstance() = 0;
		virtual ~Filter();
	};

	// Helper class to process faders
	class Fader
	{
	public:
		// Value to fade from
		float mFrom;
		// Value to fade to
		float mTo;
		// Delta between from and to
		float mDelta;
		// Total time to fade
		float mTime;
		// Time fading started
		float mStartTime;
		// Time fading will end
		float mEndTime;
		// Current value. Used in case time rolls over.
		float mCurrent;
		// Active flag; 0 means disabled, 1 is active, 2 is LFO, -1 means was active, but stopped
		int mActive;
		// Ctor
		Fader();
		// Set up LFO
		void setLFO(float aFrom, float aTo, float aTime, float aStartTime);
		// Set up fader
		void set(float aFrom, float aTo, float aTime, float aStartTime);
		// Get the current fading value
		float get(float aCurrentTime);
	}; 

	// Base class for audio instances
	class AudioInstance
	{
	public:
		enum FLAGS
		{			
			// This audio instance loops (if supported)
			LOOPING = 1,
			// This audio instance outputs stereo samples
			STEREO = 2,
			// This audio instance is protected - won't get stopped if we run out of channels
			PROTECTED = 4,
			// This audio instance is paused
			PAUSED = 8
		};
		// Ctor
		AudioInstance();
		// Dtor
		virtual ~AudioInstance();
		// Play index; used to identify instances from handles
		unsigned int mPlayIndex;
		// Flags; see AudioInstance::FLAGS
		int mFlags;
		// Pan value, for getPan()
		float mPan;
		// Left channel volume (panning)
		float mLVolume;
		// Right channel volume (panning)
		float mRVolume;
		// Overall volume
		float mVolume;
		// Base samplerate; samplerate = base samplerate * relative play speed
		float mBaseSamplerate;
		// Samplerate; samplerate = base samplerate * relative play speed
		float mSamplerate;
		// Relative play speed; samplerate = base samplerate * relative play speed
		float mRelativePlaySpeed;
		// How long this stream has played, in seconds.
		float mStreamTime;
		// Fader for the audio panning
		Fader mPanFader;
		// Fader for the audio volume
		Fader mVolumeFader;
		// Fader for the relative play speed
		Fader mRelativePlaySpeedFader;
		// Fader used to schedule pausing of the stream
		Fader mPauseScheduler;
		// Fader used to schedule stopping of the stream
		Fader mStopScheduler;
		// Affected by some fader
		int mActiveFader;
		// Fader-affected l/r volumes
		float mFaderVolume[2 * 2];
		// ID of the sound source that generated this instance
		int mAudioSourceID;
		// Filter pointer
		FilterInstance *mFilter[FILTERS_PER_STREAM];
		// Initialize instance. Mostly internal use.
		void init(int aPlayIndex, float aBaseSamplerate, int aSourceFlags);
		// Get N samples from the stream to the buffer
		virtual void getAudio(float *aBuffer, int aSamples) = 0;
		// Has the stream ended?
		virtual int hasEnded() = 0;
		// Seek to certain place in the stream. Base implementation is generic "tape" seek (and slow).
		virtual void seek(float aSeconds, float *mScratch, int mScratchSize);
		// Rewind stream. Base implementation returns 0, meaning it can't rewind.
		virtual int rewind();
	};

	class Soloud;

	// Base class for audio sources
	class AudioSource
	{
	public:
		enum FLAGS
		{
			// The instances from this audio source should loop
			SHOULD_LOOP = 1,
			// This audio source produces stereo samples
			STEREO = 2
		};
		// Flags. See AudioSource::FLAGS
		int mFlags;
		// Base sample rate, used to initialize instances
		float mBaseSamplerate;
		// Sound source ID. Assigned by SoLoud the first time it's played.
		int mAudioSourceID;
		// Filter pointer
		Filter *mFilter[FILTERS_PER_STREAM];
		// Pointer to the Soloud object. Needed to stop all instances in dtor.
		Soloud *mSoloud;

		// CTor
		AudioSource();
		// Set the looping of the instances created from this audio source
		void setLooping(int aLoop);
		// Set filter. Set to NULL to clear the filter.
		void setFilter(int aFilterId, Filter *aFilter);
		// DTor
		virtual ~AudioSource();
		// Create instance from the audio source. Called from within Soloud class.
		virtual AudioInstance *createInstance() = 0;
	};

	// Soloud core class.
	class Soloud
	{
#ifdef SOLOUD_INCLUDE_FFT
		// FFT calculation code
		FFT mFFT;
		// Data gathered for FFT input
		float mFFTInput[512];
		// FFT output data
		float mFFTData[256];
#endif
		// Scratch buffer, used for resampling.
		float *mScratch;
		// Current size of the scratch, in stereo samples.
		int mScratchSize;
		// Amount of scratch needed.
		int mScratchNeeded;
		// Audio channels.
		AudioInstance **mChannel;
		// Number of channels
		int mChannelCount;
		// Output sample rate
		int mSamplerate;
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
		// Find a free channel, stopping the oldest if no free channel is found.
		int findFreeChannel();
		// Converts handle to channel, if the handle is valid.
		int getChannelFromHandle(int aChannelHandle) const;
		// Stop channel (not handle).
		void stopChannel(int aChannel);
		// Set channel (not handle) pan.
		void setChannelPan(int aChannel, float aPan);
		// Set channel (not handle) relative play speed.
		void setChannelRelativePlaySpeed(int aChannel, float aSpeed);
		// Set channel (not handle) volume.
		void setChannelVolume(int aChannel, float aVolume);
		// Set channel (not handle) pause state.
		void setChannelPause(int aChannel, int aPause);
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
		void init(int aChannels, int aSamplerate, int aBufferSize, int aFlags);
		// Mix and return N stereo samples in the buffer. Called by the back-end.
		void mix(float *aBuffer, int aSamples);

		// Deinitialize SoLoud. Must be called before shutting down.
		void deinit();

		// Start playing a sound. Returns channel handle, which can be ignored or used to alter the playing sound's parameters.
		int play(AudioSource &aSound, float aVolume = 1.0f, float aPan = 0.0f, int aPaused = 0);
		// Seek the audio stream to certain point in time. Some streams can't seek backwards. Relative play speed affects time.
		void seek(int aChannelHandle, float aSeconds);
		// Stop the sound.
		void stop(int aChannelHandle);
		// Stop all channels.
		void stopAll();
		// Stop all channels that play this sound source
		void stopSound(AudioSource &aSound);

		// Set a live filter parameter. Use 0 for the global filters.
		void setFilterParameter(int aChannelHandle, int aFilterId, int aAttributeId, float aValue);
		// Fade a live filter parameter. Use 0 for the global filters.
		void fadeFilterParameter(int aChannelHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime);
		// Oscillate a live filter parameter. Use 0 for the global filters.
		void oscillateFilterParameter(int aChannelHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime);

		// Get current play time, in seconds.
		float getStreamTime(int aChannelHandle) const;
		// Get current pause state.
		int getPause(int aChannelHandle) const;
		// Get current volume.
		float getVolume(int aChannelHandle) const;
		// Get current pan.
		float getPan(int aChannelHandle) const;
		// Get current sample rate.
		float getSamplerate(int aChannelHandle) const;
		// Get current channel protection state.
		int getProtectChannel(int aChannelHandle) const;
		// Get the current number of busy channels.
		int getActiveVoiceCount() const; 
		// Check if the handle is still valid, or if the sound has stopped.
		int isValidChannelHandle(int aChannelHandle) const;
		// Get current relative play speed.
		float getRelativePlaySpeed(int aChannelHandle) const;
		// Get current post-clip scaler value.
		float getPostClipScaler() const;
		// Get current global volume
		float getGlobalVolume() const;

		// Set the global volume
		void setGlobalVolume(float aVolume);
		// Set the post clip scaler value
		void setPostClipScaler(float aScaler);
		// Set the pause state
		void setPause(int aChannelHandle, int aPause);
		// Pause all channels
		void setPauseAll(int aPause);
		// Set the relative play speed
		void setRelativePlaySpeed(int aChannelHandle, float aSpeed);
		// Set the channel protection state
		void setProtectChannel(int aChannelHandle, int aProtect);
		// Set the sample rate
		void setSamplerate(int aChannelHandle, float aSamplerate);
		// Set panning value; -1 is left, 0 is center, 1 is right
		void setPan(int aChannelHandle, float aPan);
		// Set absolute left/right volumes
		void setPanAbsolute(int aChannelHandle, float aLVolume, float aRVolume);
		// Set overall volume
		void setVolume(int aChannelHandle, float aVolume);

		// Set up volume fader
		void fadeVolume(int aChannelHandle, float aFrom, float aTo, float aTime);
		// Set up panning fader
		void fadePan(int aChannelHandle, float aFrom, float aTo, float aTime);
		// Set up relative play speed fader
		void fadeRelativePlaySpeed(int aChannelHandle, float aFrom, float aTo, float aTime);
		// Set up global volume fader
		void fadeGlobalVolume(float aFrom, float aTo, float aTime);
		// Schedule a stream to pause
		void schedulePause(int aChannelHandle, float aTime);
		// Schedule a stream to stop
		void scheduleStop(int aChannelHandle, float aTime);

		// Set up volume oscillator
		void oscillateVolume(int aChannelHandle, float aFrom, float aTo, float aTime);
		// Set up panning oscillator
		void oscillatePan(int aChannelHandle, float aFrom, float aTo, float aTime);
		// Set up relative play speed oscillator
		void oscillateRelativePlaySpeed(int aChannelHandle, float aFrom, float aTo, float aTime);
		// Set up global volume oscillator
		void oscillateGlobalVolume(float aFrom, float aTo, float aTime);

		// Set global filters. Set to NULL to clear the filter.
		void setGlobalFilter(int aFilterId, Filter *aFilter);

#ifdef SOLOUD_INCLUDE_FFT
		// Calculate FFT
		float *calcFFT();
#endif
	};

	// SDL back-end initialization call
	int sdl_init(SoLoud::Soloud *aSoloud, int aChannels = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// OpenAL back-end initialization call
	int openal_init(SoLoud::Soloud *aSoloud, int aChannels = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// PortAudio back-end initialization call
	int portaudio_init(SoLoud::Soloud *aSoloud, int aChannels = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);

	// WinMM back-end initialization call
	int winmm_init(SoLoud::Soloud *aSoloud, int aChannels = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 4096);

	// Xaudio2 back-end initialization call
	int xaudio2_init(SoLoud::Soloud *aSoloud, int aChannels = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);
};

#endif 