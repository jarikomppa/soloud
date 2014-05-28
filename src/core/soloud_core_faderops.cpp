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

#include "soloud.h"

// Core operations related to faders (not including filters)

namespace SoLoud
{
	void Soloud::schedulePause(handle aVoiceHandle, time aTime)
	{
		if (aTime <= 0)
		{
			setPause(aVoiceHandle, 1);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPauseScheduler.set(1, 0, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::scheduleStop(handle aVoiceHandle, time aTime)
	{
		if (aTime <= 0)
		{
			stop(aVoiceHandle);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mStopScheduler.set(1, 0, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeVolume(handle aVoiceHandle, float aTo, time aTime)
	{
		float from = getVolume(aVoiceHandle);
		if (aTime <= 0 || aTo == from)
		{
			setVolume(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mVolumeFader.set(from, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadePan(handle aVoiceHandle, float aTo, time aTime)
	{
		float from = getPan(aVoiceHandle);
		if (aTime <= 0 || aTo == from)
		{
			setPan(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPanFader.set(from, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeRelativePlaySpeed(handle aVoiceHandle, float aTo, time aTime)
	{
		float from = getRelativePlaySpeed(aVoiceHandle);
		if (aTime <= 0 || aTo == from)
		{
			setRelativePlaySpeed(aVoiceHandle, aTo);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mRelativePlaySpeedFader.set(from, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeGlobalVolume(float aTo, time aTime)
	{
		float from = getGlobalVolume();
		if (aTime <= 0 || aTo == from)
		{
			setGlobalVolume(aTo);
			return;
		}
		mStreamTime = 0; // avoid rollover (~6 days)
		mGlobalVolumeFader.set(from, aTo, aTime, mStreamTime);
	}


	void Soloud::oscillateVolume(handle aVoiceHandle, float aFrom, float aTo, time aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setVolume(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mVolumeFader.setLFO(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillatePan(handle aVoiceHandle, float aFrom, float aTo, time aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setPan(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPanFader.setLFO(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillateRelativePlaySpeed(handle aVoiceHandle, float aFrom, float aTo, time aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setRelativePlaySpeed(aVoiceHandle, aTo);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mRelativePlaySpeedFader.setLFO(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillateGlobalVolume(float aFrom, float aTo, time aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setGlobalVolume(aTo);
			return;
		}
		mStreamTime = 0; // avoid rollover (~6 days)
		mGlobalVolumeFader.setLFO(aFrom, aTo, aTime, mStreamTime);
	}
}
