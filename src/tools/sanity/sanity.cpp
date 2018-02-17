/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

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

/*********************************************************************************
*
* Testing goal is primarily for sanity checks, to verify that everything did not
* blow up due to some innocent-looking change.
*
* In some cases this means that all we're testing is that yeah, there's noise, or
* that yeah, the noise changes when we changed a parameter.
*
* Some tests against known good values can also be done, for deterministic processes.
*
**********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "soloud.h"
#include "soloud_bassboostfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_dcremovalfilter.h"
#include "soloud_echofilter.h"
#include "soloud_flangerfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_monotone.h"
#include "soloud_openmpt.h"
#include "soloud_robotizefilter.h"
#include "soloud_sfxr.h"
#include "soloud_speech.h"
#include "soloud_tedsid.h"
#include "soloud_vic.h"
#include "soloud_wav.h"
#include "soloud_waveshaperfilter.h"
#include "soloud_wavstream.h"

int errorcount = 0;
int tests = 0;
int verbose = 1;

#define CHECK_RES(x) tests++; if ((x)) { errorcount++; printf("Error on line %d, %s(): %s\n",__LINE__,__FUNCTION__, soloud.getErrorString((x)));}
#define CHECK(x) tests++; if (!(x)) { errorcount++; printf("Error on line %d, %s(): Check \"%s\" fail\n",__LINE__,__FUNCTION__,#x);}
#define CHECK_BUF_NONZERO(x, n) tests++; { int i, zero = 1; for (i = 0; i < (n); i++) if ((x)[i] != 0) zero = 0; if (zero) { errorcount++; printf("Error on line %d, %s(): buffer not nonzero\n",__LINE__,__FUNCTION__);}}
#define CHECK_BUF_ZERO(x, n) tests++; { int i, zero = 1; for (i = 0; i < (n); i++) if ((x)[i] != 0) zero = 0; if (!zero) { errorcount++; printf("Error on line %d, %s(): buffer not zero\n",__LINE__,__FUNCTION__);}}
#define CHECK_BUF_DIFF(x, y, n) tests++; { int i, same = 1; for (i = 0; i < (n); i++) if (fabs((x)[i] - (y)[i]) > 0.00001) same = 0; if (same) { errorcount++; printf("Error on line %d, %s(): buffers are equal\n",__LINE__,__FUNCTION__);}}
#define CHECK_BUF_SAME(x, y, n) tests++; { int i, same = 1; for (i = 0; i < (n); i++) if (fabs((x)[i] - (y)[i]) > 0.00001) same = 0; if (!same) { errorcount++; printf("Error on line %d, %s(): buffers differ\n",__LINE__,__FUNCTION__);}}
#define CHECK_BUF_GTE(x, y, n) tests++; { int i, lt = 0; for (i = 0; i < (n); i++) if (fabs((x)[i]) - fabs((y)[i]) < 0) lt = 1; if (lt) { errorcount++; printf("Error on line %d, %s(): buffer %s magnitude not bigger than buffer %s \n",__LINE__,__FUNCTION__,#x, #y);}}


void testWave(SoLoud::Wav &aWav)
{
	unsigned char buf[16044] = { 
		0x52, 0x49, 0x46, 0x46, // RIFF
		0xa4, 0x3e, 0x00, 0x00, // length of file - 8
		0x57, 0x41, 0x56, 0x45, // WAVE
		0x66, 0x6d, 0x74, 0x20, // fmt
		0x10, 0x00, 0x00, 0x00, // PCM block
		0x01, 0x00,             // Uncompressed PCM
		0x01, 0x00,             // Channels (mono)
		0x40, 0x1f, 0x00, 0x00, // 8000Hz
		0x40, 0x1f, 0x00, 0x00, // 8000 bytes per sec
		0x01, 0x00,             // Number of bytes per sample for all channels
		0x08, 0x00,             // Bits per channel
		0x64, 0x61, 0x74, 0x61, // data
		0x80, 0x3e, 0x00, 0x00, // bytes of data
		// 44 bytes up to this point
	};
	unsigned int buflen = sizeof(buf);
	int i;
	for (i = 0; i < 16000; i++)
	{
		buf[i + 44] = ((i&1)?1:-1)*(char)((sin(i * 0.001) * 0x7f) + i);
	}
	aWav.loadMem(buf, buflen, true, false);
}

void printinfo(const char * format, ...)
{
	if (!verbose)
		return;
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

// Some info tests
//
// Soloud.init
// Soloud.deinit
// Soloud.getVersion
// Soloud.getErrorString
// Soloud.getBackendId
// Soloud.getBackendString
// Soloud.getBackendChannels
// Soloud.getBackendSamplerate
// Soloud.getBackendBufferSize
void testInfo()
{
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::result res = soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::NULLDRIVER);
	CHECK_RES(res);
	int ver = soloud.getVersion();
	CHECK(ver == SOLOUD_VERSION);
	printinfo("SoLoud version %d\n", ver);
	CHECK(soloud.getErrorString(0) != 0);
	printinfo("Backend %d: %s, %d channels, %d samplerate, %d buffersize\n",
		soloud.getBackendId(),
		soloud.getBackendString(),
		soloud.getBackendChannels(),
		soloud.getBackendSamplerate(),
		soloud.getBackendBufferSize());
	CHECK(soloud.getBackendId() != 0);
	CHECK(soloud.getBackendString() != 0);
	CHECK(soloud.getBackendChannels() != 0);
	CHECK(soloud.getBackendSamplerate() != 0);
	CHECK(soloud.getBackendBufferSize() != 0);

	soloud.deinit();
}

// Test parameter getters
//
// Soloud.getFilterParameter
// Soloud.getStreamTime
// Soloud.getPause
// Soloud.getVolume
// Soloud.getOverallVolume
// Soloud.getPan
// Soloud.getSamplerate
// Soloud.getProtectVoice
// Soloud.getActiveVoiceCount
// Soloud.getVoiceCount
// Soloud.isValidVoiceHandle
// Soloud.getRelativePlaySpeed
// Soloud.getPostClipScaler
// Soloud.getGlobalVolume
// Soloud.getMaxActiveVoiceCount
// Soloud.getLooping
void testGetters()
{
	float scratch[2048];
	SoLoud::result res;
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Sfxr sfxr;
	SoLoud::BiquadResonantFilter filter;
	res = soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::NULLDRIVER);
	CHECK_RES(res);
	res = sfxr.loadPreset(4, 0);
	CHECK_RES(res);
	sfxr.setFilter(0, &filter);

	CHECK(soloud.getActiveVoiceCount() == 0);
	CHECK(soloud.getVoiceCount() == 0);

	CHECK(soloud.isValidVoiceHandle((SoLoud::handle)0xbaadf00d) == 0);
	int h = soloud.play(sfxr);
	CHECK(soloud.isValidVoiceHandle(h));
	
	CHECK(soloud.getActiveVoiceCount() == 1);
	CHECK(soloud.getVoiceCount() == 1);

	float v_in, v_out;
	v_in = 0.7447f;
	soloud.setFilterParameter(h, 0, 0, v_in);
	v_out = soloud.getFilterParameter(h, 0, 0);
	CHECK(fabs(v_in - v_out) < 0.00001);
	
	CHECK(soloud.getStreamTime(h) < 0.00001);
	soloud.mix(scratch, 1000);
	CHECK(soloud.getStreamTime(h) > 0.00001);
	
	CHECK(soloud.getPause(h) == 0);
	soloud.setPause(h, true);
	CHECK(soloud.getPause(h) != 0);

	float oldvol = soloud.getOverallVolume(h);
	soloud.setVolume(h, v_in);
	v_out = soloud.getVolume(h);
	CHECK(fabs(v_in - v_out) < 0.00001);
	CHECK(fabs(oldvol - v_out) > 0.00001);

	soloud.setPan(h, v_in);
	CHECK(fabs(v_in - soloud.getPan(h)) < 0.00001);

	soloud.setSamplerate(h, v_in);
	CHECK(fabs(v_in - soloud.getSamplerate(h)) < 0.00001);

	CHECK(soloud.getProtectVoice(h) == 0);
	soloud.setProtectVoice(h, true);
	CHECK(soloud.getProtectVoice(h) != 0);

	soloud.setRelativePlaySpeed(h, v_in);
	CHECK(fabs(v_in - soloud.getRelativePlaySpeed(h)) < 0.00001);

	soloud.setPostClipScaler(v_in);
	CHECK(fabs(v_in - soloud.getPostClipScaler()) < 0.00001);

	soloud.setGlobalVolume(v_in);
	CHECK(fabs(v_in - soloud.getGlobalVolume()) < 0.00001);

	CHECK(soloud.getLooping(h) == 0);
	soloud.setLooping(h, true);
	CHECK(soloud.getLooping(h) != 0);

	CHECK(soloud.getMaxActiveVoiceCount() > 0);
	soloud.setMaxActiveVoiceCount(123);
	CHECK(soloud.getMaxActiveVoiceCount() == 123);

	soloud.deinit();
}

// Visualization API tests
//
// Soloud.setVisualizationEnable
// Soloud.calcFFT
// Soloud.getWave
// Bus.setVisualizationEnable
// Bus.calcFFT
// Bus.getWave
void testVis()
{
	float scratch[2048];
	SoLoud::result res;
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Sfxr sfxr;
	SoLoud::Bus bus;
	res = soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::NULLDRIVER);
	CHECK_RES(res);
	res = sfxr.loadPreset(4, 0);
	CHECK_RES(res);

	int bush = soloud.play(bus);
	int h = bus.play(sfxr);
	soloud.setVisualizationEnable(true);
	bus.setVisualizationEnable(true);

	soloud.mix(scratch, 1000);

	float *w = soloud.getWave();
	CHECK(w != NULL);
	if (w)
	{
		int i;
		int nonzero = 0;
		for (i = 0; i < 256; i++)
			if (w[i] != 0)
				nonzero = 1;
		CHECK(nonzero != 0);
	}

	w = bus.getWave();
	CHECK(w != NULL);
	if (w)
	{
		int i;
		int nonzero = 0;
		for (i = 0; i < 256; i++)
			if (w[i] != 0)
				nonzero = 1;
		CHECK(nonzero != 0);
	}

	w = soloud.calcFFT();
	CHECK(w != NULL);
	if (w)
	{
		int i;
		int nonzero = 0;
		for (i = 0; i < 256; i++)
			if (w[i] != 0)
				nonzero = 1;
		CHECK(nonzero != 0);
	}

	w = bus.calcFFT();
	CHECK(w != NULL);
	if (w)
	{
		int i;
		int nonzero = 0;
		for (i = 0; i < 256; i++)
			if (w[i] != 0)
				nonzero = 1;
		CHECK(nonzero != 0);
	}

	soloud.deinit();
}

// Test various play-related calls
// 
// Soloud.play
// Soloud.playClocked
// Soloud.playBackground
// Soloud.seek
// Soloud.stop
// Soloud.stopAll
// Soloud.stopAudioSource
// Bus.play
// Bus.playClocked
void testPlay()
{
	float scratch[2048];
	float ref[2048];
	SoLoud::result res;
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Wav wav;
	SoLoud::Bus bus;
	testWave(wav);
	res = soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::NULLDRIVER);
	CHECK_RES(res);

	int h = soloud.play(wav);
	soloud.mix(ref, 1000);
	CHECK_BUF_NONZERO(ref, 2000);
	soloud.stop(h);
	soloud.mix(scratch, 1000);
	CHECK_BUF_ZERO(scratch, 2000);
	h = soloud.play(wav);
	soloud.stopAudioSource(wav);
	soloud.mix(scratch, 1000);
	CHECK_BUF_ZERO(scratch, 2000);
	h = soloud.play(wav);
	soloud.stopAll();
	soloud.mix(scratch, 1000);
	CHECK_BUF_ZERO(scratch, 2000);

	h = soloud.playClocked(0.01, wav);
	soloud.stopAll();
	h = soloud.playClocked(0.015, wav);
	soloud.mix(scratch, 1000);
	CHECK_BUF_NONZERO(scratch, 2000);
	CHECK_BUF_DIFF(scratch, ref, 2000);
	soloud.stopAll();

	soloud.play(bus);
	h = bus.play(wav);
	soloud.mix(scratch, 1000);
	CHECK_BUF_SAME(scratch, ref, 2000); // TODO: fails, seems to offset by a sample when played through bus.

	soloud.play(bus);
	h = bus.playClocked(0.01, wav);
	soloud.stopAll();
	soloud.play(bus);
	h = bus.playClocked(0.015, wav);
	soloud.mix(scratch, 1000);
	CHECK_BUF_NONZERO(scratch, 2000);
	CHECK_BUF_DIFF(scratch, ref, 2000);
	soloud.stopAll();

	h = soloud.play(wav);
	soloud.seek(h, 0.1);
	soloud.mix(scratch, 1000);
	CHECK_BUF_NONZERO(scratch, 2000);
	CHECK_BUF_DIFF(scratch, ref, 2000);
	soloud.stopAll();
	
	h = soloud.playBackground(wav);
	soloud.mix(scratch, 1000);
	CHECK_BUF_NONZERO(scratch, 2000);
	CHECK_BUF_GTE(scratch, ref, 2000);

	soloud.deinit();
}

// Test various 3d functions
// 
// Soloud.play3d
// Soloud.play3dClocked
// Bus.play3d
// Bus.play3dClocked
// Soloud.set3dSoundSpeed
// Soloud.get3dSoundSpeed
// Soloud.set3dListenerParameters
// Soloud.set3dListenerPosition
// Soloud.set3dListenerAt
// Soloud.set3dListenerUp
// Soloud.set3dListenerVelocity
// Soloud.set3dSourceParameters
// Soloud.set3dSourcePosition
// Soloud.set3dSourceVelocity
// Soloud.set3dSourceMinMaxDistance
// Soloud.set3dSourceAttenuation
// Soloud.set3dSourceDopplerFactor
// Wav.set3dMinMaxDistance
// Wav.set3dAttenuation
// Wav.set3dDopplerFactor
// Wav.set3dProcessing
// Wav.set3dListenerRelative
// Wav.set3dDistanceDelay
// Wav.set3dCollider
// Wav.set3dAttenuator
void test3d()
{
	// TODO
}

// Test various filter options
//
// BiquadResonantFilter.setParams
// LofiFilter.setParams
// EchoFilter.setParams
// BassboostFilter.setParams
// FlangerFilter.setParams
// DCRemovalFilter.setParams
void testFilters()
{
	// TODO (basically check if filter does anything)
}

int main(int parc, char ** pars)
{
	testInfo();
	testGetters();
	testVis();
	testPlay();
	test3d();
	testFilters();

	printf("\n%d tests, %d error(s)\n", tests, errorcount);
	return 0;
}

/*
TODO:

Soloud.setSpeakerPosition
Soloud.setFilterParameter
Soloud.fadeFilterParameter
Soloud.oscillateFilterParameter
Soloud.setLooping
Soloud.setMaxActiveVoiceCount
Soloud.setInaudibleBehavior
Soloud.setGlobalVolume
Soloud.setPostClipScaler
Soloud.setPause
Soloud.setPauseAll
Soloud.setRelativePlaySpeed
Soloud.setProtectVoice
Soloud.setSamplerate
Soloud.setPan
Soloud.setPanAbsolute
Soloud.setVolume
Soloud.setDelaySamples
Soloud.fadeVolume
Soloud.fadePan
Soloud.fadeRelativePlaySpeed
Soloud.fadeGlobalVolume
Soloud.schedulePause
Soloud.scheduleStop
Soloud.oscillateVolume
Soloud.oscillatePan
Soloud.oscillateRelativePlaySpeed
Soloud.oscillateGlobalVolume
Soloud.setGlobalFilter
Soloud.getLoopCount
Soloud.getInfo
Soloud.createVoiceGroup
Soloud.destroyVoiceGroup
Soloud.addVoiceToGroup
Soloud.isVoiceGroup
Soloud.isVoiceGroupEmpty
Soloud.update3dAudio
Soloud.mix
Soloud.mixSigned16
AudioAttenuator.attenuate
Bus.setChannels
Bus.setVolume
Bus.setLooping
Bus.setFilter
Bus.set3dMinMaxDistance
Bus.set3dAttenuation
Bus.set3dDopplerFactor
Bus.set3dProcessing
Bus.set3dListenerRelative
Bus.set3dDistanceDelay
Bus.set3dCollider
Bus.set3dAttenuator
Bus.setInaudibleBehavior
Bus.stop
Speech.setText
Speech.setParams
Speech.setVolume
Speech.setLooping
Speech.set3dMinMaxDistance
Speech.set3dAttenuation
Speech.set3dDopplerFactor
Speech.set3dProcessing
Speech.set3dListenerRelative
Speech.set3dDistanceDelay
Speech.set3dCollider
Speech.set3dAttenuator
Speech.setInaudibleBehavior
Speech.setFilter
Speech.stop
Wav.load
Wav.loadMem
Wav.loadFile
Wav.getLength
Wav.setVolume
Wav.setLooping
Wav.setInaudibleBehavior
Wav.setFilter
Wav.stop
WavStream.load
WavStream.loadMem
WavStream.loadToMem
WavStream.loadFile
WavStream.loadFileToMem
WavStream.getLength
WavStream.setLoopRange
WavStream.setVolume
WavStream.setLooping
WavStream.set3dMinMaxDistance
WavStream.set3dAttenuation
WavStream.set3dDopplerFactor
WavStream.set3dProcessing
WavStream.set3dListenerRelative
WavStream.set3dDistanceDelay
WavStream.set3dCollider
WavStream.set3dAttenuator
WavStream.setInaudibleBehavior
WavStream.setFilter
WavStream.stop
Prg.rand
Prg.srand
Sfxr.resetParams
Sfxr.loadParams
Sfxr.loadParamsMem
Sfxr.loadParamsFile
Sfxr.loadPreset
Sfxr.setVolume
Sfxr.setLooping
Sfxr.set3dMinMaxDistance
Sfxr.set3dAttenuation
Sfxr.set3dDopplerFactor
Sfxr.set3dProcessing
Sfxr.set3dListenerRelative
Sfxr.set3dDistanceDelay
Sfxr.set3dCollider
Sfxr.set3dAttenuator
Sfxr.setInaudibleBehavior
Sfxr.setFilter
Sfxr.stop
Openmpt.load
Openmpt.loadMem
Openmpt.loadFile
Openmpt.setVolume
Openmpt.setLooping
Openmpt.set3dMinMaxDistance
Openmpt.set3dAttenuation
Openmpt.set3dDopplerFactor
Openmpt.set3dProcessing
Openmpt.set3dListenerRelative
Openmpt.set3dDistanceDelay
Openmpt.set3dCollider
Openmpt.set3dAttenuator
Openmpt.setInaudibleBehavior
Openmpt.setFilter
Openmpt.stop
Monotone.setParams
Monotone.load
Monotone.loadMem
Monotone.loadFile
Monotone.setVolume
Monotone.setLooping
Monotone.set3dMinMaxDistance
Monotone.set3dAttenuation
Monotone.set3dDopplerFactor
Monotone.set3dProcessing
Monotone.set3dListenerRelative
Monotone.set3dDistanceDelay
Monotone.set3dCollider
Monotone.set3dAttenuator
Monotone.setInaudibleBehavior
Monotone.setFilter
Monotone.stop
TedSid.load
TedSid.loadToMem
TedSid.loadMem
TedSid.loadFileToMem
TedSid.loadFile
TedSid.setVolume
TedSid.setLooping
TedSid.set3dMinMaxDistance
TedSid.set3dAttenuation
TedSid.set3dDopplerFactor
TedSid.set3dProcessing
TedSid.set3dListenerRelative
TedSid.set3dDistanceDelay
TedSid.set3dCollider
TedSid.set3dAttenuator
TedSid.setInaudibleBehavior
TedSid.setFilter
TedSid.stop
*/