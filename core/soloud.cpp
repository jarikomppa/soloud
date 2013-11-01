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

#include <string.h>
#include <stdlib.h> // rand
#include <math.h> // sin
#include "soloud.h"

namespace SoLoud
{
	Soloud::Soloud()
	{
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
	}

	Soloud::~Soloud()
	{
		deinit();
		stopAll();
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			delete mFilterInstance[i];
		}
		delete[] mScratch;
	}

	void Soloud::deinit()
	{
		if (mBackendCleanupFunc)
			mBackendCleanupFunc(this);
		mBackendCleanupFunc = 0;
	}

	void Soloud::init(int aSamplerate, int aBufferSize, int aFlags)
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
		mPostClipScaler = 0.5f;
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

	static void smbFft(float *fftBuffer, int fftFrameSizeLog, int sign)
	/* 
	 * COPYRIGHT 1996 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
	 *
	 * 						The Wide Open License (WOL)
	 *
	 * Permission to use, copy, modify, distribute and sell this software and its
	 * documentation for any purpose is hereby granted without fee, provided that
	 * the above copyright notice and this license appear in all source copies. 
	 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
	 * ANY KIND. See http://www.dspguru.com/wol.htm for more information.
	 *
	 * Sign = -1 is FFT, 1 is iFFT (inverse)
	 * Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	 * time domain data in fftBuffer[0...2*fftFrameSize-1]. 
	 * The FFT array takes and returns the cosine and sine parts in an interleaved 
	 * manner, ie.	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. 
	 * fftFrameSize	must be a power of 2. 
	 * It expects a complex input signal (see footnote 2), ie. when working with 
	 * 'common' audio signals our input signal has to be passed as 
	 * {in[0],0.,in[1],0.,in[2],0.,...} asf. 
	 * In that case, the transform of the frequencies of interest is in 
	 * fftBuffer[0...fftFrameSize].
	*/
	{
		float wr, wi, arg, *p1, *p2, temp;
		float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
		int i, bitm, j, le, le2, k;
		int fftFrameSize = 1 << fftFrameSizeLog;

		for (i = 2; i < 2 * fftFrameSize - 2; i += 2) 
		{
			for (bitm = 2, j = 0; bitm < 2 * fftFrameSize; bitm <<= 1) 
			{
				if (i & bitm) j++;
				j <<= 1;
			}

			if (i < j) 
			{
				p1 = fftBuffer+i; 
				p2 = fftBuffer+j;
				temp = *p1; 
				*(p1++) = *p2;
				*(p2++) = temp; 
				temp = *p1;
				*p1 = *p2; 
				*p2 = temp;
			}
		}
		for (k = 0, le = 2; k < fftFrameSizeLog; k++) 
		{
			le <<= 1;
			le2 = le >> 1;
			ur = 1.0;
			ui = 0.0;
			arg = (float)(M_PI / (le2 >> 1));
			wr = cos(arg);
			wi = sign * sin(arg);
			for (j = 0; j < le2; j += 2) 
			{
				p1r = fftBuffer + j; 
				p1i = p1r + 1;
				p2r = p1r + le2; 
				p2i = p2r + 1;
				for (i = j; i < 2 * fftFrameSize; i += le) 
				{
					tr = *p2r * ur - *p2i * ui;
					ti = *p2r * ui + *p2i * ur;
					*p2r = *p1r - tr; 
					*p2i = *p1i - ti;
					*p1r += tr; 
					*p1i += ti;
					p1r += le; 
					p1i += le;
					p2r += le; 
					p2i += le;
				}
				tr = ur * wr - ui * wi;
				ui = ur * wi + ui * wr;
				ur = tr;
			}
		}
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

		smbFft(temp, 9, -1);

		for (i = 0; i < 256; i++)
		{
			float real = temp[i*2];
			float imag = temp[i*2+1];
			mFFTData[i] = sqrt(real*real+imag*imag);
		}

		return mFFTData;
	}

	void Soloud::clip(float *aBuffer, float *aDestBuffer, int aSamples, float aVolume0, float aVolume1)
	{
		float vd = (aVolume1 - aVolume0) / aSamples;
		float v = aVolume0;
		int i, j, c;
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
			int p = pos >> 16;
			int f = pos & 0xffff;
#ifdef _DEBUG
			if (p >= SAMPLE_GRANULARITY)
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
			aDst[i] = s1 + (s2 - s1) * f * (1 / 65536.0f);
		}
#else // Point sample
		int i;
		float pos = aSrcOffset;

		for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
		{
			int p = pos >> 16;
			aDst[i] = aSrc[p];
		}
#endif
	}

	void Soloud::mixBus(float *aBuffer, int aSamples, float *aScratch, int aBus, float aSamplerate)
	{
		int i;
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
				int j;
				float step = mVoice[i]->mSamplerate / aSamplerate;
				int step_fixed = step * 65536;
				float samples_per_block = SAMPLE_GRANULARITY / step;
				int outofs = 0;
		
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

						if (mVoice[i]->mSrcOffset + step_fixed > (SAMPLE_GRANULARITY * 65536))
						{
							mVoice[i]->mSrcOffset -= SAMPLE_GRANULARITY * 65536;
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

					int writesamples = 0;

					if (mVoice[i]->mSrcOffset < SAMPLE_GRANULARITY * 65536)
					{
						writesamples = ((SAMPLE_GRANULARITY * 65536) - mVoice[i]->mSrcOffset) / step_fixed + 1;

						// avoid reading past the current buffer..
						if (((writesamples * step_fixed + mVoice[i]->mSrcOffset) >> 16) >= SAMPLE_GRANULARITY + 1)
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


				int chofs[2];
				chofs[0] = 0;
				chofs[1] = aSamples;
				
				if (mVoice[i]->mActiveFader)
				{
					float lpan = mVoice[i]->mFaderVolume[0];
					float rpan = mVoice[i]->mFaderVolume[2];
					float lpani = mVoice[i]->mFaderVolume[1];
					float rpani = mVoice[i]->mFaderVolume[3];

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
					
					mVoice[i]->mFaderVolume[0] = lpan;
					mVoice[i]->mFaderVolume[2] = rpan;
				}
				else
				{
					float lpan = mVoice[i]->mLVolume * mVoice[i]->mVolume;
					float rpan = mVoice[i]->mRVolume * mVoice[i]->mVolume;
					if (mVoice[i]->mChannels == 2)
					{
						for (j = 0; j < aSamples; j++)
						{
							float s1 = aScratch[chofs[0] + j];
							float s2 = aScratch[chofs[1] + j];
							aBuffer[j + 0] += s1 * lpan;
							aBuffer[j + aSamples] += s2 * rpan;
						}
					}
					else
					{
						for (j = 0; j < aSamples; j++)
						{
							float s = aScratch[chofs[0] + j];
							aBuffer[j + 0] += s * lpan;
							aBuffer[j + aSamples] += s * rpan;
						}
					}
				}

				// clear voice if the sound is over
				if (!(mVoice[i]->mFlags & AudioSourceInstance::LOOPING) && mVoice[i]->hasEnded())
				{
					stopVoice(i);
				}
			}
		}
	}

	void Soloud::mix(float *aBuffer, int aSamples)
	{
		float buffertime = aSamples / (float)mSamplerate;
		float globalVolume[2];
		mStreamTime += buffertime;

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
				float panl[2];
				float panr[2];

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


				panl[0] = mVoice[i]->mLVolume;
				panr[0] = mVoice[i]->mRVolume;
				if (mVoice[i]->mPanFader.mActive > 0)
				{
					float pan = mVoice[i]->mPanFader.get(mVoice[i]->mStreamTime);
					setVoicePan(i, pan);
					mVoice[i]->mActiveFader = 1;
				}
				panl[1] = mVoice[i]->mLVolume;
				panr[1] = mVoice[i]->mRVolume;

				if (mVoice[i]->mPauseScheduler.mActive)
				{
					mVoice[i]->mPauseScheduler.get(mVoice[i]->mStreamTime);
					if (mVoice[i]->mPauseScheduler.mActive == -1)
					{
						mVoice[i]->mPauseScheduler.mActive = 0;
						setVoicePause(i, 1);
					}
				}

				if (mVoice[i]->mActiveFader)
				{
					mVoice[i]->mFaderVolume[0*2+0] = panl[0] * volume[0];
					mVoice[i]->mFaderVolume[0*2+1] = panl[1] * volume[1];
					mVoice[i]->mFaderVolume[1*2+0] = panr[0] * volume[0];
					mVoice[i]->mFaderVolume[1*2+1] = panr[1] * volume[1];

					mVoice[i]->mFaderVolume[0*2+1] = (mVoice[i]->mFaderVolume[1] - mVoice[i]->mFaderVolume[0]) / aSamples;
					mVoice[i]->mFaderVolume[1*2+1] = (mVoice[i]->mFaderVolume[3] - mVoice[i]->mFaderVolume[2]) / aSamples;

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

	void deinterlace_samples(const float *aSourceBuffer, float *aDestBuffer, int aSamples, int aChannels)
	{
		// 121212 -> 111222
		int i, j, c;
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

	void interlace_samples(const float *aSourceBuffer, float *aDestBuffer, int aSamples, int aChannels)
	{
		// 111222 -> 121212
		int i, j, c;
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
