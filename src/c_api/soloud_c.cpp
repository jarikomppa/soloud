/* **************************************************
 *  WARNING: this is a generated file. Do not edit. *
 *  Any edits will be overwritten by the generator. *
 ************************************************** */

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

/* SoLoud C-Api Code Generator (c)2013 Jari Komppa http://iki.fi/sol/ */

#include "../include/soloud.h"
#include "../include/soloud_audiosource.h"
#include "../include/soloud_biquadresonantfilter.h"
#include "../include/soloud_bus.h"
#include "../include/soloud_echofilter.h"
#include "../include/soloud_fader.h"
#include "../include/soloud_fftfilter.h"
#include "../include/soloud_filter.h"
#include "../include/soloud_speech.h"
#include "../include/soloud_thread.h"
#include "../include/soloud_wav.h"
#include "../include/soloud_wavstream.h"

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

void Soloud_deinit(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->deinit();
}

int Soloud_play(void * aClassPtr, AudioSource * aSound)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play(*aSound);
}

int Soloud_playEx(void * aClassPtr, AudioSource * aSound, float aVolume, float aPan, int aPaused, int aBus)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play(*aSound, aVolume, aPan, aPaused, aBus);
}

void Soloud_seek(void * aClassPtr, int aVoiceHandle, float aSeconds)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->seek(aVoiceHandle, aSeconds);
}

void Soloud_stop(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->stop(aVoiceHandle);
}

void Soloud_stopAll(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->stopAll();
}

void Soloud_stopSound(void * aClassPtr, AudioSource * aSound)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->stopSound(*aSound);
}

void Soloud_setFilterParameter(void * aClassPtr, int aVoiceHandle, int aFilterId, int aAttributeId, float aValue)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setFilterParameter(aVoiceHandle, aFilterId, aAttributeId, aValue);
}

float Soloud_getFilterParameter(void * aClassPtr, int aVoiceHandle, int aFilterId, int aAttributeId)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getFilterParameter(aVoiceHandle, aFilterId, aAttributeId);
}

void Soloud_fadeFilterParameter(void * aClassPtr, int aVoiceHandle, int aFilterId, int aAttributeId, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeFilterParameter(aVoiceHandle, aFilterId, aAttributeId, aTo, aTime);
}

void Soloud_oscillateFilterParameter(void * aClassPtr, int aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateFilterParameter(aVoiceHandle, aFilterId, aAttributeId, aFrom, aTo, aTime);
}

float Soloud_getStreamTime(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getStreamTime(aVoiceHandle);
}

int Soloud_getPause(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getPause(aVoiceHandle);
}

float Soloud_getVolume(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getVolume(aVoiceHandle);
}

float Soloud_getPan(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getPan(aVoiceHandle);
}

float Soloud_getSamplerate(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getSamplerate(aVoiceHandle);
}

int Soloud_getProtectVoice(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getProtectVoice(aVoiceHandle);
}

int Soloud_getActiveVoiceCount(void * aClassPtr)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->getActiveVoiceCount();
}

int Soloud_isValidVoiceHandle(void * aClassPtr, int aVoiceHandle)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->isValidVoiceHandle(aVoiceHandle);
}

float Soloud_getRelativePlaySpeed(void * aClassPtr, int aVoiceHandle)
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

void Soloud_setPause(void * aClassPtr, int aVoiceHandle, int aPause)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPause(aVoiceHandle, aPause);
}

void Soloud_setPauseAll(void * aClassPtr, int aPause)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPauseAll(aPause);
}

void Soloud_setRelativePlaySpeed(void * aClassPtr, int aVoiceHandle, float aSpeed)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setRelativePlaySpeed(aVoiceHandle, aSpeed);
}

void Soloud_setProtectVoice(void * aClassPtr, int aVoiceHandle, int aProtect)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setProtectVoice(aVoiceHandle, aProtect);
}

void Soloud_setSamplerate(void * aClassPtr, int aVoiceHandle, float aSamplerate)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setSamplerate(aVoiceHandle, aSamplerate);
}

void Soloud_setPan(void * aClassPtr, int aVoiceHandle, float aPan)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPan(aVoiceHandle, aPan);
}

void Soloud_setPanAbsolute(void * aClassPtr, int aVoiceHandle, float aLVolume, float aRVolume)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPanAbsolute(aVoiceHandle, aLVolume, aRVolume);
}

void Soloud_setVolume(void * aClassPtr, int aVoiceHandle, float aVolume)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setVolume(aVoiceHandle, aVolume);
}

void Soloud_fadeVolume(void * aClassPtr, int aVoiceHandle, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeVolume(aVoiceHandle, aTo, aTime);
}

void Soloud_fadePan(void * aClassPtr, int aVoiceHandle, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadePan(aVoiceHandle, aTo, aTime);
}

void Soloud_fadeRelativePlaySpeed(void * aClassPtr, int aVoiceHandle, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeRelativePlaySpeed(aVoiceHandle, aTo, aTime);
}

void Soloud_fadeGlobalVolume(void * aClassPtr, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->fadeGlobalVolume(aTo, aTime);
}

void Soloud_schedulePause(void * aClassPtr, int aVoiceHandle, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->schedulePause(aVoiceHandle, aTime);
}

void Soloud_scheduleStop(void * aClassPtr, int aVoiceHandle, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->scheduleStop(aVoiceHandle, aTime);
}

void Soloud_oscillateVolume(void * aClassPtr, int aVoiceHandle, float aFrom, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateVolume(aVoiceHandle, aFrom, aTo, aTime);
}

void Soloud_oscillatePan(void * aClassPtr, int aVoiceHandle, float aFrom, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillatePan(aVoiceHandle, aFrom, aTo, aTime);
}

void Soloud_oscillateRelativePlaySpeed(void * aClassPtr, int aVoiceHandle, float aFrom, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateRelativePlaySpeed(aVoiceHandle, aFrom, aTo, aTime);
}

void Soloud_oscillateGlobalVolume(void * aClassPtr, float aFrom, float aTo, float aTime)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->oscillateGlobalVolume(aFrom, aTo, aTime);
}

void Soloud_setGlobalFilter(void * aClassPtr, int aFilterId, Filter * aFilter)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setGlobalFilter(aFilterId, aFilter);
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

void BiquadResonantFilter_destroy(void * aClassPtr)
{
  delete (BiquadResonantFilter *)aClassPtr;
}

void * BiquadResonantFilter_create()
{
  return (void *)new BiquadResonantFilter;
}

void BiquadResonantFilter_setParams(void * aClassPtr, int aType, float aSampleRate, float aFrequency, float aResonance)
{
	BiquadResonantFilter * cl = (BiquadResonantFilter *)aClassPtr;
	cl->setParams(aType, aSampleRate, aFrequency, aResonance);
}

void Bus_destroy(void * aClassPtr)
{
  delete (Bus *)aClassPtr;
}

void * Bus_create()
{
  return (void *)new Bus;
}

void Bus_setFilter(void * aClassPtr, int aFilterId, Filter * aFilter)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

int Bus_play(void * aClassPtr, AudioSource * aSound)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play(*aSound);
}

int Bus_playEx(void * aClassPtr, AudioSource * aSound, float aVolume, float aPan, int aPaused)
{
	Bus * cl = (Bus *)aClassPtr;
	return cl->play(*aSound, aVolume, aPan, aPaused);
}

void Bus_setLooping(void * aClassPtr, int aLoop)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setLooping(aLoop);
}

void EchoFilter_destroy(void * aClassPtr)
{
  delete (EchoFilter *)aClassPtr;
}

void * EchoFilter_create()
{
  return (void *)new EchoFilter;
}

void EchoFilter_setParams(void * aClassPtr, float aDelay, float aDecay)
{
	EchoFilter * cl = (EchoFilter *)aClassPtr;
	cl->setParams(aDelay, aDecay);
}

void FFTFilter_destroy(void * aClassPtr)
{
  delete (FFTFilter *)aClassPtr;
}

void * FFTFilter_create()
{
  return (void *)new FFTFilter;
}

void FFTFilter_setParameters(void * aClassPtr, int aShift)
{
	FFTFilter * cl = (FFTFilter *)aClassPtr;
	cl->setParameters(aShift);
}

void FFTFilter_setParametersEx(void * aClassPtr, int aShift, int aCombine, float aScale)
{
	FFTFilter * cl = (FFTFilter *)aClassPtr;
	cl->setParameters(aShift, aCombine, aScale);
}

void Speech_destroy(void * aClassPtr)
{
  delete (Speech *)aClassPtr;
}

void * Speech_create()
{
  return (void *)new Speech;
}

void Speech_setText(void * aClassPtr, char * aText)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setText(aText);
}

void Speech_setLooping(void * aClassPtr, int aLoop)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setLooping(aLoop);
}

void Speech_setFilter(void * aClassPtr, int aFilterId, Filter * aFilter)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void Wav_destroy(void * aClassPtr)
{
  delete (Wav *)aClassPtr;
}

void * Wav_create()
{
  return (void *)new Wav;
}

void Wav_load(void * aClassPtr, const char * aFilename)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->load(aFilename);
}

void Wav_loadMem(void * aClassPtr, unsigned char * aMem, int aLength)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->loadMem(aMem, aLength);
}

void Wav_setLooping(void * aClassPtr, int aLoop)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setLooping(aLoop);
}

void Wav_setFilter(void * aClassPtr, int aFilterId, Filter * aFilter)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

void WavStream_destroy(void * aClassPtr)
{
  delete (WavStream *)aClassPtr;
}

void * WavStream_create()
{
  return (void *)new WavStream;
}

void WavStream_load(void * aClassPtr, const char * aFilename)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->load(aFilename);
}

void WavStream_setLooping(void * aClassPtr, int aLoop)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setLooping(aLoop);
}

void WavStream_setFilter(void * aClassPtr, int aFilterId, Filter * aFilter)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setFilter(aFilterId, aFilter);
}

} // extern "C"

