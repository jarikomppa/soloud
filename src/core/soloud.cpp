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

#include <string.h>
#include <stdlib.h> // rand
#include <math.h> // sin
#include "soloud_internal.h"
#include "soloud_fft.h"

//#define FLOATING_POINT_DEBUG

#ifdef FLOATING_POINT_DEBUG
#include <float.h>
#endif

#if !defined(WITH_SDL) && !defined(WITH_PORTAUDIO) && !defined(WITH_OPENAL) && !defined(WITH_XAUDIO2) && !defined(WITH_WINMM) && !defined(WITH_WASAPI) && !defined(WITH_OSS) && !defined(WITH_SDL_NONDYN) && !defined(WITH_SDL2_NONDYN)
#error It appears you haven't enabled any of the back-ends. Please #define one or more of the WITH_ defines (or use premake)
#endif


namespace SoLoud
{
	Soloud::Soloud()
	{
#ifdef FLOATING_POINT_DEBUG
		unsigned int u;
		u = _controlfp(0, 0);
		u = u & ~(_EM_INVALID | /*_EM_DENORMAL |*/ _EM_ZERODIVIDE | _EM_OVERFLOW /*| _EM_UNDERFLOW  | _EM_INEXACT*/);
		_controlfp(u, _MCW_EM);
#endif
		
		mScratch = NULL;
		mScratchSize = 0;
		mScratchNeeded = 0;
		mSamplerate = 0;
		mBufferSize = 0;
		mFlags = 0;
		mGlobalVolume = 0;
		mPlayIndex = 0;
		mBackendData = NULL;
		mMutex = NULL;
		mPostClipScaler = 0;
		mLockMutexFunc = NULL;
		mUnlockMutexFunc = NULL;
		mBackendCleanupFunc = NULL;
		mChannels = 2;		
		mStreamTime = 0;
		mLastClockedTime = 0;
		mAudioSourceID = 1;
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			mFilter[i] = NULL;
			mFilterInstance[i] = NULL;
		}
		for (i = 0; i < 256; i++)
		{
			mFFTData[i] = 0;
			mVisualizationWaveData[i] = 0;
			mWaveData[i] = 0;
		}
		for (i = 0; i < VOICE_COUNT; i++)
		{
			mVoice[i] = 0;
		}
		mVoiceGroup = 0;
		mVoiceGroupCount = 0;

		m3dPosition[0] = 0;
		m3dPosition[1] = 0;
		m3dPosition[2] = 0;
		m3dAt[0] = 0;
		m3dAt[1] = 0;
		m3dAt[2] = -1;
		m3dUp[0] = 0;
		m3dUp[1] = 1;
		m3dUp[2] = 0;		
		m3dVelocity[0] = 0;
		m3dVelocity[1] = 0;
		m3dVelocity[2] = 0;		
		m3dSoundSpeed = 343.3f;

	}

	Soloud::~Soloud()
	{
		// let's stop all sounds before deinit, so we don't mess up our mutexes
		stopAll();
		deinit();
		unsigned int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			delete mFilterInstance[i];
		}
		delete[] mScratch;
		for (i = 0; i < mVoiceGroupCount; i++)
			delete[] mVoiceGroup[i];
		delete[] mVoiceGroup;
	}

	void Soloud::deinit()
	{
		if (mBackendCleanupFunc)
			mBackendCleanupFunc(this);
		mBackendCleanupFunc = 0;
		mLockMutexFunc = 0;
		mUnlockMutexFunc = 0;
	}

	result Soloud::init(unsigned int aFlags, unsigned int aBackend, unsigned int aSamplerate, unsigned int aBufferSize)
	{		
		if (aBackend < 0 || aBackend >= BACKEND_MAX || aSamplerate < 0 || aBufferSize < 0)
			return INVALID_PARAMETER;

		int samplerate = 44100;
		int buffersize = 2048;
		int inited = 0;

		if (aSamplerate != Soloud::AUTO) samplerate = aSamplerate;
		if (aBufferSize != Soloud::AUTO) buffersize = aBufferSize;

#if defined(WITH_SDL_NONDYN)
		if (aBackend == Soloud::SDL || 
			aBackend == Soloud::SDL2 ||
			aBackend == Soloud::AUTO)
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 2048;

			int ret = sdlnondyn_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_SDL2_NONDYN)
		if (aBackend == Soloud::SDL ||
			aBackend == Soloud::SDL2 ||
			aBackend == Soloud::AUTO)
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 2048;

			int ret = sdl2nondyn_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;
		}
#endif

#if defined(WITH_SDL)
		if (aBackend == Soloud::SDL || 
			aBackend == Soloud::SDL2 ||
			aBackend == Soloud::AUTO)
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 2048;

			int ret = sdl_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_PORTAUDIO)
		if (!inited &&
			(aBackend == Soloud::PORTAUDIO ||
			aBackend == Soloud::AUTO))
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 2048;

			int ret = portaudio_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_XAUDIO2)
		if (!inited &&
			(aBackend == Soloud::XAUDIO2 ||
			aBackend == Soloud::AUTO))
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 4096;

			int ret = xaudio2_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_WINMM)
		if (!inited &&
			(aBackend == Soloud::WINMM ||
			aBackend == Soloud::AUTO))
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 4096;

			int ret = winmm_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_WASAPI)
		if (!inited &&
			(aBackend == Soloud::WASAPI ||
			aBackend == Soloud::AUTO))
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 4096;

			int ret = wasapi_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_OSS)
		if (!inited &&
			(aBackend == Soloud::OSS ||
			aBackend == Soloud::AUTO))
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 2048;

			int ret = oss_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif

#if defined(WITH_OPENAL)
		if (!inited &&
			(aBackend == Soloud::OPENAL ||
			aBackend == Soloud::AUTO))
		{
			if (aBufferSize == Soloud::AUTO) buffersize = 4096;

			int ret = openal_init(this, aFlags, samplerate, buffersize);
			if (ret == 0)
				inited = 1;

			if (ret != 0 && aBackend != Soloud::AUTO)
				return ret;			
		}
#endif
		if (!inited)
			return UNKNOWN_ERROR;
		return 0;
	}

	void Soloud::postinit(unsigned int aSamplerate, unsigned int aBufferSize, unsigned int aFlags)
	{		
		mGlobalVolume = 1;
		mSamplerate = aSamplerate;
		mBufferSize = aBufferSize;
		mScratchSize = aBufferSize;
		if (mScratchSize < SAMPLE_GRANULARITY * 2) mScratchSize = SAMPLE_GRANULARITY * 2;
		if (mScratchSize < 4096) mScratchSize = 4096;
		mScratchNeeded = mScratchSize;
		mScratch = new float[mScratchSize * 2];
		mFlags = aFlags;
		mPostClipScaler = 0.95f;
	}

	const char * Soloud::getErrorString(result aErrorCode) const
	{
		switch (aErrorCode)
		{
		case SO_NO_ERROR: return "No error";
		case INVALID_PARAMETER: return "Some parameter is invalid";
		case FILE_NOT_FOUND: return "File not found";
		case FILE_LOAD_FAILED: return "File found, but could not be loaded";
		case DLL_NOT_FOUND: return "DLL not found, or wrong DLL";
		case OUT_OF_MEMORY: return "Out of memory";
		case NOT_IMPLEMENTED: return "Feature not implemented";
		/*case UNKNOWN_ERROR: return "Other error";*/
		}
		return "Other error";
	}


	float * Soloud::getWave()
	{
		int i;
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		for (i = 0; i < 256; i++)
			mWaveData[i] = mVisualizationWaveData[i];
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return mWaveData;
	}


	float * Soloud::calcFFT()
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		float temp[1024];
		int i;
		for (i = 0; i < 256; i++)
		{
			temp[i*2] = mVisualizationWaveData[i];
			temp[i*2+1] = 0;
			temp[i+512] = 0;
			temp[i+768] = 0;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);

		SoLoud::FFT::fft1024(temp);

		for (i = 0; i < 256; i++)
		{
			float real = temp[i];
			float imag = temp[i+512];
			mFFTData[i] = sqrt(real*real+imag*imag);
		}

		return mFFTData;
	}

	void Soloud::clip(float *aBuffer, float *aDestBuffer, unsigned int aSamples, float aVolume0, float aVolume1)
	{
		float vd = (aVolume1 - aVolume0) / aSamples;
		float v = aVolume0;
		unsigned int i, j, c;
		// Clip
		if (mFlags & CLIP_ROUNDOFF)
		{
			int c = 0;
			for (j = 0; j < 2; j++)
			{
				v = aVolume0;
				for (i = 0; i < aSamples; i++, c++, v += vd)
				{
					float f = aBuffer[c] * v;
					if (f <= -1.65f)
					{
						f = -0.9862875f;
					}
					else
					if (f >= 1.65f)
					{
						f = 0.9862875f;
					}
					else
					{
						f =  0.87f * f - 0.1f * f * f * f;
					}
					aDestBuffer[c] = f * mPostClipScaler;
				}
			}
		}
		else
		{
			c = 0;
			for (j = 0; j < 2; j++)
			{
				v = aVolume0;
				for (i = 0; i < aSamples; i++, c++, v += vd)
				{
					float f = aBuffer[i] * v;
					if (f < -1.0f)
					{
						f = -1.0f;
					}
					else
					if (f > 1.0f)
					{
						f = 1.0f;
					}
					aDestBuffer[i] = f * mPostClipScaler;
				}
			}
		}
	}

#define FIXPOINT_FRAC_BITS 20
#define FIXPOINT_FRAC_MUL (1 << FIXPOINT_FRAC_BITS)
#define FIXPOINT_FRAC_MASK ((1 << FIXPOINT_FRAC_BITS) - 1)

	void resample(float *aSrc,
		          float *aSrc1, 
				  float *aDst, 
				  int aSrcOffset,
				  int aDstSampleCount,
				  float aSrcSamplerate, 
				  float aDstSamplerate,
				  int aStepFixed)
	{
#if 0

#elif defined(RESAMPLER_LINEAR)
		int i;
		int pos = aSrcOffset;

		for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
		{
			int p = pos >> FIXPOINT_FRAC_BITS;
			int f = pos & FIXPOINT_FRAC_MASK;
#ifdef _DEBUG
			if (p >= SAMPLE_GRANULARITY || p < 0)
			{
				// This should never actually happen
				p = SAMPLE_GRANULARITY - 1;
			}
#endif
			float s1 = aSrc1[SAMPLE_GRANULARITY - 1];
			float s2 = aSrc[p];
			if (p != 0)
			{
				s1 = aSrc[p-1];
			}
			aDst[i] = s1 + (s2 - s1) * f * (1 / (float)FIXPOINT_FRAC_MUL);
		}
#else // Point sample
		int i;
		int pos = aSrcOffset;

		for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
		{
			int p = pos >> FIXPOINT_FRAC_BITS;
			aDst[i] = aSrc[p];
		}
#endif
	}

	void Soloud::mixBus(float *aBuffer, unsigned int aSamples, float *aScratch, unsigned int aBus, float aSamplerate)
	{
		unsigned int i;
		// Clear accumulation buffer
		for (i = 0; i < aSamples * 2; i++)
		{
			aBuffer[i] = 0;
		}

		// Accumulate sound sources
		for (i = 0; i < VOICE_COUNT; i++)
		{
			if (mVoice[i] && 
				mVoice[i]->mBusHandle == aBus && 
				!(mVoice[i]->mFlags & AudioSourceInstance::PAUSED))
			{
				unsigned int j;
				float step = mVoice[i]->mSamplerate / aSamplerate;
				int step_fixed = (int)floor(step * FIXPOINT_FRAC_MUL);
				unsigned int outofs = 0;
				
				if (mVoice[i]->mDelaySamples)
				{
					if (mVoice[i]->mDelaySamples > aSamples)
					{
						outofs = aSamples;
						mVoice[i]->mDelaySamples -= aSamples;
					}
					else
					{
						outofs = mVoice[i]->mDelaySamples;
						mVoice[i]->mDelaySamples = 0;
					}
					
					// Clear scratch where we're skipping
					for (j = 0; j < mVoice[i]->mChannels; j++)
					{
						memset(aScratch + j * aSamples, 0, sizeof(float) * outofs); 
					}
				}												

				while (outofs < aSamples)
				{
					if (mVoice[i]->mLeftoverSamples == 0)
					{
						// Swap resample buffers (ping-pong)
						AudioSourceResampleData * t = mVoice[i]->mResampleData[0];
						mVoice[i]->mResampleData[0] = mVoice[i]->mResampleData[1];
						mVoice[i]->mResampleData[1] = t;

						// Get a block of source data

						if (mVoice[i]->hasEnded())
						{
							memset(mVoice[i]->mResampleData[0]->mBuffer, 0, sizeof(float) * SAMPLE_GRANULARITY * mVoice[i]->mChannels);
						}
						else
						{
							mVoice[i]->getAudio(mVoice[i]->mResampleData[0]->mBuffer, SAMPLE_GRANULARITY);
						}

						
						

						// If we go past zero, crop to zero (a bit of a kludge)
						if (mVoice[i]->mSrcOffset < SAMPLE_GRANULARITY * FIXPOINT_FRAC_MUL) 
						{
							mVoice[i]->mSrcOffset = 0;
						}
						else
						{
							// We have new block of data, move pointer backwards
							mVoice[i]->mSrcOffset -= SAMPLE_GRANULARITY * FIXPOINT_FRAC_MUL;
						}

					
						// Run the per-stream filters to get our source data

						for (j = 0; j < FILTERS_PER_STREAM; j++)
						{
							if (mVoice[i]->mFilter[j])
							{
								mVoice[i]->mFilter[j]->filter(
									mVoice[i]->mResampleData[0]->mBuffer, 
									SAMPLE_GRANULARITY, 
									mVoice[i]->mChannels,
									mVoice[i]->mSamplerate,
									mStreamTime);
							}
						}
					}
					else
					{
						mVoice[i]->mLeftoverSamples = 0;
					}

					// Figure out how many samples we can generate from this source data.
					// The value may be zero.

					unsigned int writesamples = 0;

					if (mVoice[i]->mSrcOffset < SAMPLE_GRANULARITY * FIXPOINT_FRAC_MUL)
					{
						writesamples = ((SAMPLE_GRANULARITY * FIXPOINT_FRAC_MUL) - mVoice[i]->mSrcOffset) / step_fixed + 1;

						// avoid reading past the current buffer..
						if (((writesamples * step_fixed + mVoice[i]->mSrcOffset) >> FIXPOINT_FRAC_BITS) >= SAMPLE_GRANULARITY)
							writesamples--;
					}


					// If this is too much for our output buffer, don't write that many:
					if (writesamples + outofs > aSamples)
					{
						mVoice[i]->mLeftoverSamples = (writesamples + outofs) - aSamples;
						writesamples = aSamples - outofs;
					}

					// Call resampler to generate the samples, once per channel
					if (writesamples)
					{
						for (j = 0; j < mVoice[i]->mChannels; j++)
						{
							resample(mVoice[i]->mResampleData[0]->mBuffer + SAMPLE_GRANULARITY * j, 
									 mVoice[i]->mResampleData[1]->mBuffer + SAMPLE_GRANULARITY * j, 
									 aScratch + aSamples * j + outofs, 
									 mVoice[i]->mSrcOffset,
									 writesamples,
									 mVoice[i]->mSamplerate,
									 aSamplerate,
									 step_fixed);
						}
					}

					// Keep track of how many samples we've written so far
					outofs += writesamples;

					// Move source pointer onwards (writesamples may be zero)
					mVoice[i]->mSrcOffset += writesamples * step_fixed;
				}


				unsigned int chofs[2];
				chofs[0] = 0;
				chofs[1] = aSamples;
				
				float lpan = mVoice[i]->mCurrentChannelVolume[0];
				float rpan = mVoice[i]->mCurrentChannelVolume[1];
				float lpand = mVoice[i]->mChannelVolume[0] * mVoice[i]->mVolume;
				float rpand = mVoice[i]->mChannelVolume[1] * mVoice[i]->mVolume;
				float lpani = (lpand - lpan) / aSamples;
				float rpani = (rpand - rpan) / aSamples;

				if (mVoice[i]->mChannels == 2)
				{
					for (j = 0; j < aSamples; j++, lpan += lpani, rpan += rpani)
					{
						float s1 = aScratch[chofs[0] + j];
						float s2 = aScratch[chofs[1] + j];
						aBuffer[j + 0] += s1 * lpan;
						aBuffer[j + aSamples] += s2 * rpan;
					}
				}
				else
				{
					for (j = 0; j < aSamples; j++, lpan += lpani, rpan += rpani)
					{
						float s = aScratch[chofs[0] + j];
						aBuffer[j + 0] += s * lpan;
						aBuffer[j + aSamples] += s * rpan;
					}
				}
					
				mVoice[i]->mCurrentChannelVolume[0] = lpand;
				mVoice[i]->mCurrentChannelVolume[1] = rpand;

				// clear voice if the sound is over
				if (!(mVoice[i]->mFlags & AudioSourceInstance::LOOPING) && mVoice[i]->hasEnded())
				{
					stopVoice(i);
				}
			}
		}
	}

	void Soloud::mix(float *aBuffer, unsigned int aSamples)
	{
#ifdef FLOATING_POINT_DEBUG
		// This needs to be done in the audio thread as well..
		static int done = 0;
		if (!done)
		{
			unsigned int u;
			u = _controlfp(0, 0);
			u = u & ~(_EM_INVALID | /*_EM_DENORMAL |*/ _EM_ZERODIVIDE | _EM_OVERFLOW /*| _EM_UNDERFLOW  | _EM_INEXACT*/);
			_controlfp(u, _MCW_EM);
			done = 1;
		}
#endif

		float buffertime = aSamples / (float)mSamplerate;
		float globalVolume[2];
		mStreamTime += buffertime;
		mLastClockedTime = 0;

		globalVolume[0] = mGlobalVolume;
		if (mGlobalVolumeFader.mActive)
		{
			mGlobalVolume = mGlobalVolumeFader.get(mStreamTime);
		}
		globalVolume[1] = mGlobalVolume;

		if (mLockMutexFunc) mLockMutexFunc(mMutex);

		// Process faders. May change scratch size.
		int i;
		for (i = 0; i < VOICE_COUNT; i++)
		{
			if (mVoice[i] && !(mVoice[i]->mFlags & AudioSourceInstance::PAUSED))
			{
				float volume[2];

				mVoice[i]->mActiveFader = 0;

				if (mGlobalVolumeFader.mActive > 0)
				{
					mVoice[i]->mActiveFader = 1;
				}

				mVoice[i]->mStreamTime += buffertime;

				if (mVoice[i]->mRelativePlaySpeedFader.mActive > 0)
				{
					float speed = mVoice[i]->mRelativePlaySpeedFader.get(mVoice[i]->mStreamTime);
					setVoiceRelativePlaySpeed(i, speed);
				}

				volume[0] = mVoice[i]->mVolume;
				if (mVoice[i]->mVolumeFader.mActive > 0)
				{
					mVoice[i]->mVolume = mVoice[i]->mVolumeFader.get(mVoice[i]->mStreamTime);
					mVoice[i]->mActiveFader = 1;
				}
				volume[1] = mVoice[i]->mVolume;

				if (mVoice[i]->mPanFader.mActive > 0)
				{
					float pan = mVoice[i]->mPanFader.get(mVoice[i]->mStreamTime);
					setVoicePan(i, pan);
					mVoice[i]->mActiveFader = 1;
				}

				if (mVoice[i]->mPauseScheduler.mActive)
				{
					mVoice[i]->mPauseScheduler.get(mVoice[i]->mStreamTime);
					if (mVoice[i]->mPauseScheduler.mActive == -1)
					{
						mVoice[i]->mPauseScheduler.mActive = 0;
						setVoicePause(i, 1);
					}
				}

				if (mVoice[i]->mStopScheduler.mActive)
				{
					mVoice[i]->mStopScheduler.get(mVoice[i]->mStreamTime);
					if (mVoice[i]->mStopScheduler.mActive == -1)
					{
						mVoice[i]->mStopScheduler.mActive = 0;
						stopVoice(i);
					}
				}
			}
		}

		// Resize scratch if needed.
		if (mScratchSize < mScratchNeeded)
		{
			mScratchSize = mScratchNeeded;
			delete[] mScratch;
			mScratch = new float[mScratchSize];
		}
		
		mixBus(aBuffer, aSamples, mScratch, 0, (float)mSamplerate);

		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			if (mFilterInstance[i])
			{
				mFilterInstance[i]->filter(aBuffer, aSamples, 2, (float)mSamplerate, mStreamTime);
			}
		}

		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);

		clip(aBuffer, mScratch, aSamples, globalVolume[0], globalVolume[1]);
		interlace_samples(mScratch, aBuffer, aSamples, 2);

		if (mFlags & ENABLE_VISUALIZATION)
		{
			if (aSamples > 255)
			{
				for (i = 0; i < 256; i++)
				{
					mVisualizationWaveData[i] = aBuffer[i*2+0] + aBuffer[i*2+1];
				}
			}
			else
			{
				// Very unlikely failsafe branch
				for (i = 0; i < 256; i++)
				{
					mVisualizationWaveData[i] = aBuffer[((i % aSamples) * 2) + 0] + aBuffer[((i % aSamples) * 2) + 1];
				}
			}
		}
	}

	void deinterlace_samples(const float *aSourceBuffer, float *aDestBuffer, unsigned int aSamples, unsigned int aChannels)
	{
		// 121212 -> 111222
		unsigned int i, j, c;
		c = 0;
		for (j = 0; j < aChannels; j++)
		{
			for (i = j; i < aSamples; i += aChannels)
			{
				aDestBuffer[c] = aSourceBuffer[i + j];
				c++;
			}
		}
	}

	void interlace_samples(const float *aSourceBuffer, float *aDestBuffer, unsigned int aSamples, unsigned int aChannels)
	{
		// 111222 -> 121212
		unsigned int i, j, c;
		c = 0;
		for (j = 0; j < aChannels; j++)
		{
			for (i = j; i < aSamples * aChannels; i += aChannels)
			{
				aDestBuffer[i] = aSourceBuffer[c];
				c++;
			}
		}
	}

};
