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

// Core operations related to filters

namespace SoLoud
{
	void Soloud::setGlobalFilter(int aFilterId, Filter *aFilter)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		delete mFilterInstance[aFilterId];
		mFilterInstance[aFilterId] = 0;
		
		mFilter[aFilterId] = aFilter;
		if (aFilter)
		{
			mFilterInstance[aFilterId] = mFilter[aFilterId]->createInstance();
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	float Soloud::getFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId)
	{
		float ret = 0;
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return ret;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				ret = mFilterInstance[aFilterId]->getFilterParameter(aAttributeId);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return ret;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return ret;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			ret = mVoice[ch]->mFilter[aFilterId]->getFilterParameter(aAttributeId);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		
		return ret;
	}

	void Soloud::setFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId, float aValue)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				mFilterInstance[aFilterId]->setFilterParameter(aAttributeId, aValue);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			mVoice[ch]->mFilter[aFilterId]->setFilterParameter(aAttributeId, aValue);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId, float aTo, double aTime)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				mFilterInstance[aFilterId]->fadeFilterParameter(aAttributeId, aTo, aTime, mStreamTime);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			mVoice[ch]->mFilter[aFilterId]->fadeFilterParameter(aAttributeId, aTo, aTime, mStreamTime);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillateFilterParameter(handle aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, double aTime)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				mFilterInstance[aFilterId]->oscillateFilterParameter(aAttributeId, aFrom, aTo, aTime, mStreamTime);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			mVoice[ch]->mFilter[aFilterId]->oscillateFilterParameter(aAttributeId, aFrom, aTo, aTime, mStreamTime);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

}
