/* **************************************************
 *  WARNING: this is a generated file. Do not edit. *
 *  Any edits will be overwritten by the generator. *
 ************************************************** */

/*
SoLoud audio engine
Copyright (c) 2013-2016 Jari Komppa

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

/* SoLoud C-Api Code Generator (c)2013-2015 Jari Komppa http://iki.fi/sol/ */

#include "../include/soloud.h"
#include "../include/soloud_audiosource.h"
#include "../include/soloud_biquadresonantfilter.h"
#include "../include/soloud_lofifilter.h"
#include "../include/soloud_bus.h"
#include "../include/soloud_echofilter.h"
#include "../include/soloud_fader.h"
#include "../include/soloud_fftfilter.h"
#include "../include/soloud_bassboostfilter.h"
#include "../include/soloud_filter.h"
#include "../include/soloud_speech.h"
#include "../include/soloud_wav.h"
#include "../include/soloud_wavstream.h"
#include "../include/soloud_sfxr.h"
#include "../include/soloud_flangerfilter.h"
#include "../include/soloud_dcremovalfilter.h"
#include "../include/soloud_modplug.h"
#include "../include/soloud_monotone.h"
#include "../include/soloud_tedsid.h"

using namespace SoLoud;

extern "C"
{

void Soloud_destroy(void * aClassPtr)
{
  delete (Soloud *)aClassPtr;
}

void * Soloud_create()
{
  return (void *)new Soloud;
}

int Soloud_init(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->init();
}

int Soloud_initEx(void * aClassPtr, unsigned int aFlags, unsigned int aBackend, unsigned int aSamplerate, unsigned int aBufferSize, unsigned int aChannels)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->init(aFlags, aBackend, aSamplerate, aBufferSize, aChannels);
}

void Soloud_deinit(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->deinit();
}

unsigned int Soloud_getVersion(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getVersion();
}

const char * Soloud_getErrorString(void * aClassPtr, int aErrorCode)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getErrorString(aErrorCode);
}

unsigned int Soloud_getBackendId(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getBackendId();
}

const char * Soloud_getBackendString(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getBackendString();
}

unsigned int Soloud_getBackendChannels(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getBackendChannels();
}

unsigned int Soloud_getBackendSamplerate(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getBackendSamplerate();
}

unsigned int Soloud_getBackendBufferSize(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getBackendBufferSize();
}

int Soloud_setSpeakerPosition(void * aClassPtr, unsigned int aChannel, float aX, float aY, float aZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->setSpeakerPosition(aChannel, aX, aY, aZ);
}

unsigned int Soloud_play(void * aClassPtr, AudioSource * aSound)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play(*aSound);
}

unsigned int Soloud_playEx(void * aClassPtr, AudioSource * aSound, float aVolume, float aPan, int aPaused, unsigned int aBus)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play(*aSound, aVolume, aPan, !!aPaused, aBus);
}

unsigned int Soloud_playClocked(void * aClassPtr, double aSoundTime, AudioSource * aSound)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->playClocked(aSoundTime, *aSound);
}

unsigned int Soloud_playClockedEx(void * aClassPtr, double aSoundTime, AudioSource * aSound, float aVolume, float aPan, unsigned int aBus)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->playClocked(aSoundTime, *aSound, aVolume, aPan, aBus);
}

unsigned int Soloud_play3d(void * aClassPtr, AudioSource * aSound, float aPosX, float aPosY, float aPosZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play3d(*aSound, aPosX, aPosY, aPosZ);
}

unsigned int Soloud_play3dEx(void * aClassPtr, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX, float aVelY, float aVelZ, float aVolume, int aPaused, unsigned int aBus)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play3d(*aSound, aPosX, aPosY, aPosZ, aVelX, aVelY, aVelZ, aVolume, !!aPaused, aBus);
}

unsigned int Soloud_play3dClocked(void * aClassPtr, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play3dClocked(aSoundTime, *aSound, aPosX, aPosY, aPosZ);
}

unsigned int Soloud_play3dClockedEx(void * aClassPtr, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX, float aVelY, float aVelZ, float aVolume, unsigned int aBus)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play3dClocked(aSoundTime, *aSound, aPosX, aPosY, aPosZ, aVelX, aVelY, aVelZ, aVolume, aBus);
}

void Soloud_seek(void * aClassPtr, unsigned int aVoiceHandle, double aSeconds)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->seek(aVoiceHandle, aSeconds);
}

void Soloud_stop(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->stop(aVoiceHandle);
}

void Soloud_stopAll(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->stopAll();
}

void Soloud_stopAudioSource(void * aClassPtr, AudioSource * aSound)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->stopAudioSource(*aSound);
}

void Soloud_setFilterParameter(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId, float aValue)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setFilterParameter(aVoiceHandle, aFilterId, aAttributeId, aValue);
}

float Soloud_getFilterParameter(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getFilterParameter(aVoiceHandle, aFilterId, aAttributeId);
}

void Soloud_fadeFilterParameter(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeFilterParameter(aVoiceHandle, aFilterId, aAttributeId, aTo, aTime);
}

void Soloud_oscillateFilterParameter(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId, float aFrom, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateFilterParameter(aVoiceHandle, aFilterId, aAttributeId, aFrom, aTo, aTime);
}

double Soloud_getStreamTime(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getStreamTime(aVoiceHandle);
}

int Soloud_getPause(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getPause(aVoiceHandle);
}

float Soloud_getVolume(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getVolume(aVoiceHandle);
}

float Soloud_getOverallVolume(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getOverallVolume(aVoiceHandle);
}

float Soloud_getPan(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getPan(aVoiceHandle);
}

float Soloud_getSamplerate(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getSamplerate(aVoiceHandle);
}

int Soloud_getProtectVoice(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getProtectVoice(aVoiceHandle);
}

unsigned int Soloud_getActiveVoiceCount(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getActiveVoiceCount();
}

unsigned int Soloud_getVoiceCount(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getVoiceCount();
}

int Soloud_isValidVoiceHandle(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->isValidVoiceHandle(aVoiceHandle);
}

float Soloud_getRelativePlaySpeed(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getRelativePlaySpeed(aVoiceHandle);
}

float Soloud_getPostClipScaler(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getPostClipScaler();
}

float Soloud_getGlobalVolume(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getGlobalVolume();
}

unsigned int Soloud_getMaxActiveVoiceCount(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getMaxActiveVoiceCount();
}

int Soloud_getLooping(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getLooping(aVoiceHandle);
}

void Soloud_setLooping(void * aClassPtr, unsigned int aVoiceHandle, int aLooping)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setLooping(aVoiceHandle, !!aLooping);
}

int Soloud_setMaxActiveVoiceCount(void * aClassPtr, unsigned int aVoiceCount)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->setMaxActiveVoiceCount(aVoiceCount);
}

void Soloud_setInaudibleBehavior(void * aClassPtr, unsigned int aVoiceHandle, int aMustTick, int aKill)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setInaudibleBehavior(aVoiceHandle, !!aMustTick, !!aKill);
}

void Soloud_setGlobalVolume(void * aClassPtr, float aVolume)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setGlobalVolume(aVolume);
}

void Soloud_setPostClipScaler(void * aClassPtr, float aScaler)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPostClipScaler(aScaler);
}

void Soloud_setPause(void * aClassPtr, unsigned int aVoiceHandle, int aPause)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPause(aVoiceHandle, !!aPause);
}

void Soloud_setPauseAll(void * aClassPtr, int aPause)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPauseAll(!!aPause);
}

int Soloud_setRelativePlaySpeed(void * aClassPtr, unsigned int aVoiceHandle, float aSpeed)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->setRelativePlaySpeed(aVoiceHandle, aSpeed);
}

void Soloud_setProtectVoice(void * aClassPtr, unsigned int aVoiceHandle, int aProtect)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setProtectVoice(aVoiceHandle, !!aProtect);
}

void Soloud_setSamplerate(void * aClassPtr, unsigned int aVoiceHandle, float aSamplerate)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setSamplerate(aVoiceHandle, aSamplerate);
}

void Soloud_setPan(void * aClassPtr, unsigned int aVoiceHandle, float aPan)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPan(aVoiceHandle, aPan);
}

void Soloud_setPanAbsolute(void * aClassPtr, unsigned int aVoiceHandle, float aLVolume, float aRVolume)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPanAbsolute(aVoiceHandle, aLVolume, aRVolume);
}

void Soloud_setPanAbsoluteEx(void * aClassPtr, unsigned int aVoiceHandle, float aLVolume, float aRVolume, float aLBVolume, float aRBVolume, float aCVolume, float aSVolume)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPanAbsolute(aVoiceHandle, aLVolume, aRVolume, aLBVolume, aRBVolume, aCVolume, aSVolume);
}

void Soloud_setVolume(void * aClassPtr, unsigned int aVoiceHandle, float aVolume)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setVolume(aVoiceHandle, aVolume);
}

void Soloud_setDelaySamples(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aSamples)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setDelaySamples(aVoiceHandle, aSamples);
}

void Soloud_fadeVolume(void * aClassPtr, unsigned int aVoiceHandle, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeVolume(aVoiceHandle, aTo, aTime);
}

void Soloud_fadePan(void * aClassPtr, unsigned int aVoiceHandle, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadePan(aVoiceHandle, aTo, aTime);
}

void Soloud_fadeRelativePlaySpeed(void * aClassPtr, unsigned int aVoiceHandle, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeRelativePlaySpeed(aVoiceHandle, aTo, aTime);
}

void Soloud_fadeGlobalVolume(void * aClassPtr, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeGlobalVolume(aTo, aTime);
}

void Soloud_schedulePause(void * aClassPtr, unsigned int aVoiceHandle, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->schedulePause(aVoiceHandle, aTime);
}

void Soloud_scheduleStop(void * aClassPtr, unsigned int aVoiceHandle, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->scheduleStop(aVoiceHandle, aTime);
}

void Soloud_oscillateVolume(void * aClassPtr, unsigned int aVoiceHandle, float aFrom, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateVolume(aVoiceHandle, aFrom, aTo, aTime);
}

void Soloud_oscillatePan(void * aClassPtr, unsigned int aVoiceHandle, float aFrom, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillatePan(aVoiceHandle, aFrom, aTo, aTime);
}

void Soloud_oscillateRelativePlaySpeed(void * aClassPtr, unsigned int aVoiceHandle, float aFrom, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateRelativePlaySpeed(aVoiceHandle, aFrom, aTo, aTime);
}

void Soloud_oscillateGlobalVolume(void * aClassPtr, float aFrom, float aTo, double aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateGlobalVolume(aFrom, aTo, aTime);
}

void Soloud_setGlobalFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setGlobalFilter(aFilterId, aFilter);
}

void Soloud_setVisualizationEnable(void * aClassPtr, int aEnable)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setVisualizationEnable(!!aEnable);
}

float * Soloud_calcFFT(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->calcFFT();
}

float * Soloud_getWave(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getWave();
}

unsigned int Soloud_getLoopCount(void * aClassPtr, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getLoopCount(aVoiceHandle);
}

float Soloud_getInfo(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aInfoKey)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getInfo(aVoiceHandle, aInfoKey);
}

unsigned int Soloud_createVoiceGroup(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->createVoiceGroup();
}

int Soloud_destroyVoiceGroup(void * aClassPtr, unsigned int aVoiceGroupHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->destroyVoiceGroup(aVoiceGroupHandle);
}

int Soloud_addVoiceToGroup(void * aClassPtr, unsigned int aVoiceGroupHandle, unsigned int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->addVoiceToGroup(aVoiceGroupHandle, aVoiceHandle);
}

int Soloud_isVoiceGroup(void * aClassPtr, unsigned int aVoiceGroupHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->isVoiceGroup(aVoiceGroupHandle);
}

int Soloud_isVoiceGroupEmpty(void * aClassPtr, unsigned int aVoiceGroupHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->isVoiceGroupEmpty(aVoiceGroupHandle);
}

void Soloud_update3dAudio(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->update3dAudio();
}

int Soloud_set3dSoundSpeed(void * aClassPtr, float aSpeed)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->set3dSoundSpeed(aSpeed);
}

float Soloud_get3dSoundSpeed(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->get3dSoundSpeed();
}

void Soloud_set3dListenerParameters(void * aClassPtr, float aPosX, float aPosY, float aPosZ, float aAtX, float aAtY, float aAtZ, float aUpX, float aUpY, float aUpZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dListenerParameters(aPosX, aPosY, aPosZ, aAtX, aAtY, aAtZ, aUpX, aUpY, aUpZ);
}

void Soloud_set3dListenerParametersEx(void * aClassPtr, float aPosX, float aPosY, float aPosZ, float aAtX, float aAtY, float aAtZ, float aUpX, float aUpY, float aUpZ, float aVelocityX, float aVelocityY, float aVelocityZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dListenerParameters(aPosX, aPosY, aPosZ, aAtX, aAtY, aAtZ, aUpX, aUpY, aUpZ, aVelocityX, aVelocityY, aVelocityZ);
}

void Soloud_set3dListenerPosition(void * aClassPtr, float aPosX, float aPosY, float aPosZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dListenerPosition(aPosX, aPosY, aPosZ);
}

void Soloud_set3dListenerAt(void * aClassPtr, float aAtX, float aAtY, float aAtZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dListenerAt(aAtX, aAtY, aAtZ);
}

void Soloud_set3dListenerUp(void * aClassPtr, float aUpX, float aUpY, float aUpZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dListenerUp(aUpX, aUpY, aUpZ);
}

void Soloud_set3dListenerVelocity(void * aClassPtr, float aVelocityX, float aVelocityY, float aVelocityZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dListenerVelocity(aVelocityX, aVelocityY, aVelocityZ);
}

void Soloud_set3dSourceParameters(void * aClassPtr, unsigned int aVoiceHandle, float aPosX, float aPosY, float aPosZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourceParameters(aVoiceHandle, aPosX, aPosY, aPosZ);
}

void Soloud_set3dSourceParametersEx(void * aClassPtr, unsigned int aVoiceHandle, float aPosX, float aPosY, float aPosZ, float aVelocityX, float aVelocityY, float aVelocityZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourceParameters(aVoiceHandle, aPosX, aPosY, aPosZ, aVelocityX, aVelocityY, aVelocityZ);
}

void Soloud_set3dSourcePosition(void * aClassPtr, unsigned int aVoiceHandle, float aPosX, float aPosY, float aPosZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourcePosition(aVoiceHandle, aPosX, aPosY, aPosZ);
}

void Soloud_set3dSourceVelocity(void * aClassPtr, unsigned int aVoiceHandle, float aVelocityX, float aVelocityY, float aVelocityZ)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourceVelocity(aVoiceHandle, aVelocityX, aVelocityY, aVelocityZ);
}

void Soloud_set3dSourceMinMaxDistance(void * aClassPtr, unsigned int aVoiceHandle, float aMinDistance, float aMaxDistance)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourceMinMaxDistance(aVoiceHandle, aMinDistance, aMaxDistance);
}

void Soloud_set3dSourceAttenuation(void * aClassPtr, unsigned int aVoiceHandle, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourceAttenuation(aVoiceHandle, aAttenuationModel, aAttenuationRolloffFactor);
}

void Soloud_set3dSourceDopplerFactor(void * aClassPtr, unsigned int aVoiceHandle, float aDopplerFactor)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->set3dSourceDopplerFactor(aVoiceHandle, aDopplerFactor);
}

void Soloud_mix(void * aClassPtr, float * aBuffer, unsigned int aSamples)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->mix(aBuffer, aSamples);
}

void Soloud_mixSigned16(void * aClassPtr, short * aBuffer, unsigned int aSamples)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->mixSigned16(aBuffer, aSamples);
}

void AudioAttenuator_destroy(void * aClassPtr)
{
  delete (AudioAttenuator *)aClassPtr;
}

float AudioAttenuator_attenuate(void * aClassPtr, float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor)
{
	AudioAttenuator * cl = (AudioAttenuator *)aClassPtr;
	return cl->attenuate(aDistance, aMinDistance, aMaxDistance, aRolloffFactor);
}

void BiquadResonantFilter_destroy(void * aClassPtr)
{
  delete (BiquadResonantFilter *)aClassPtr;
}

void * BiquadResonantFilter_create()
{
  return (void *)new BiquadResonantFilter;
}

int BiquadResonantFilter_setParams(void * aClassPtr, int aType, float aSampleRate, float aFrequency, float aResonance)
{
	BiquadResonantFilter * cl = (BiquadResonantFilter *)aClassPtr;
	return cl->setParams(aType, aSampleRate, aFrequency, aResonance);
}

void LofiFilter_destroy(void * aClassPtr)
{
  delete (LofiFilter *)aClassPtr;
}

void * LofiFilter_create()
{
  return (void *)new LofiFilter;
}

int LofiFilter_setParams(void * aClassPtr, float aSampleRate, float aBitdepth)
{
	LofiFilter * cl = (LofiFilter *)aClassPtr;
	return cl->setParams(aSampleRate, aBitdepth);
}

void Bus_destroy(void * aClassPtr)
{
  delete (Bus *)aClassPtr;
}

void * Bus_create()
{
  return (void *)new Bus;
}

void Bus_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

unsigned int Bus_play(void * aClassPtr, AudioSource * aSound)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play(*aSound);
}

unsigned int Bus_playEx(void * aClassPtr, AudioSource * aSound, float aVolume, float aPan, int aPaused)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play(*aSound, aVolume, aPan, !!aPaused);
}

unsigned int Bus_playClocked(void * aClassPtr, double aSoundTime, AudioSource * aSound)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->playClocked(aSoundTime, *aSound);
}

unsigned int Bus_playClockedEx(void * aClassPtr, double aSoundTime, AudioSource * aSound, float aVolume, float aPan)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->playClocked(aSoundTime, *aSound, aVolume, aPan);
}

unsigned int Bus_play3d(void * aClassPtr, AudioSource * aSound, float aPosX, float aPosY, float aPosZ)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play3d(*aSound, aPosX, aPosY, aPosZ);
}

unsigned int Bus_play3dEx(void * aClassPtr, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX, float aVelY, float aVelZ, float aVolume, int aPaused)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play3d(*aSound, aPosX, aPosY, aPosZ, aVelX, aVelY, aVelZ, aVolume, !!aPaused);
}

unsigned int Bus_play3dClocked(void * aClassPtr, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play3dClocked(aSoundTime, *aSound, aPosX, aPosY, aPosZ);
}

unsigned int Bus_play3dClockedEx(void * aClassPtr, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX, float aVelY, float aVelZ, float aVolume)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play3dClocked(aSoundTime, *aSound, aPosX, aPosY, aPosZ, aVelX, aVelY, aVelZ, aVolume);
}

int Bus_setChannels(void * aClassPtr, unsigned int aChannels)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->setChannels(aChannels);
}

void Bus_setVisualizationEnable(void * aClassPtr, int aEnable)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setVisualizationEnable(!!aEnable);
}

float * Bus_calcFFT(void * aClassPtr)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->calcFFT();
}

float * Bus_getWave(void * aClassPtr)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->getWave();
}

void Bus_setVolume(void * aClassPtr, float aVolume)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setVolume(aVolume);
}

void Bus_setLooping(void * aClassPtr, int aLoop)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void Bus_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void Bus_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void Bus_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void Bus_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void Bus_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void Bus_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void Bus_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void Bus_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void Bus_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void Bus_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void Bus_stop(void * aClassPtr)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->stop();
}

void EchoFilter_destroy(void * aClassPtr)
{
  delete (EchoFilter *)aClassPtr;
}

void * EchoFilter_create()
{
  return (void *)new EchoFilter;
}

int EchoFilter_setParams(void * aClassPtr, float aDelay)
{
	EchoFilter * cl = (EchoFilter *)aClassPtr;
	return cl->setParams(aDelay);
}

int EchoFilter_setParamsEx(void * aClassPtr, float aDelay, float aDecay, float aFilter)
{
	EchoFilter * cl = (EchoFilter *)aClassPtr;
	return cl->setParams(aDelay, aDecay, aFilter);
}

void FFTFilter_destroy(void * aClassPtr)
{
  delete (FFTFilter *)aClassPtr;
}

void * FFTFilter_create()
{
  return (void *)new FFTFilter;
}

void BassboostFilter_destroy(void * aClassPtr)
{
  delete (BassboostFilter *)aClassPtr;
}

int BassboostFilter_setParams(void * aClassPtr, float aBoost)
{
	BassboostFilter * cl = (BassboostFilter *)aClassPtr;
	return cl->setParams(aBoost);
}

void * BassboostFilter_create()
{
  return (void *)new BassboostFilter;
}

void Speech_destroy(void * aClassPtr)
{
  delete (Speech *)aClassPtr;
}

void * Speech_create()
{
  return (void *)new Speech;
}

int Speech_setText(void * aClassPtr, const char * aText)
{
	Speech * cl = (Speech *)aClassPtr;
	return cl->setText(aText);
}

void Speech_setVolume(void * aClassPtr, float aVolume)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setVolume(aVolume);
}

void Speech_setLooping(void * aClassPtr, int aLoop)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void Speech_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void Speech_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void Speech_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void Speech_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void Speech_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void Speech_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void Speech_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void Speech_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void Speech_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void Speech_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void Speech_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void Speech_stop(void * aClassPtr)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->stop();
}

void Wav_destroy(void * aClassPtr)
{
  delete (Wav *)aClassPtr;
}

void * Wav_create()
{
  return (void *)new Wav;
}

int Wav_load(void * aClassPtr, const char * aFilename)
{
	Wav * cl = (Wav *)aClassPtr;
	return cl->load(aFilename);
}

int Wav_loadMem(void * aClassPtr, unsigned char * aMem, unsigned int aLength)
{
	Wav * cl = (Wav *)aClassPtr;
	return cl->loadMem(aMem, aLength);
}

int Wav_loadMemEx(void * aClassPtr, unsigned char * aMem, unsigned int aLength, int aCopy, int aTakeOwnership)
{
	Wav * cl = (Wav *)aClassPtr;
	return cl->loadMem(aMem, aLength, !!aCopy, !!aTakeOwnership);
}

int Wav_loadFile(void * aClassPtr, File * aFile)
{
	Wav * cl = (Wav *)aClassPtr;
	return cl->loadFile(aFile);
}

double Wav_getLength(void * aClassPtr)
{
	Wav * cl = (Wav *)aClassPtr;
	return cl->getLength();
}

void Wav_setVolume(void * aClassPtr, float aVolume)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setVolume(aVolume);
}

void Wav_setLooping(void * aClassPtr, int aLoop)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void Wav_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void Wav_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void Wav_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void Wav_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void Wav_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void Wav_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void Wav_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void Wav_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void Wav_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void Wav_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void Wav_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void Wav_stop(void * aClassPtr)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->stop();
}

void WavStream_destroy(void * aClassPtr)
{
  delete (WavStream *)aClassPtr;
}

void * WavStream_create()
{
  return (void *)new WavStream;
}

int WavStream_load(void * aClassPtr, const char * aFilename)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->load(aFilename);
}

int WavStream_loadMem(void * aClassPtr, unsigned char * aData, unsigned int aDataLen)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->loadMem(aData, aDataLen);
}

int WavStream_loadMemEx(void * aClassPtr, unsigned char * aData, unsigned int aDataLen, int aCopy, int aTakeOwnership)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->loadMem(aData, aDataLen, !!aCopy, !!aTakeOwnership);
}

int WavStream_loadToMem(void * aClassPtr, const char * aFilename)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->loadToMem(aFilename);
}

int WavStream_loadFile(void * aClassPtr, File * aFile)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->loadFile(aFile);
}

int WavStream_loadFileToMem(void * aClassPtr, File * aFile)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->loadFileToMem(aFile);
}

double WavStream_getLength(void * aClassPtr)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->getLength();
}

void WavStream_setVolume(void * aClassPtr, float aVolume)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setVolume(aVolume);
}

void WavStream_setLooping(void * aClassPtr, int aLoop)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void WavStream_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void WavStream_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void WavStream_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void WavStream_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void WavStream_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void WavStream_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void WavStream_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void WavStream_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void WavStream_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void WavStream_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void WavStream_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void WavStream_stop(void * aClassPtr)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->stop();
}

void Prg_destroy(void * aClassPtr)
{
  delete (Prg *)aClassPtr;
}

void * Prg_create()
{
  return (void *)new Prg;
}

unsigned int Prg_rand(void * aClassPtr)
{
	Prg * cl = (Prg *)aClassPtr;
	return cl->rand();
}

void Prg_srand(void * aClassPtr, int aSeed)
{
	Prg * cl = (Prg *)aClassPtr;
	cl->srand(aSeed);
}

void Sfxr_destroy(void * aClassPtr)
{
  delete (Sfxr *)aClassPtr;
}

void * Sfxr_create()
{
  return (void *)new Sfxr;
}

void Sfxr_resetParams(void * aClassPtr)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->resetParams();
}

int Sfxr_loadParams(void * aClassPtr, const char * aFilename)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	return cl->loadParams(aFilename);
}

int Sfxr_loadParamsMem(void * aClassPtr, unsigned char * aMem, unsigned int aLength)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	return cl->loadParamsMem(aMem, aLength);
}

int Sfxr_loadParamsMemEx(void * aClassPtr, unsigned char * aMem, unsigned int aLength, int aCopy, int aTakeOwnership)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	return cl->loadParamsMem(aMem, aLength, !!aCopy, !!aTakeOwnership);
}

int Sfxr_loadParamsFile(void * aClassPtr, File * aFile)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	return cl->loadParamsFile(aFile);
}

int Sfxr_loadPreset(void * aClassPtr, int aPresetNo, int aRandSeed)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	return cl->loadPreset(aPresetNo, aRandSeed);
}

void Sfxr_setVolume(void * aClassPtr, float aVolume)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->setVolume(aVolume);
}

void Sfxr_setLooping(void * aClassPtr, int aLoop)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void Sfxr_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void Sfxr_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void Sfxr_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void Sfxr_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void Sfxr_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void Sfxr_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void Sfxr_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void Sfxr_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void Sfxr_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void Sfxr_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void Sfxr_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void Sfxr_stop(void * aClassPtr)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->stop();
}

void FlangerFilter_destroy(void * aClassPtr)
{
  delete (FlangerFilter *)aClassPtr;
}

void * FlangerFilter_create()
{
  return (void *)new FlangerFilter;
}

int FlangerFilter_setParams(void * aClassPtr, float aDelay, float aFreq)
{
	FlangerFilter * cl = (FlangerFilter *)aClassPtr;
	return cl->setParams(aDelay, aFreq);
}

void DCRemovalFilter_destroy(void * aClassPtr)
{
  delete (DCRemovalFilter *)aClassPtr;
}

void * DCRemovalFilter_create()
{
  return (void *)new DCRemovalFilter;
}

int DCRemovalFilter_setParams(void * aClassPtr)
{
	DCRemovalFilter * cl = (DCRemovalFilter *)aClassPtr;
	return cl->setParams();
}

int DCRemovalFilter_setParamsEx(void * aClassPtr, float aLength)
{
	DCRemovalFilter * cl = (DCRemovalFilter *)aClassPtr;
	return cl->setParams(aLength);
}

void Modplug_destroy(void * aClassPtr)
{
  delete (Modplug *)aClassPtr;
}

void * Modplug_create()
{
  return (void *)new Modplug;
}

int Modplug_load(void * aClassPtr, const char * aFilename)
{
	Modplug * cl = (Modplug *)aClassPtr;
	return cl->load(aFilename);
}

int Modplug_loadMem(void * aClassPtr, unsigned char * aMem, unsigned int aLength)
{
	Modplug * cl = (Modplug *)aClassPtr;
	return cl->loadMem(aMem, aLength);
}

int Modplug_loadMemEx(void * aClassPtr, unsigned char * aMem, unsigned int aLength, int aCopy, int aTakeOwnership)
{
	Modplug * cl = (Modplug *)aClassPtr;
	return cl->loadMem(aMem, aLength, !!aCopy, !!aTakeOwnership);
}

int Modplug_loadFile(void * aClassPtr, File * aFile)
{
	Modplug * cl = (Modplug *)aClassPtr;
	return cl->loadFile(aFile);
}

void Modplug_setVolume(void * aClassPtr, float aVolume)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->setVolume(aVolume);
}

void Modplug_setLooping(void * aClassPtr, int aLoop)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void Modplug_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void Modplug_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void Modplug_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void Modplug_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void Modplug_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void Modplug_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void Modplug_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void Modplug_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void Modplug_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void Modplug_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void Modplug_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void Modplug_stop(void * aClassPtr)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->stop();
}

void Monotone_destroy(void * aClassPtr)
{
  delete (Monotone *)aClassPtr;
}

void * Monotone_create()
{
  return (void *)new Monotone;
}

int Monotone_setParams(void * aClassPtr, int aHardwareChannels)
{
	Monotone * cl = (Monotone *)aClassPtr;
	return cl->setParams(aHardwareChannels);
}

int Monotone_setParamsEx(void * aClassPtr, int aHardwareChannels, int aWaveform)
{
	Monotone * cl = (Monotone *)aClassPtr;
	return cl->setParams(aHardwareChannels, aWaveform);
}

int Monotone_load(void * aClassPtr, const char * aFilename)
{
	Monotone * cl = (Monotone *)aClassPtr;
	return cl->load(aFilename);
}

int Monotone_loadMem(void * aClassPtr, unsigned char * aMem, unsigned int aLength)
{
	Monotone * cl = (Monotone *)aClassPtr;
	return cl->loadMem(aMem, aLength);
}

int Monotone_loadMemEx(void * aClassPtr, unsigned char * aMem, unsigned int aLength, int aCopy, int aTakeOwnership)
{
	Monotone * cl = (Monotone *)aClassPtr;
	return cl->loadMem(aMem, aLength, !!aCopy, !!aTakeOwnership);
}

int Monotone_loadFile(void * aClassPtr, File * aFile)
{
	Monotone * cl = (Monotone *)aClassPtr;
	return cl->loadFile(aFile);
}

void Monotone_setVolume(void * aClassPtr, float aVolume)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->setVolume(aVolume);
}

void Monotone_setLooping(void * aClassPtr, int aLoop)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void Monotone_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void Monotone_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void Monotone_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void Monotone_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void Monotone_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void Monotone_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void Monotone_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void Monotone_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void Monotone_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void Monotone_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void Monotone_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void Monotone_stop(void * aClassPtr)
{
	Monotone * cl = (Monotone *)aClassPtr;
	cl->stop();
}

void TedSid_destroy(void * aClassPtr)
{
  delete (TedSid *)aClassPtr;
}

void * TedSid_create()
{
  return (void *)new TedSid;
}

int TedSid_load(void * aClassPtr, const char * aFilename)
{
	TedSid * cl = (TedSid *)aClassPtr;
	return cl->load(aFilename);
}

int TedSid_loadToMem(void * aClassPtr, const char * aFilename)
{
	TedSid * cl = (TedSid *)aClassPtr;
	return cl->loadToMem(aFilename);
}

int TedSid_loadMem(void * aClassPtr, unsigned char * aMem, unsigned int aLength)
{
	TedSid * cl = (TedSid *)aClassPtr;
	return cl->loadMem(aMem, aLength);
}

int TedSid_loadMemEx(void * aClassPtr, unsigned char * aMem, unsigned int aLength, int aCopy, int aTakeOwnership)
{
	TedSid * cl = (TedSid *)aClassPtr;
	return cl->loadMem(aMem, aLength, !!aCopy, !!aTakeOwnership);
}

int TedSid_loadFileToMem(void * aClassPtr, File * aFile)
{
	TedSid * cl = (TedSid *)aClassPtr;
	return cl->loadFileToMem(aFile);
}

int TedSid_loadFile(void * aClassPtr, File * aFile)
{
	TedSid * cl = (TedSid *)aClassPtr;
	return cl->loadFile(aFile);
}

void TedSid_setVolume(void * aClassPtr, float aVolume)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->setVolume(aVolume);
}

void TedSid_setLooping(void * aClassPtr, int aLoop)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->setLooping(!!aLoop);
}

void TedSid_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void TedSid_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void TedSid_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dDopplerFactor(aDopplerFactor);
}

void TedSid_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dProcessing(!!aDo3dProcessing);
}

void TedSid_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dListenerRelative(!!aListenerRelative);
}

void TedSid_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dDistanceDelay(!!aDistanceDelay);
}

void TedSid_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dCollider(aCollider);
}

void TedSid_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dCollider(aCollider, aUserData);
}

void TedSid_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->set3dAttenuator(aAttenuator);
}

void TedSid_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void TedSid_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void TedSid_stop(void * aClassPtr)
{
	TedSid * cl = (TedSid *)aClassPtr;
	cl->stop();
}

} // extern "C"

