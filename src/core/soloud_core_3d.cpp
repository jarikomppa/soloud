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

#include <math.h>
#include "soloud_internal.h"

// 3d audio operations

namespace SoLoud
{
	struct vec3
	{
		float mX, mY, mZ;

		float mag()
		{
			return sqrt(mX * mX + mY * mY + mZ * mZ);
		}
		
		void normalize()
		{
			float m = mag();
			if (m == 0)
			{
				mX = mY = mZ = 0;
				return;
			}
			mX /= m;
			mY /= m;
			mZ /= m;
		}
		
		float dot(vec3 a)
		{
			return mX * a.mX + mY * a.mY + mZ * a.mZ;
		}
		
		vec3 sub(vec3 a)
		{
			vec3 r;
			r.mX = mX - a.mX;
			r.mY = mY - a.mY;
			r.mZ = mZ - a.mZ;
			return r;
		}

		vec3 cross(vec3 a)
		{
			vec3 r;

			r.mX = mY * a.mZ - a.mY * mZ;
			r.mY = mZ * a.mX - a.mZ * mX;
			r.mZ = mX * a.mY - a.mX * mY;

			return r;
		}
	};

	struct mat3
	{
		vec3 m[3];

		vec3 mul(vec3 a)
		{
			vec3 r;

			r.mX = m[0].mX * a.mX + m[0].mY * a.mY + m[0].mZ * a.mZ;
			r.mY = m[1].mX * a.mX + m[1].mY * a.mY + m[1].mZ * a.mZ;
			r.mZ = m[2].mX * a.mX + m[2].mY * a.mY + m[2].mZ * a.mZ;

			return r;
		}

		void lookat(vec3 at, vec3 up)
		{
			vec3 z = at;
			z.normalize();
			vec3 x = up.cross(z);
			x.normalize();
			vec3 y = z.cross(x);
			m[0] = x;
			m[1] = y;
			m[2] = z;
		}
	};

#ifndef MIN
#define MIN(a,b) ((a) < (b)) ? (a) : (b)
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b)) ? (a) : (b)
#endif

	float doppler(vec3 aDeltaPos, vec3 aSrcVel, vec3 aDstVel, float aFactor, float aSoundSpeed)
	{
		float deltamag = aDeltaPos.mag();
		if (deltamag == 0)
			return 1.0f;
		float vls = aDeltaPos.dot(aDstVel) / deltamag;
		float vss = aDeltaPos.dot(aSrcVel) / deltamag;
		float maxspeed = aSoundSpeed / aFactor;
		vss = MIN(vss, maxspeed);
		vls = MIN(vls, maxspeed);
		return (aSoundSpeed - aFactor * vls) / (aSoundSpeed - aFactor * vss);
	}

	float attenuateInvDistance(float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor)
	{
		float distance = MAX(aDistance, aMinDistance);
		distance = MIN(distance, aMaxDistance);
		return aMinDistance / (aMinDistance + aRolloffFactor * (distance - aMinDistance));
	}

	float attenuateLinearDistance(float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor)
	{
		float distance = MAX(aDistance, aMinDistance);
		distance = MIN(distance, aMaxDistance);
		return 1 - aRolloffFactor * (distance - aMinDistance) / (aMaxDistance - aMinDistance);
	}

	float attenuateExponentialDistance(float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor)
	{
		float distance = MAX(aDistance, aMinDistance);
		distance = MIN(distance, aMaxDistance);
		return pow(distance / aMinDistance, -aRolloffFactor);
	}

	void Soloud::update3dVoice(int aVoice)
	{
		AudioSourceInstance * v = mVoice[aVoice];
		if (!v) return;

		vec3 speaker[MAX_CHANNELS];

		speaker[0].mX = 2;
		speaker[0].mY = 0;
		speaker[0].mZ = 1;
		speaker[0].normalize();
		speaker[1].mX = -2;
		speaker[1].mY = 0;
		speaker[1].mZ = 1;
		speaker[1].normalize();

		vec3 lpos, lvel, at, up;
		at.mX = m3dAt[0];
		at.mY = m3dAt[1];
		at.mZ = m3dAt[2];
		up.mX = m3dUp[0];
		up.mY = m3dUp[1];
		up.mZ = m3dUp[2];
		lpos.mX = m3dPosition[0];
		lpos.mY = m3dPosition[1];
		lpos.mZ = m3dPosition[2];
		lvel.mX = m3dVelocity[0];
		lvel.mY = m3dVelocity[1];
		lvel.mZ = m3dVelocity[2];
		mat3 m;
		m.lookat(at, up);

		// Optimization: everything above this can be done once per listener update

		float vol = 1;

		// custom collider
		if (v->mCollider)
		{
			vol *= v->mCollider->collide(this, v, v->mColliderData);
		}

		vec3 pos, vel;
		pos.mX = v->m3dPosition[0];
		pos.mY = v->m3dPosition[1];
		pos.mZ = v->m3dPosition[2];

		vel.mX = v->m3dVelocity[0];
		vel.mY = v->m3dVelocity[1];
		vel.mZ = v->m3dVelocity[2];

		if (!(v->mFlags & AudioSourceInstance::LISTENER_RELATIVE))
		{
			pos = pos.sub(lpos);
		}

		float dist = pos.mag();
								
		// attenuation
		switch (mVoice[aVoice]->m3dAttenuationModel)
		{
		case AudioSource::INVERSE_DISTANCE:
			vol = attenuateInvDistance(dist, v->m3dMinDistance, v->m3dMaxDistance, v->m3dAttenuationRolloff);
			break;
		case AudioSource::LINEAR_DISTANCE:
			vol = attenuateLinearDistance(dist, v->m3dMinDistance, v->m3dMaxDistance, v->m3dAttenuationRolloff);
			break;
		case AudioSource::EXPONENTIAL_DISTANCE:
			vol = attenuateExponentialDistance(dist, v->m3dMinDistance, v->m3dMaxDistance, v->m3dAttenuationRolloff);
			break;
		default:
		//case AudioSource::NO_ATTENUATION:
			break;
		}

		// cone

		// (todo) vol *= conev;
								
		// doppler

		v->mRelativePlaySpeed = doppler(pos, vel, lvel, v->m3dDopplerFactor, m3dSoundSpeed);
		v->mSamplerate = v->mBaseSamplerate * v->mRelativePlaySpeed;

		// panning
		pos = m.mul(pos);
		pos.normalize();
		
		// Apply volume to channels based on speaker vectors
		int i;
		for (i = 0; i < MAX_CHANNELS; i++)
		{
			float speakervol = (speaker[i].dot(pos) + 1) / 2;
			// Different speaker "focus" calculations to try, if the default "bleeds" too much..
			//speakervol = (speakervol * speakervol + speakervol) / 2;
			//speakervol = speakervol * speakervol;
			v->mChannelVolume[i] = vol * speakervol;
		}
	}

	void Soloud::update3dAudio()
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);

		int i;
		for (i = 0; i < VOICE_COUNT; i++)
		{
			if (mVoice && mVoice[i] && mVoice[i]->mFlags & AudioSourceInstance::PROCESS_3D)
			{
				update3dVoice(i);
			}
		}

		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}


	handle Soloud::play3d(AudioSource &aSound, float aPosX, float aPosY, float aPosZ, float aVelX, float aVelY, float aVelZ, float aVolume, bool aPaused, unsigned int aBus)
	{
		handle h = play(aSound, aVolume, 0, 1, aBus);
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int v = getVoiceFromHandle(h);
		if (v < 0) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return h;
		}
		mVoice[v]->mFlags |= AudioSourceInstance::PROCESS_3D;
		update3dVoice(v);

		vec3 pos;
		pos.mX = aPosX;
		pos.mY = aPosY;
		pos.mZ = aPosZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);

		int samples = 0;
		if (aSound.mFlags & AudioSource::DISTANCE_DELAY)
		{
			float dist = pos.mag();
			samples += (int)floor((dist / m3dSoundSpeed) * mSamplerate);
		}

		setDelaySamples(h, samples);
		setPause(h, aPaused);
		return h;		
	}

	handle Soloud::play3dClocked(time aSoundTime, AudioSource &aSound, float aPosX, float aPosY, float aPosZ, float aVelX, float aVelY, float aVelZ, float aVolume, unsigned int aBus)
	{
		handle h = play(aSound, aVolume, 0, 1, aBus);
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int v = getVoiceFromHandle(h);
		if (v < 0) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return h;
		}
		mVoice[v]->mFlags |= AudioSourceInstance::PROCESS_3D;
		time lasttime = mLastClockedTime;
		if (lasttime == 0) 
			mLastClockedTime = aSoundTime;
		update3dVoice(v);
		vec3 pos;
		pos.mX = aPosX;
		pos.mY = aPosY;
		pos.mZ = aPosZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		int samples = 0;
		if (lasttime != 0)
		{
			samples = (int)floor((aSoundTime - lasttime) * mSamplerate);
		}
		if (aSound.mFlags & AudioSource::DISTANCE_DELAY)
		{
			float dist = pos.mag();
			samples += (int)floor((dist / m3dSoundSpeed) * mSamplerate);
		}
		setDelaySamples(h, samples);
		setPause(h, 0);
		return h;
	}


	
	result Soloud::set3dSoundSpeed(float aSpeed)
	{
		if (aSpeed <= 0)
			return INVALID_PARAMETER;
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		m3dSoundSpeed = aSpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return SO_NO_ERROR;
	}

	
	float Soloud::get3dSoundSpeed()
	{
		return m3dSoundSpeed;
	}

	
	void Soloud::set3dListenerParameters(float aPosX, float aPosY, float aPosZ, float aAtX, float aAtY, float aAtZ, float aUpX, float aUpY, float aUpZ, float aVelocityX, float aVelocityY, float aVelocityZ)
	{
		if (mLockMutexFunc) mUnlockMutexFunc(mMutex);
		m3dPosition[0] = aPosX;
		m3dPosition[1] = aPosY;
		m3dPosition[2] = aPosZ;
		m3dAt[0] = aAtX;
		m3dAt[1] = aAtY;
		m3dAt[2] = aAtZ;
		m3dUp[0] = aUpX;
		m3dUp[1] = aUpY;
		m3dUp[2] = aUpZ;
		m3dVelocity[0] = aVelocityX;
		m3dVelocity[1] = aVelocityY;
		m3dVelocity[2] = aVelocityZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	
	void Soloud::set3dListenerPosition(float aPosX, float aPosY, float aPosZ)
	{
		if (mLockMutexFunc) mUnlockMutexFunc(mMutex);
		m3dPosition[0] = aPosX;
		m3dPosition[1] = aPosY;
		m3dPosition[2] = aPosZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	
	void Soloud::set3dListenerAt(float aAtX, float aAtY, float aAtZ)
	{
		if (mLockMutexFunc) mUnlockMutexFunc(mMutex);
		m3dAt[0] = aAtX;
		m3dAt[1] = aAtY;
		m3dAt[2] = aAtZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	
	void Soloud::set3dListenerUp(float aUpX, float aUpY, float aUpZ)
	{
		if (mLockMutexFunc) mUnlockMutexFunc(mMutex);
		m3dUp[0] = aUpX;
		m3dUp[1] = aUpY;
		m3dUp[2] = aUpZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	
	void Soloud::set3dListenerVelocity(float aVelocityX, float aVelocityY, float aVelocityZ)
	{
		if (mLockMutexFunc) mUnlockMutexFunc(mMutex);
		m3dVelocity[0] = aVelocityX;
		m3dVelocity[1] = aVelocityY;
		m3dVelocity[2] = aVelocityZ;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	
	void Soloud::set3dSourceParameters(handle aVoiceHandle, float aPosX, float aPosY, float aPosZ, float aVelocityX, float aVelocityY, float aVelocityZ)
	{
		FOR_ALL_VOICES_PRE
		mVoice[ch]->m3dPosition[0] = aPosX;
		mVoice[ch]->m3dPosition[1] = aPosY;
		mVoice[ch]->m3dPosition[2] = aPosZ;
		mVoice[ch]->m3dVelocity[0] = aVelocityX;
		mVoice[ch]->m3dVelocity[1] = aVelocityY;
		mVoice[ch]->m3dVelocity[2] = aVelocityZ;
		FOR_ALL_VOICES_POST
	}

	
	void Soloud::set3dSourcePosition(handle aVoiceHandle, float aPosX, float aPosY, float aPosZ)
	{
		FOR_ALL_VOICES_PRE
		mVoice[ch]->m3dPosition[0] = aPosX;
		mVoice[ch]->m3dPosition[1] = aPosY;
		mVoice[ch]->m3dPosition[2] = aPosZ;
		FOR_ALL_VOICES_POST
	}

	
	void Soloud::set3dSourceVelocity(handle aVoiceHandle, float aVelocityX, float aVelocityY, float aVelocityZ)
	{
		FOR_ALL_VOICES_PRE
		mVoice[ch]->m3dVelocity[0] = aVelocityX;
		mVoice[ch]->m3dVelocity[1] = aVelocityY;
		mVoice[ch]->m3dVelocity[2] = aVelocityZ;
		FOR_ALL_VOICES_POST
	}

	
	void Soloud::set3dSourceMinMaxDistance(handle aVoiceHandle, float aMinDistance, float aMaxDistance)
	{
		FOR_ALL_VOICES_PRE
		mVoice[ch]->m3dMinDistance = aMinDistance;
		mVoice[ch]->m3dMaxDistance = aMaxDistance;
		FOR_ALL_VOICES_POST
	}

	
	void Soloud::set3dSourceAttenuation(handle aVoiceHandle, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
	{
		FOR_ALL_VOICES_PRE
		mVoice[ch]->m3dAttenuationModel = aAttenuationModel;
		mVoice[ch]->m3dAttenuationRolloff = aAttenuationRolloffFactor;
		FOR_ALL_VOICES_POST
	}

	
	void Soloud::set3dSourceDopplerFactor(handle aVoiceHandle, float aDopplerFactor)
	{
		FOR_ALL_VOICES_PRE
		mVoice[ch]->m3dDopplerFactor = aDopplerFactor;
		FOR_ALL_VOICES_POST
	}
};
