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

#ifndef SOLOUD_C_H_INCLUDED
#define SOLOUD_C_H_INCLUDED

#ifdef  __cplusplus
extern "C" {
#endif
// Collected enumerations
enum SOLOUD_ENUMS
{
	SOLOUD_AUTO = 0,
	SOLOUD_SDL = 1,
	SOLOUD_SDL2 = 2,
	SOLOUD_PORTAUDIO = 3,
	SOLOUD_WINMM = 4,
	SOLOUD_XAUDIO2 = 5,
	SOLOUD_WASAPI = 6,
	SOLOUD_ALSA = 7,
	SOLOUD_OSS = 8,
	SOLOUD_OPENAL = 9,
	SOLOUD_COREAUDIO = 10,
	SOLOUD_OPENSLES = 11,
	SOLOUD_NULLDRIVER = 12,
	SOLOUD_BACKEND_MAX = 13,
	SOLOUD_CLIP_ROUNDOFF = 1,
	SOLOUD_ENABLE_VISUALIZATION = 2,
	SOLOUD_LEFT_HANDED_3D = 4,
	BIQUADRESONANTFILTER_NONE = 0,
	BIQUADRESONANTFILTER_LOWPASS = 1,
	BIQUADRESONANTFILTER_HIGHPASS = 2,
	BIQUADRESONANTFILTER_BANDPASS = 3,
	BIQUADRESONANTFILTER_WET = 0,
	BIQUADRESONANTFILTER_SAMPLERATE = 1,
	BIQUADRESONANTFILTER_FREQUENCY = 2,
	BIQUADRESONANTFILTER_RESONANCE = 3,
	LOFIFILTER_WET = 0,
	LOFIFILTER_SAMPLERATE = 1,
	LOFIFILTER_BITDEPTH = 2,
	BASSBOOSTFILTER_WET = 0,
	BASSBOOSTFILTER_BOOST = 1,
	SFXR_COIN = 0,
	SFXR_LASER = 1,
	SFXR_EXPLOSION = 2,
	SFXR_POWERUP = 3,
	SFXR_HURT = 4,
	SFXR_JUMP = 5,
	SFXR_BLIP = 6,
	FLANGERFILTER_WET = 0,
	FLANGERFILTER_DELAY = 1,
	FLANGERFILTER_FREQ = 2,
	MONOTONE_SQUARE = 0,
	MONOTONE_SAW = 1,
	MONOTONE_SIN = 2,
	MONOTONE_SAWSIN = 3
};

// Object handle typedefs
typedef void * AlignedFloatBuffer;
typedef void * Soloud;
typedef void * AudioCollider;
typedef void * AudioAttenuator;
typedef void * AudioSource;
typedef void * BiquadResonantFilter;
typedef void * LofiFilter;
typedef void * Bus;
typedef void * EchoFilter;
typedef void * Fader;
typedef void * FFTFilter;
typedef void * BassboostFilter;
typedef void * Filter;
typedef void * Speech;
typedef void * Wav;
typedef void * WavStream;
typedef void * Prg;
typedef void * Sfxr;
typedef void * FlangerFilter;
typedef void * DCRemovalFilter;
typedef void * Modplug;
typedef void * Monotone;
typedef void * TedSid;
typedef void * File;

/*
 * Soloud
 */
void Soloud_destroy(Soloud * aSoloud);
Soloud * Soloud_create();
int Soloud_init(Soloud * aSoloud);
int Soloud_initEx(Soloud * aSoloud, unsigned int aFlags /* = Soloud::CLIP_ROUNDOFF */, unsigned int aBackend /* = Soloud::AUTO */, unsigned int aSamplerate /* = Soloud::AUTO */, unsigned int aBufferSize /* = Soloud::AUTO */, unsigned int aChannels /* = 2 */);
void Soloud_deinit(Soloud * aSoloud);
unsigned int Soloud_getVersion(Soloud * aSoloud);
const char * Soloud_getErrorString(Soloud * aSoloud, int aErrorCode);
unsigned int Soloud_getBackendId(Soloud * aSoloud);
const char * Soloud_getBackendString(Soloud * aSoloud);
unsigned int Soloud_getBackendChannels(Soloud * aSoloud);
unsigned int Soloud_getBackendSamplerate(Soloud * aSoloud);
unsigned int Soloud_getBackendBufferSize(Soloud * aSoloud);
int Soloud_setSpeakerPosition(Soloud * aSoloud, unsigned int aChannel, float aX, float aY, float aZ);
unsigned int Soloud_play(Soloud * aSoloud, AudioSource * aSound);
unsigned int Soloud_playEx(Soloud * aSoloud, AudioSource * aSound, float aVolume /* = -1.0f */, float aPan /* = 0.0f */, int aPaused /* = 0 */, unsigned int aBus /* = 0 */);
unsigned int Soloud_playClocked(Soloud * aSoloud, double aSoundTime, AudioSource * aSound);
unsigned int Soloud_playClockedEx(Soloud * aSoloud, double aSoundTime, AudioSource * aSound, float aVolume /* = -1.0f */, float aPan /* = 0.0f */, unsigned int aBus /* = 0 */);
unsigned int Soloud_play3d(Soloud * aSoloud, AudioSource * aSound, float aPosX, float aPosY, float aPosZ);
unsigned int Soloud_play3dEx(Soloud * aSoloud, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX /* = 0.0f */, float aVelY /* = 0.0f */, float aVelZ /* = 0.0f */, float aVolume /* = 1.0f */, int aPaused /* = 0 */, unsigned int aBus /* = 0 */);
unsigned int Soloud_play3dClocked(Soloud * aSoloud, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ);
unsigned int Soloud_play3dClockedEx(Soloud * aSoloud, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX /* = 0.0f */, float aVelY /* = 0.0f */, float aVelZ /* = 0.0f */, float aVolume /* = 1.0f */, unsigned int aBus /* = 0 */);
void Soloud_seek(Soloud * aSoloud, unsigned int aVoiceHandle, double aSeconds);
void Soloud_stop(Soloud * aSoloud, unsigned int aVoiceHandle);
void Soloud_stopAll(Soloud * aSoloud);
void Soloud_stopAudioSource(Soloud * aSoloud, AudioSource * aSound);
void Soloud_setFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId, float aValue);
float Soloud_getFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId);
void Soloud_fadeFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId, float aTo, double aTime);
void Soloud_oscillateFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aFilterId, unsigned int aAttributeId, float aFrom, float aTo, double aTime);
double Soloud_getStreamTime(Soloud * aSoloud, unsigned int aVoiceHandle);
int Soloud_getPause(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getVolume(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getOverallVolume(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getPan(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getSamplerate(Soloud * aSoloud, unsigned int aVoiceHandle);
int Soloud_getProtectVoice(Soloud * aSoloud, unsigned int aVoiceHandle);
unsigned int Soloud_getActiveVoiceCount(Soloud * aSoloud);
unsigned int Soloud_getVoiceCount(Soloud * aSoloud);
int Soloud_isValidVoiceHandle(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getRelativePlaySpeed(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getPostClipScaler(Soloud * aSoloud);
float Soloud_getGlobalVolume(Soloud * aSoloud);
unsigned int Soloud_getMaxActiveVoiceCount(Soloud * aSoloud);
int Soloud_getLooping(Soloud * aSoloud, unsigned int aVoiceHandle);
void Soloud_setLooping(Soloud * aSoloud, unsigned int aVoiceHandle, int aLooping);
int Soloud_setMaxActiveVoiceCount(Soloud * aSoloud, unsigned int aVoiceCount);
void Soloud_setInaudibleBehavior(Soloud * aSoloud, unsigned int aVoiceHandle, int aMustTick, int aKill);
void Soloud_setGlobalVolume(Soloud * aSoloud, float aVolume);
void Soloud_setPostClipScaler(Soloud * aSoloud, float aScaler);
void Soloud_setPause(Soloud * aSoloud, unsigned int aVoiceHandle, int aPause);
void Soloud_setPauseAll(Soloud * aSoloud, int aPause);
int Soloud_setRelativePlaySpeed(Soloud * aSoloud, unsigned int aVoiceHandle, float aSpeed);
void Soloud_setProtectVoice(Soloud * aSoloud, unsigned int aVoiceHandle, int aProtect);
void Soloud_setSamplerate(Soloud * aSoloud, unsigned int aVoiceHandle, float aSamplerate);
void Soloud_setPan(Soloud * aSoloud, unsigned int aVoiceHandle, float aPan);
void Soloud_setPanAbsolute(Soloud * aSoloud, unsigned int aVoiceHandle, float aLVolume, float aRVolume);
void Soloud_setPanAbsoluteEx(Soloud * aSoloud, unsigned int aVoiceHandle, float aLVolume, float aRVolume, float aLBVolume /* = 0 */, float aRBVolume /* = 0 */, float aCVolume /* = 0 */, float aSVolume /* = 0 */);
void Soloud_setVolume(Soloud * aSoloud, unsigned int aVoiceHandle, float aVolume);
void Soloud_setDelaySamples(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aSamples);
void Soloud_fadeVolume(Soloud * aSoloud, unsigned int aVoiceHandle, float aTo, double aTime);
void Soloud_fadePan(Soloud * aSoloud, unsigned int aVoiceHandle, float aTo, double aTime);
void Soloud_fadeRelativePlaySpeed(Soloud * aSoloud, unsigned int aVoiceHandle, float aTo, double aTime);
void Soloud_fadeGlobalVolume(Soloud * aSoloud, float aTo, double aTime);
void Soloud_schedulePause(Soloud * aSoloud, unsigned int aVoiceHandle, double aTime);
void Soloud_scheduleStop(Soloud * aSoloud, unsigned int aVoiceHandle, double aTime);
void Soloud_oscillateVolume(Soloud * aSoloud, unsigned int aVoiceHandle, float aFrom, float aTo, double aTime);
void Soloud_oscillatePan(Soloud * aSoloud, unsigned int aVoiceHandle, float aFrom, float aTo, double aTime);
void Soloud_oscillateRelativePlaySpeed(Soloud * aSoloud, unsigned int aVoiceHandle, float aFrom, float aTo, double aTime);
void Soloud_oscillateGlobalVolume(Soloud * aSoloud, float aFrom, float aTo, double aTime);
void Soloud_setGlobalFilter(Soloud * aSoloud, unsigned int aFilterId, Filter * aFilter);
void Soloud_setVisualizationEnable(Soloud * aSoloud, int aEnable);
float * Soloud_calcFFT(Soloud * aSoloud);
float * Soloud_getWave(Soloud * aSoloud);
unsigned int Soloud_getLoopCount(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getInfo(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aInfoKey);
unsigned int Soloud_createVoiceGroup(Soloud * aSoloud);
int Soloud_destroyVoiceGroup(Soloud * aSoloud, unsigned int aVoiceGroupHandle);
int Soloud_addVoiceToGroup(Soloud * aSoloud, unsigned int aVoiceGroupHandle, unsigned int aVoiceHandle);
int Soloud_isVoiceGroup(Soloud * aSoloud, unsigned int aVoiceGroupHandle);
int Soloud_isVoiceGroupEmpty(Soloud * aSoloud, unsigned int aVoiceGroupHandle);
void Soloud_update3dAudio(Soloud * aSoloud);
int Soloud_set3dSoundSpeed(Soloud * aSoloud, float aSpeed);
float Soloud_get3dSoundSpeed(Soloud * aSoloud);
void Soloud_set3dListenerParameters(Soloud * aSoloud, float aPosX, float aPosY, float aPosZ, float aAtX, float aAtY, float aAtZ, float aUpX, float aUpY, float aUpZ);
void Soloud_set3dListenerParametersEx(Soloud * aSoloud, float aPosX, float aPosY, float aPosZ, float aAtX, float aAtY, float aAtZ, float aUpX, float aUpY, float aUpZ, float aVelocityX /* = 0.0f */, float aVelocityY /* = 0.0f */, float aVelocityZ /* = 0.0f */);
void Soloud_set3dListenerPosition(Soloud * aSoloud, float aPosX, float aPosY, float aPosZ);
void Soloud_set3dListenerAt(Soloud * aSoloud, float aAtX, float aAtY, float aAtZ);
void Soloud_set3dListenerUp(Soloud * aSoloud, float aUpX, float aUpY, float aUpZ);
void Soloud_set3dListenerVelocity(Soloud * aSoloud, float aVelocityX, float aVelocityY, float aVelocityZ);
void Soloud_set3dSourceParameters(Soloud * aSoloud, unsigned int aVoiceHandle, float aPosX, float aPosY, float aPosZ);
void Soloud_set3dSourceParametersEx(Soloud * aSoloud, unsigned int aVoiceHandle, float aPosX, float aPosY, float aPosZ, float aVelocityX /* = 0.0f */, float aVelocityY /* = 0.0f */, float aVelocityZ /* = 0.0f */);
void Soloud_set3dSourcePosition(Soloud * aSoloud, unsigned int aVoiceHandle, float aPosX, float aPosY, float aPosZ);
void Soloud_set3dSourceVelocity(Soloud * aSoloud, unsigned int aVoiceHandle, float aVelocityX, float aVelocityY, float aVelocityZ);
void Soloud_set3dSourceMinMaxDistance(Soloud * aSoloud, unsigned int aVoiceHandle, float aMinDistance, float aMaxDistance);
void Soloud_set3dSourceAttenuation(Soloud * aSoloud, unsigned int aVoiceHandle, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Soloud_set3dSourceDopplerFactor(Soloud * aSoloud, unsigned int aVoiceHandle, float aDopplerFactor);
void Soloud_mix(Soloud * aSoloud, float * aBuffer, unsigned int aSamples);
void Soloud_mixSigned16(Soloud * aSoloud, short * aBuffer, unsigned int aSamples);

/*
 * AudioAttenuator
 */
void AudioAttenuator_destroy(AudioAttenuator * aAudioAttenuator);
float AudioAttenuator_attenuate(AudioAttenuator * aAudioAttenuator, float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor);

/*
 * BiquadResonantFilter
 */
void BiquadResonantFilter_destroy(BiquadResonantFilter * aBiquadResonantFilter);
BiquadResonantFilter * BiquadResonantFilter_create();
int BiquadResonantFilter_setParams(BiquadResonantFilter * aBiquadResonantFilter, int aType, float aSampleRate, float aFrequency, float aResonance);

/*
 * LofiFilter
 */
void LofiFilter_destroy(LofiFilter * aLofiFilter);
LofiFilter * LofiFilter_create();
int LofiFilter_setParams(LofiFilter * aLofiFilter, float aSampleRate, float aBitdepth);

/*
 * Bus
 */
void Bus_destroy(Bus * aBus);
Bus * Bus_create();
void Bus_setFilter(Bus * aBus, unsigned int aFilterId, Filter * aFilter);
unsigned int Bus_play(Bus * aBus, AudioSource * aSound);
unsigned int Bus_playEx(Bus * aBus, AudioSource * aSound, float aVolume /* = 1.0f */, float aPan /* = 0.0f */, int aPaused /* = 0 */);
unsigned int Bus_playClocked(Bus * aBus, double aSoundTime, AudioSource * aSound);
unsigned int Bus_playClockedEx(Bus * aBus, double aSoundTime, AudioSource * aSound, float aVolume /* = 1.0f */, float aPan /* = 0.0f */);
unsigned int Bus_play3d(Bus * aBus, AudioSource * aSound, float aPosX, float aPosY, float aPosZ);
unsigned int Bus_play3dEx(Bus * aBus, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX /* = 0.0f */, float aVelY /* = 0.0f */, float aVelZ /* = 0.0f */, float aVolume /* = 1.0f */, int aPaused /* = 0 */);
unsigned int Bus_play3dClocked(Bus * aBus, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ);
unsigned int Bus_play3dClockedEx(Bus * aBus, double aSoundTime, AudioSource * aSound, float aPosX, float aPosY, float aPosZ, float aVelX /* = 0.0f */, float aVelY /* = 0.0f */, float aVelZ /* = 0.0f */, float aVolume /* = 1.0f */);
int Bus_setChannels(Bus * aBus, unsigned int aChannels);
void Bus_setVisualizationEnable(Bus * aBus, int aEnable);
float * Bus_calcFFT(Bus * aBus);
float * Bus_getWave(Bus * aBus);
void Bus_setVolume(Bus * aBus, float aVolume);
void Bus_setLooping(Bus * aBus, int aLoop);
void Bus_set3dMinMaxDistance(Bus * aBus, float aMinDistance, float aMaxDistance);
void Bus_set3dAttenuation(Bus * aBus, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Bus_set3dDopplerFactor(Bus * aBus, float aDopplerFactor);
void Bus_set3dProcessing(Bus * aBus, int aDo3dProcessing);
void Bus_set3dListenerRelative(Bus * aBus, int aListenerRelative);
void Bus_set3dDistanceDelay(Bus * aBus, int aDistanceDelay);
void Bus_set3dCollider(Bus * aBus, AudioCollider * aCollider);
void Bus_set3dColliderEx(Bus * aBus, AudioCollider * aCollider, int aUserData /* = 0 */);
void Bus_set3dAttenuator(Bus * aBus, AudioAttenuator * aAttenuator);
void Bus_setInaudibleBehavior(Bus * aBus, int aMustTick, int aKill);
void Bus_stop(Bus * aBus);

/*
 * EchoFilter
 */
void EchoFilter_destroy(EchoFilter * aEchoFilter);
EchoFilter * EchoFilter_create();
int EchoFilter_setParams(EchoFilter * aEchoFilter, float aDelay);
int EchoFilter_setParamsEx(EchoFilter * aEchoFilter, float aDelay, float aDecay /* = 0.7f */, float aFilter /* = 0.0f */);

/*
 * FFTFilter
 */
void FFTFilter_destroy(FFTFilter * aFFTFilter);
FFTFilter * FFTFilter_create();

/*
 * BassboostFilter
 */
void BassboostFilter_destroy(BassboostFilter * aBassboostFilter);
int BassboostFilter_setParams(BassboostFilter * aBassboostFilter, float aBoost);
BassboostFilter * BassboostFilter_create();

/*
 * Speech
 */
void Speech_destroy(Speech * aSpeech);
Speech * Speech_create();
int Speech_setText(Speech * aSpeech, const char * aText);
void Speech_setVolume(Speech * aSpeech, float aVolume);
void Speech_setLooping(Speech * aSpeech, int aLoop);
void Speech_set3dMinMaxDistance(Speech * aSpeech, float aMinDistance, float aMaxDistance);
void Speech_set3dAttenuation(Speech * aSpeech, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Speech_set3dDopplerFactor(Speech * aSpeech, float aDopplerFactor);
void Speech_set3dProcessing(Speech * aSpeech, int aDo3dProcessing);
void Speech_set3dListenerRelative(Speech * aSpeech, int aListenerRelative);
void Speech_set3dDistanceDelay(Speech * aSpeech, int aDistanceDelay);
void Speech_set3dCollider(Speech * aSpeech, AudioCollider * aCollider);
void Speech_set3dColliderEx(Speech * aSpeech, AudioCollider * aCollider, int aUserData /* = 0 */);
void Speech_set3dAttenuator(Speech * aSpeech, AudioAttenuator * aAttenuator);
void Speech_setInaudibleBehavior(Speech * aSpeech, int aMustTick, int aKill);
void Speech_setFilter(Speech * aSpeech, unsigned int aFilterId, Filter * aFilter);
void Speech_stop(Speech * aSpeech);

/*
 * Wav
 */
void Wav_destroy(Wav * aWav);
Wav * Wav_create();
int Wav_load(Wav * aWav, const char * aFilename);
int Wav_loadMem(Wav * aWav, unsigned char * aMem, unsigned int aLength);
int Wav_loadMemEx(Wav * aWav, unsigned char * aMem, unsigned int aLength, int aCopy /* = false */, int aTakeOwnership /* = true */);
int Wav_loadFile(Wav * aWav, File * aFile);
double Wav_getLength(Wav * aWav);
void Wav_setVolume(Wav * aWav, float aVolume);
void Wav_setLooping(Wav * aWav, int aLoop);
void Wav_set3dMinMaxDistance(Wav * aWav, float aMinDistance, float aMaxDistance);
void Wav_set3dAttenuation(Wav * aWav, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Wav_set3dDopplerFactor(Wav * aWav, float aDopplerFactor);
void Wav_set3dProcessing(Wav * aWav, int aDo3dProcessing);
void Wav_set3dListenerRelative(Wav * aWav, int aListenerRelative);
void Wav_set3dDistanceDelay(Wav * aWav, int aDistanceDelay);
void Wav_set3dCollider(Wav * aWav, AudioCollider * aCollider);
void Wav_set3dColliderEx(Wav * aWav, AudioCollider * aCollider, int aUserData /* = 0 */);
void Wav_set3dAttenuator(Wav * aWav, AudioAttenuator * aAttenuator);
void Wav_setInaudibleBehavior(Wav * aWav, int aMustTick, int aKill);
void Wav_setFilter(Wav * aWav, unsigned int aFilterId, Filter * aFilter);
void Wav_stop(Wav * aWav);

/*
 * WavStream
 */
void WavStream_destroy(WavStream * aWavStream);
WavStream * WavStream_create();
int WavStream_load(WavStream * aWavStream, const char * aFilename);
int WavStream_loadMem(WavStream * aWavStream, unsigned char * aData, unsigned int aDataLen);
int WavStream_loadMemEx(WavStream * aWavStream, unsigned char * aData, unsigned int aDataLen, int aCopy /* = false */, int aTakeOwnership /* = true */);
int WavStream_loadToMem(WavStream * aWavStream, const char * aFilename);
int WavStream_loadFile(WavStream * aWavStream, File * aFile);
int WavStream_loadFileToMem(WavStream * aWavStream, File * aFile);
double WavStream_getLength(WavStream * aWavStream);
void WavStream_setVolume(WavStream * aWavStream, float aVolume);
void WavStream_setLooping(WavStream * aWavStream, int aLoop);
void WavStream_set3dMinMaxDistance(WavStream * aWavStream, float aMinDistance, float aMaxDistance);
void WavStream_set3dAttenuation(WavStream * aWavStream, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void WavStream_set3dDopplerFactor(WavStream * aWavStream, float aDopplerFactor);
void WavStream_set3dProcessing(WavStream * aWavStream, int aDo3dProcessing);
void WavStream_set3dListenerRelative(WavStream * aWavStream, int aListenerRelative);
void WavStream_set3dDistanceDelay(WavStream * aWavStream, int aDistanceDelay);
void WavStream_set3dCollider(WavStream * aWavStream, AudioCollider * aCollider);
void WavStream_set3dColliderEx(WavStream * aWavStream, AudioCollider * aCollider, int aUserData /* = 0 */);
void WavStream_set3dAttenuator(WavStream * aWavStream, AudioAttenuator * aAttenuator);
void WavStream_setInaudibleBehavior(WavStream * aWavStream, int aMustTick, int aKill);
void WavStream_setFilter(WavStream * aWavStream, unsigned int aFilterId, Filter * aFilter);
void WavStream_stop(WavStream * aWavStream);

/*
 * Prg
 */
void Prg_destroy(Prg * aPrg);
Prg * Prg_create();
unsigned int Prg_rand(Prg * aPrg);
void Prg_srand(Prg * aPrg, int aSeed);

/*
 * Sfxr
 */
void Sfxr_destroy(Sfxr * aSfxr);
Sfxr * Sfxr_create();
void Sfxr_resetParams(Sfxr * aSfxr);
int Sfxr_loadParams(Sfxr * aSfxr, const char * aFilename);
int Sfxr_loadParamsMem(Sfxr * aSfxr, unsigned char * aMem, unsigned int aLength);
int Sfxr_loadParamsMemEx(Sfxr * aSfxr, unsigned char * aMem, unsigned int aLength, int aCopy /* = false */, int aTakeOwnership /* = true */);
int Sfxr_loadParamsFile(Sfxr * aSfxr, File * aFile);
int Sfxr_loadPreset(Sfxr * aSfxr, int aPresetNo, int aRandSeed);
void Sfxr_setVolume(Sfxr * aSfxr, float aVolume);
void Sfxr_setLooping(Sfxr * aSfxr, int aLoop);
void Sfxr_set3dMinMaxDistance(Sfxr * aSfxr, float aMinDistance, float aMaxDistance);
void Sfxr_set3dAttenuation(Sfxr * aSfxr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Sfxr_set3dDopplerFactor(Sfxr * aSfxr, float aDopplerFactor);
void Sfxr_set3dProcessing(Sfxr * aSfxr, int aDo3dProcessing);
void Sfxr_set3dListenerRelative(Sfxr * aSfxr, int aListenerRelative);
void Sfxr_set3dDistanceDelay(Sfxr * aSfxr, int aDistanceDelay);
void Sfxr_set3dCollider(Sfxr * aSfxr, AudioCollider * aCollider);
void Sfxr_set3dColliderEx(Sfxr * aSfxr, AudioCollider * aCollider, int aUserData /* = 0 */);
void Sfxr_set3dAttenuator(Sfxr * aSfxr, AudioAttenuator * aAttenuator);
void Sfxr_setInaudibleBehavior(Sfxr * aSfxr, int aMustTick, int aKill);
void Sfxr_setFilter(Sfxr * aSfxr, unsigned int aFilterId, Filter * aFilter);
void Sfxr_stop(Sfxr * aSfxr);

/*
 * FlangerFilter
 */
void FlangerFilter_destroy(FlangerFilter * aFlangerFilter);
FlangerFilter * FlangerFilter_create();
int FlangerFilter_setParams(FlangerFilter * aFlangerFilter, float aDelay, float aFreq);

/*
 * DCRemovalFilter
 */
void DCRemovalFilter_destroy(DCRemovalFilter * aDCRemovalFilter);
DCRemovalFilter * DCRemovalFilter_create();
int DCRemovalFilter_setParams(DCRemovalFilter * aDCRemovalFilter);
int DCRemovalFilter_setParamsEx(DCRemovalFilter * aDCRemovalFilter, float aLength /* = 0.1f */);

/*
 * Modplug
 */
void Modplug_destroy(Modplug * aModplug);
Modplug * Modplug_create();
int Modplug_load(Modplug * aModplug, const char * aFilename);
int Modplug_loadMem(Modplug * aModplug, unsigned char * aMem, unsigned int aLength);
int Modplug_loadMemEx(Modplug * aModplug, unsigned char * aMem, unsigned int aLength, int aCopy /* = false */, int aTakeOwnership /* = true */);
int Modplug_loadFile(Modplug * aModplug, File * aFile);
void Modplug_setVolume(Modplug * aModplug, float aVolume);
void Modplug_setLooping(Modplug * aModplug, int aLoop);
void Modplug_set3dMinMaxDistance(Modplug * aModplug, float aMinDistance, float aMaxDistance);
void Modplug_set3dAttenuation(Modplug * aModplug, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Modplug_set3dDopplerFactor(Modplug * aModplug, float aDopplerFactor);
void Modplug_set3dProcessing(Modplug * aModplug, int aDo3dProcessing);
void Modplug_set3dListenerRelative(Modplug * aModplug, int aListenerRelative);
void Modplug_set3dDistanceDelay(Modplug * aModplug, int aDistanceDelay);
void Modplug_set3dCollider(Modplug * aModplug, AudioCollider * aCollider);
void Modplug_set3dColliderEx(Modplug * aModplug, AudioCollider * aCollider, int aUserData /* = 0 */);
void Modplug_set3dAttenuator(Modplug * aModplug, AudioAttenuator * aAttenuator);
void Modplug_setInaudibleBehavior(Modplug * aModplug, int aMustTick, int aKill);
void Modplug_setFilter(Modplug * aModplug, unsigned int aFilterId, Filter * aFilter);
void Modplug_stop(Modplug * aModplug);

/*
 * Monotone
 */
void Monotone_destroy(Monotone * aMonotone);
Monotone * Monotone_create();
int Monotone_setParams(Monotone * aMonotone, int aHardwareChannels);
int Monotone_setParamsEx(Monotone * aMonotone, int aHardwareChannels, int aWaveform /* = SQUARE */);
int Monotone_load(Monotone * aMonotone, const char * aFilename);
int Monotone_loadMem(Monotone * aMonotone, unsigned char * aMem, unsigned int aLength);
int Monotone_loadMemEx(Monotone * aMonotone, unsigned char * aMem, unsigned int aLength, int aCopy /* = false */, int aTakeOwnership /* = true */);
int Monotone_loadFile(Monotone * aMonotone, File * aFile);
void Monotone_setVolume(Monotone * aMonotone, float aVolume);
void Monotone_setLooping(Monotone * aMonotone, int aLoop);
void Monotone_set3dMinMaxDistance(Monotone * aMonotone, float aMinDistance, float aMaxDistance);
void Monotone_set3dAttenuation(Monotone * aMonotone, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void Monotone_set3dDopplerFactor(Monotone * aMonotone, float aDopplerFactor);
void Monotone_set3dProcessing(Monotone * aMonotone, int aDo3dProcessing);
void Monotone_set3dListenerRelative(Monotone * aMonotone, int aListenerRelative);
void Monotone_set3dDistanceDelay(Monotone * aMonotone, int aDistanceDelay);
void Monotone_set3dCollider(Monotone * aMonotone, AudioCollider * aCollider);
void Monotone_set3dColliderEx(Monotone * aMonotone, AudioCollider * aCollider, int aUserData /* = 0 */);
void Monotone_set3dAttenuator(Monotone * aMonotone, AudioAttenuator * aAttenuator);
void Monotone_setInaudibleBehavior(Monotone * aMonotone, int aMustTick, int aKill);
void Monotone_setFilter(Monotone * aMonotone, unsigned int aFilterId, Filter * aFilter);
void Monotone_stop(Monotone * aMonotone);

/*
 * TedSid
 */
void TedSid_destroy(TedSid * aTedSid);
TedSid * TedSid_create();
int TedSid_load(TedSid * aTedSid, const char * aFilename);
int TedSid_loadToMem(TedSid * aTedSid, const char * aFilename);
int TedSid_loadMem(TedSid * aTedSid, unsigned char * aMem, unsigned int aLength);
int TedSid_loadMemEx(TedSid * aTedSid, unsigned char * aMem, unsigned int aLength, int aCopy /* = false */, int aTakeOwnership /* = true */);
int TedSid_loadFileToMem(TedSid * aTedSid, File * aFile);
int TedSid_loadFile(TedSid * aTedSid, File * aFile);
void TedSid_setVolume(TedSid * aTedSid, float aVolume);
void TedSid_setLooping(TedSid * aTedSid, int aLoop);
void TedSid_set3dMinMaxDistance(TedSid * aTedSid, float aMinDistance, float aMaxDistance);
void TedSid_set3dAttenuation(TedSid * aTedSid, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void TedSid_set3dDopplerFactor(TedSid * aTedSid, float aDopplerFactor);
void TedSid_set3dProcessing(TedSid * aTedSid, int aDo3dProcessing);
void TedSid_set3dListenerRelative(TedSid * aTedSid, int aListenerRelative);
void TedSid_set3dDistanceDelay(TedSid * aTedSid, int aDistanceDelay);
void TedSid_set3dCollider(TedSid * aTedSid, AudioCollider * aCollider);
void TedSid_set3dColliderEx(TedSid * aTedSid, AudioCollider * aCollider, int aUserData /* = 0 */);
void TedSid_set3dAttenuator(TedSid * aTedSid, AudioAttenuator * aAttenuator);
void TedSid_setInaudibleBehavior(TedSid * aTedSid, int aMustTick, int aKill);
void TedSid_setFilter(TedSid * aTedSid, unsigned int aFilterId, Filter * aFilter);
void TedSid_stop(TedSid * aTedSid);
#ifdef  __cplusplus
} // extern "C"
#endif

#endif // SOLOUD_C_H_INCLUDED

