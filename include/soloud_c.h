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
	SOLOUD_OSS = 7,
	SOLOUD_OPENAL = 8,
	SOLOUD_BACKEND_MAX = 9,
	SOLOUD_CLIP_ROUNDOFF = 1,
	SOLOUD_ENABLE_VISUALIZATION = 2,
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
	FFTFILTER_OVER = 0,
	FFTFILTER_SUBTRACT = 1,
	FFTFILTER_MULTIPLY = 2,
	SFXR_COIN = 0,
	SFXR_LASER = 1,
	SFXR_EXPLOSION = 2,
	SFXR_POWERUP = 3,
	SFXR_HURT = 4,
	SFXR_JUMP = 5,
	SFXR_BLIP = 6
};

// Object handle typedefs
typedef void * Soloud;
typedef void * AudioSource;
typedef void * BiquadResonantFilter;
typedef void * LofiFilter;
typedef void * Bus;
typedef void * EchoFilter;
typedef void * Fader;
typedef void * FFTFilter;
typedef void * Filter;
typedef void * Speech;
typedef void * Wav;
typedef void * WavStream;
typedef void * Prg;
typedef void * Sfxr;
typedef void * Modplug;

/*
 * Soloud
 */
void Soloud_destroy(Soloud * aSoloud);
Soloud * Soloud_create();
int Soloud_init(Soloud * aSoloud);
int Soloud_initEx(Soloud * aSoloud, int aFlags /* = Soloud::CLIP_ROUNDOFF */, int aBackend /* = Soloud::AUTO */, int aSamplerate /* = Soloud::AUTO */, int aBufferSize /* = Soloud::AUTO */);
void Soloud_deinit(Soloud * aSoloud);
int Soloud_getVersion(Soloud * aSoloud);
const char * Soloud_getErrorString(Soloud * aSoloud, int aErrorCode);
unsigned int Soloud_play(Soloud * aSoloud, AudioSource * aSound);
unsigned int Soloud_playEx(Soloud * aSoloud, AudioSource * aSound, float aVolume /* = 1.0f */, float aPan /* = 0.0f */, int aPaused /* = 0 */, int aBus /* = 0 */);
void Soloud_seek(Soloud * aSoloud, unsigned int aVoiceHandle, double aSeconds);
void Soloud_stop(Soloud * aSoloud, unsigned int aVoiceHandle);
void Soloud_stopAll(Soloud * aSoloud);
void Soloud_stopAudioSource(Soloud * aSoloud, AudioSource * aSound);
void Soloud_setFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, int aFilterId, int aAttributeId, float aValue);
float Soloud_getFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, int aFilterId, int aAttributeId);
void Soloud_fadeFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, int aFilterId, int aAttributeId, float aTo, double aTime);
void Soloud_oscillateFilterParameter(Soloud * aSoloud, unsigned int aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, double aTime);
double Soloud_getStreamTime(Soloud * aSoloud, unsigned int aVoiceHandle);
int Soloud_getPause(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getVolume(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getPan(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getSamplerate(Soloud * aSoloud, unsigned int aVoiceHandle);
int Soloud_getProtectVoice(Soloud * aSoloud, unsigned int aVoiceHandle);
int Soloud_getActiveVoiceCount(Soloud * aSoloud);
int Soloud_isValidVoiceHandle(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getRelativePlaySpeed(Soloud * aSoloud, unsigned int aVoiceHandle);
float Soloud_getPostClipScaler(Soloud * aSoloud);
float Soloud_getGlobalVolume(Soloud * aSoloud);
void Soloud_setGlobalVolume(Soloud * aSoloud, float aVolume);
void Soloud_setPostClipScaler(Soloud * aSoloud, float aScaler);
void Soloud_setPause(Soloud * aSoloud, unsigned int aVoiceHandle, int aPause);
void Soloud_setPauseAll(Soloud * aSoloud, int aPause);
void Soloud_setRelativePlaySpeed(Soloud * aSoloud, unsigned int aVoiceHandle, float aSpeed);
void Soloud_setProtectVoice(Soloud * aSoloud, unsigned int aVoiceHandle, int aProtect);
void Soloud_setSamplerate(Soloud * aSoloud, unsigned int aVoiceHandle, float aSamplerate);
void Soloud_setPan(Soloud * aSoloud, unsigned int aVoiceHandle, float aPan);
void Soloud_setPanAbsolute(Soloud * aSoloud, unsigned int aVoiceHandle, float aLVolume, float aRVolume);
void Soloud_setVolume(Soloud * aSoloud, unsigned int aVoiceHandle, float aVolume);
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
void Soloud_setGlobalFilter(Soloud * aSoloud, int aFilterId, Filter * aFilter);
void Soloud_setVisualizationEnable(Soloud * aSoloud, int aEnable);
float * Soloud_calcFFT(Soloud * aSoloud);
float * Soloud_getWave(Soloud * aSoloud);
int Soloud_getLoopCount(Soloud * aSoloud, unsigned int aVoiceHandle);

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
void Bus_setFilter(Bus * aBus, int aFilterId, Filter * aFilter);
int Bus_play(Bus * aBus, AudioSource * aSound);
int Bus_playEx(Bus * aBus, AudioSource * aSound, float aVolume /* = 1.0f */, float aPan /* = 0.0f */, int aPaused /* = 0 */);
void Bus_setVisualizationEnable(Bus * aBus, int aEnable);
float * Bus_calcFFT(Bus * aBus);
float * Bus_getWave(Bus * aBus);
void Bus_setLooping(Bus * aBus, int aLoop);
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
int FFTFilter_setParameters(FFTFilter * aFFTFilter, int aShift);
int FFTFilter_setParametersEx(FFTFilter * aFFTFilter, int aShift, int aCombine /* = 0 */, float aScale /* = 0.002 */);

/*
 * Speech
 */
void Speech_destroy(Speech * aSpeech);
Speech * Speech_create();
int Speech_setText(Speech * aSpeech, const char * aText);
void Speech_setLooping(Speech * aSpeech, int aLoop);
void Speech_setFilter(Speech * aSpeech, int aFilterId, Filter * aFilter);
void Speech_stop(Speech * aSpeech);

/*
 * Wav
 */
void Wav_destroy(Wav * aWav);
Wav * Wav_create();
int Wav_load(Wav * aWav, const char * aFilename);
int Wav_loadMem(Wav * aWav, unsigned char * aMem, int aLength);
double Wav_getLength(Wav * aWav);
void Wav_setLooping(Wav * aWav, int aLoop);
void Wav_setFilter(Wav * aWav, int aFilterId, Filter * aFilter);
void Wav_stop(Wav * aWav);

/*
 * WavStream
 */
void WavStream_destroy(WavStream * aWavStream);
WavStream * WavStream_create();
int WavStream_load(WavStream * aWavStream, const char * aFilename);
double WavStream_getLength(WavStream * aWavStream);
void WavStream_setLooping(WavStream * aWavStream, int aLoop);
void WavStream_setFilter(WavStream * aWavStream, int aFilterId, Filter * aFilter);
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
int Sfxr_loadPreset(Sfxr * aSfxr, int aPresetNo, int aRandSeed);
void Sfxr_setLooping(Sfxr * aSfxr, int aLoop);
void Sfxr_setFilter(Sfxr * aSfxr, int aFilterId, Filter * aFilter);
void Sfxr_stop(Sfxr * aSfxr);

/*
 * Modplug
 */
void Modplug_destroy(Modplug * aModplug);
Modplug * Modplug_create();
int Modplug_load(Modplug * aModplug, const char * aFilename);
void Modplug_setLooping(Modplug * aModplug, int aLoop);
void Modplug_setFilter(Modplug * aModplug, int aFilterId, Filter * aFilter);
void Modplug_stop(Modplug * aModplug);
#ifdef  __cplusplus
} // extern "C"
#endif

#endif // SOLOUD_C_H_INCLUDED

