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

// Setters - set various bits of SoLoud state

namespace SoLoud
{
	void Soloud::setPostClipScaler(float aScaler)
	{
		mPostClipScaler = aScaler;
	}

	void Soloud::setGlobalVolume(float aVolume)
	{
		mGlobalVolumeFader.mActive = 0;
		mGlobalVolume = aVolume;
	}		

	void Soloud::setRelativePlaySpeed(handle aVoiceHandle, float aSpeed)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mRelativePlaySpeedFader.mActive = 0;
		setVoiceRelativePlaySpeed(ch, aSpeed);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setSamplerate(handle aVoiceHandle, float aSamplerate)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mBaseSamplerate = aSamplerate;
		mVoice[ch]->mSamplerate = mVoice[ch]->mBaseSamplerate * mVoice[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPause(handle aVoiceHandle, bool aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		setVoicePause(ch, aPause);

		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPauseAll(bool aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch;
		for (ch = 0; ch < VOICE_COUNT; ch++)
		{
			setVoicePause(ch, aPause);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setProtectVoice(handle aVoiceHandle, bool aProtect)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		if (aProtect)
		{
			mVoice[ch]->mFlags |= AudioSourceInstance::PROTECTED;
		}
		else
		{
			mVoice[ch]->mFlags &= ~AudioSourceInstance::PROTECTED;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPan(handle aVoiceHandle, float aPan)
	{		
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		setVoicePan(ch, aPan);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPanAbsolute(handle aVoiceHandle, float aLVolume, float aRVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPanFader.mActive = 0;
		mVoice[ch]->mLVolume = aLVolume;
		mVoice[ch]->mRVolume = aRVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setVolume(handle aVoiceHandle, float aVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mVolumeFader.mActive = 0;
		setVoiceVolume(ch, aVolume);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setVisualizationEnable(bool aEnable)
	{
		if (aEnable)
		{
			mFlags |= ENABLE_VISUALIZATION;
		}
		else
		{
			mFlags &= ~ENABLE_VISUALIZATION;
		}
	}

}
