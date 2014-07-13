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

// Direct voice operations (no mutexes - called from other functions)

namespace SoLoud
{
	void Soloud::setVoiceRelativePlaySpeed(unsigned int aVoice, float aSpeed)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mRelativePlaySpeed = aSpeed;
			mVoice[aVoice]->mSamplerate = mVoice[aVoice]->mBaseSamplerate * mVoice[aVoice]->mRelativePlaySpeed;
		}
	}

	void Soloud::setVoicePause(unsigned int aVoice, int aPause)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mPauseScheduler.mActive = 0;

			if (aPause)
			{
				mVoice[aVoice]->mFlags |= AudioSourceInstance::PAUSED;
			}
			else
			{
				mVoice[aVoice]->mFlags &= ~AudioSourceInstance::PAUSED;
			}
		}
	}

	void Soloud::setVoicePan(unsigned int aVoice, float aPan)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mPan = aPan;
			mVoice[aVoice]->mChannelVolume[0] = (float)cos((aPan + 1) * M_PI / 4);
			mVoice[aVoice]->mChannelVolume[1] = (float)sin((aPan + 1) * M_PI / 4);
		}
	}

	void Soloud::setVoiceVolume(unsigned int aVoice, float aVolume)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mVolume = aVolume;
		}
	}

	void Soloud::stopVoice(unsigned int aVoice)
	{
		if (mVoice[aVoice])
		{
			// Delete via temporary variable to avoid recursion
			AudioSourceInstance * v = mVoice[aVoice];
			mVoice[aVoice] = 0;			
			delete v;
		}
	}


}
