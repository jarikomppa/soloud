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

/* SoLoud C-Api Code Generator (c)2013-2014 Jari Komppa http://iki.fi/sol/ */

#include "../include/soloud.h"
#include "../include/soloud_audiosource.h"
#include "../include/soloud_biquadresonantfilter.h"
#include "../include/soloud_lofifilter.h"
#include "../include/soloud_bus.h"
#include "../include/soloud_echofilter.h"
#include "../include/soloud_fader.h"
#include "../include/soloud_fftfilter.h"
#include "../include/soloud_filter.h"
#include "../include/soloud_speech.h"
#include "../include/soloud_thread.h"
#include "../include/soloud_wav.h"
#include "../include/soloud_wavstream.h"
#include "../include/soloud_sfxr.h"
#include "../include/soloud_flangerfilter.h"
#include "../include/soloud_modplug.h"

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

int Soloud_initEx(void * aClassPtr, unsigned int aFlags, unsigned int aBackend, unsigned int aSamplerate, unsigned int aBufferSize)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->init(aFlags, aBackend, aSamplerate, aBufferSize);
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

unsigned int Soloud_play(void * aClassPtr, AudioSource * aSound)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play(*aSound);
}

unsigned int Soloud_playEx(void * aClassPtr, AudioSource * aSound, float aVolume, float aPan, int aPaused, unsigned int aBus)
{
	Soloud * cl = (Soloud *)aClassPtr;
	return cl->play(*aSound, aVolume, aPan, aPaused, aBus);
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
	cl->setPause(aVoiceHandle, aPause);
}

void Soloud_setPauseAll(void * aClassPtr, int aPause)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setPauseAll(aPause);
}

void Soloud_setRelativePlaySpeed(void * aClassPtr, unsigned int aVoiceHandle, float aSpeed)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setRelativePlaySpeed(aVoiceHandle, aSpeed);
}

void Soloud_setProtectVoice(void * aClassPtr, unsigned int aVoiceHandle, int aProtect)
{
	Soloud * cl = (Soloud *)aClassPtr;
	cl->setProtectVoice(aVoiceHandle, aProtect);
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
	cl->setVisualizationEnable(aEnable);
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
	return cl->play(*aSound, aVolume, aPan, aPaused);
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

void Bus_setVisualizationEnable(void * aClassPtr, int aEnable)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setVisualizationEnable(aEnable);
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

void Bus_setLooping(void * aClassPtr, int aLoop)
{
	Bus * cl = (Bus *)aClassPtr;
	cl->setLooping(aLoop);
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

int FFTFilter_setParameters(void * aClassPtr, int aShift)
{
	FFTFilter * cl = (FFTFilter *)aClassPtr;
	return cl->setParameters(aShift);
}

int FFTFilter_setParametersEx(void * aClassPtr, int aShift, int aCombine, float aScale)
{
	FFTFilter * cl = (FFTFilter *)aClassPtr;
	return cl->setParameters(aShift, aCombine, aScale);
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

void Modplug_setLooping(void * aClassPtr, int aLoop)
{
	Modplug * cl = (Modplug *)aClassPtr;
	cl->setLooping(aLoop);
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

int Sfxr_loadPreset(void * aClassPtr, int aPresetNo, int aRandSeed)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	return cl->loadPreset(aPresetNo, aRandSeed);
}

void Sfxr_setLooping(void * aClassPtr, int aLoop)
{
	Sfxr * cl = (Sfxr *)aClassPtr;
	cl->setLooping(aLoop);
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

void Speech_setLooping(void * aClassPtr, int aLoop)
{
	Speech * cl = (Speech *)aClassPtr;
	cl->setLooping(aLoop);
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

double Wav_getLength(void * aClassPtr)
{
	Wav * cl = (Wav *)aClassPtr;
	return cl->getLength();
}

void Wav_setLooping(void * aClassPtr, int aLoop)
{
	Wav * cl = (Wav *)aClassPtr;
	cl->setLooping(aLoop);
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

double WavStream_getLength(void * aClassPtr)
{
	WavStream * cl = (WavStream *)aClassPtr;
	return cl->getLength();
}

void WavStream_setLooping(void * aClassPtr, int aLoop)
{
	WavStream * cl = (WavStream *)aClassPtr;
	cl->setLooping(aLoop);
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

} // extern "C"

