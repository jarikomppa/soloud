/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "imgui.h"
#include "soloud_demo_framework.h"

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_padsynth.h"
#include "soloud_basicwave.h"
#include "soloud_echofilter.h"
#include "soloud_speech.h"
#include "soloud_fftfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_dcremovalfilter.h"

#include "RtMidi.h"

struct plonked
{
	int mHandle;
	float mRel;
};

SoLoud::Speech gSpeech;
SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Basicwave gWave;		// Simple wave audio source
SoLoud::Wav gLoadedWave;
SoLoud::Wav gPadsynth;
SoLoud::EchoFilter gEchoFilter;		// Simple echo filter
SoLoud::BiquadResonantFilter gBQRFilter;   // BQR filter
SoLoud::Bus gBus;
SoLoud::FFTFilter gFftFilter;
SoLoud::LofiFilter gLofiFilter;
SoLoud::DCRemovalFilter gDCRemovalFilter;

float gAttack = 0.02f;
float gRelease = 0.5f;

plonked gPlonked[128] = { 0 };
int gWaveSelect = 2;
int gFilterSelect = 0;
int gEcho = 0;
char *gInfo = (char*)"";

RtMidiIn *midi = NULL;

void plonk(float rel, float vol = 0x50)
{
	int i = 0;
	while (gPlonked[i].mHandle != 0 && i < 128) i++;
	if (i == 128) return;

	vol = (vol + 10) / (float)(0x7f + 10);
	vol *= vol;
	float pan = (float)sin(DemoTick() * 0.0234);
	int handle;
	if (gWaveSelect < 4)
	{
		handle = gBus.play(gWave, 0);
	}
	else
	{
		if (gWaveSelect == 4)
			handle = gBus.play(gLoadedWave, 0);
		if (gWaveSelect == 5)
		{
			handle = gBus.play(gPadsynth, 0);
		}
	}
	gSoloud.fadeVolume(handle, vol, gAttack);
	gSoloud.setRelativePlaySpeed(handle, 2 * rel);
	gPlonked[i].mHandle = handle;
	gPlonked[i].mRel = rel;
}

void unplonk(float rel)
{
	int i = 0;
	while (gPlonked[i].mRel != rel &&i < 128) i++;
	if (i == 128) return;
	gSoloud.fadeVolume(gPlonked[i].mHandle, 0, gRelease);
	gSoloud.scheduleStop(gPlonked[i].mHandle, gRelease);
	gPlonked[i].mHandle = 0;
}

void replonk(float vol = 0x50)
{
	int i = 0;
	while (gPlonked[i].mHandle != 0 && i < 128) i++;
	if (i == 128) return;

	vol = (vol + 10) / (float)(0x7f + 10);
	vol *= vol;
	for (i = 0; i < 128; i++)
	{
		if (gPlonked[i].mHandle != 0)
		{	
			gSoloud.fadeVolume(gPlonked[i].mHandle, vol, 0.1);
		}
	}
}

void say(const char *text)
{
	gInfo = (char*)text;
	gSpeech.setText(text);
	gSoloud.play(gSpeech, 4);
}

float harm[7] = { 0.7f, 0.3f, 0.2f, 1.7f, 0.4f, 1.3f, 0.2f };
float bw = 0.25f;
float bws = 1.0f;

int bushandle;

void midicallback(double deltatime, std::vector< unsigned char > *message, void *userData)
{
	if (((*message)[0] & 0xf0) == 0x90)
	{
		// some keyboards send volume 0 play instead of note off..
		if (((*message)[2]) != 0)
		{
			plonk((float)pow(0.943875f, 0x3c - (*message)[1]), (float)(*message)[2]);
		}
		else
		{
			unplonk((float)pow(0.943875f, 0x3c - (*message)[1]));
		}
	}
	// note off
	if (((*message)[0] & 0xf0) == 0x80)
	{
		unplonk((float)pow(0.943875f, 0x3c - (*message)[1]));
	}
	// aftertouch
	if (((*message)[0] & 0xf0) == 0xd0)
	{
		replonk((float)(*message)[1]);
	}
}

int DemoEntry(int argc, char *argv[])
{

	try
	{
		midi = new RtMidiIn();
	}
	catch (RtMidiError &error)
	{
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	unsigned int nPorts = midi->getPortCount();
	if (nPorts > 0)
	{
		midi->openPort();
		if (midi->isPortOpen())
		{
			midi->setCallback(midicallback);
			midi->ignoreTypes(false, true, false);
		}
	}

	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);
	//	gBus.setFilter(0, &gBQRFilter);
	//	gBus.setFilter(1, &gFilter);
	gEchoFilter.setParams(0.5f, 0.5f);
	bushandle = gSoloud.play(gBus);

	

	gSpeech.setFilter(1, &gFftFilter);

	gSpeech.setText(". . . . . . . . . . . . . . . Use keyboard to play!");
	gInfo = (char*)"Use keyboard to play!";
	gSoloud.play(gSpeech, 4);

	gLoadedWave.load("audio/AKWF_c604_0024.wav");
	gLoadedWave.setLooping(1);

	gBus.setFilter(0, &gLofiFilter);
	gBus.setFilter(1, &gEchoFilter);
	gBus.setFilter(3, &gDCRemovalFilter);

	SoLoud::generatePadsynth(gPadsynth, 7, harm, bw, bws);

	return 0;
}

float filter_param0[4] = { 0, 0, 1, 1 };
float filter_param1[4] = { 8000, 0, 1000, 0 };
float filter_param2[4] = { 3, 0, 2, 0 };

void DemoMainloop()
{
	gSoloud.setFilterParameter(bushandle, 0, 0, filter_param0[0]);
	gSoloud.setFilterParameter(bushandle, 1, 0, filter_param0[1]);
	gSoloud.setFilterParameter(bushandle, 2, 0, filter_param0[2]);
	gSoloud.setFilterParameter(bushandle, 3, 0, filter_param0[3]);

	gSoloud.setFilterParameter(bushandle, 0, 1, filter_param1[0]);
	gSoloud.setFilterParameter(bushandle, 0, 2, filter_param2[0]);


#define NOTEKEY(x, p)\
	if (gPressed[x] && !gWasPressed[x]) { plonk((float)pow(0.943875f, p)); gWasPressed[x] = 1; } \
	if (!gPressed[x] && gWasPressed[x]) { unplonk((float)pow(0.943875f, p)); gWasPressed[x] = 0; }

	NOTEKEY('1', 18); // F#
	NOTEKEY('q', 17); // G
	NOTEKEY('2', 16); // G#
	NOTEKEY('w', 15); // A
	NOTEKEY('3', 14); // A#
	NOTEKEY('e', 13); // B
	NOTEKEY('r', 12); // C
	NOTEKEY('5', 11); // C#
	NOTEKEY('t', 10); // D
	NOTEKEY('6', 9); // D#
	NOTEKEY('y', 8); // E
	NOTEKEY('u', 7); // F
	NOTEKEY('8', 6); // F#
	NOTEKEY('i', 5); // G
	NOTEKEY('9', 4); // G#
	NOTEKEY('o', 3); // A
	NOTEKEY('0', 2); // A#
	NOTEKEY('p', 1); // B

	DemoUpdateStart();

	float *buf = gSoloud.getWave();
	float *fft = gSoloud.calcFFT();

	ONCE(ImGui::SetNextWindowPos(ImVec2(320, 20)));
	ONCE(ImGui::SetNextWindowSize(ImVec2(200, 350)));
	ImGui::Begin("Padsynth");
	{
		int changed = 0;
		if (ImGui::DragFloat("Harmonic 1", &harm[0], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Harmonic 2", &harm[1], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Harmonic 3", &harm[2], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Harmonic 4", &harm[3], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Harmonic 5", &harm[4], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Harmonic 6", &harm[5], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Harmonic 7", &harm[6], 0.1f)) changed = 1;
		if (ImGui::DragFloat("Bandwidth", &bw, 0.1f)) changed = 1;
		if (ImGui::DragFloat("Bandwidth scale", &bws, 0.1f)) changed = 1;
		if (changed)
			SoLoud::generatePadsynth(gPadsynth, 5, harm, bw, bws);
	}
	ImGui::End();

	ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
	ImGui::Begin("Output");
	ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
	ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
	ImGui::Text("Active voices     : %d", gSoloud.getActiveVoiceCount());
	ImGui::Text("1 2 3   5 6   8 9 0");		
	ImGui::Text(" Q W E R T Y U I O P");
	ImGui::Text(gInfo);
	ImGui::End();

	ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
	ONCE(ImGui::SetNextWindowSize(ImVec2(300, 350)));
	ImGui::Begin("Control");

	if (ImGui::CollapsingHeader("Waveform",0,true,true))
	{
		if (ImGui::RadioButton("Sine", gWaveSelect == 0))
		{
			gWaveSelect = 0;
			gWave.setWaveform(SoLoud::Basicwave::SINE);
			say("Sine wave");
		}
		if (ImGui::RadioButton("Triangle", gWaveSelect == 1))
		{
			gWaveSelect = 1;
			gWave.setWaveform(SoLoud::Basicwave::TRIANGLE);
			say("Triangle wave");
		}
		if (ImGui::RadioButton("Square", gWaveSelect == 2))
		{
			gWaveSelect = 2;
			gWave.setWaveform(SoLoud::Basicwave::SQUARE);
			say("Square wave");
		}
		if (ImGui::RadioButton("Saw", gWaveSelect == 3))
		{
			gWaveSelect = 3;
			gWave.setWaveform(SoLoud::Basicwave::SAW);
			say("Saw wave");
		}
		if (ImGui::RadioButton("Looping sample", gWaveSelect == 4))
		{
			gWaveSelect = 4;
			say("Looping sample");
		}
		if (ImGui::RadioButton("Padsynth", gWaveSelect == 5))
		{
			gWaveSelect = 5;
			say("Padsynth");
		}
	}
		
	if (ImGui::CollapsingHeader("BQRFilter",0,true,true))
	{
		if (ImGui::RadioButton("None", gFilterSelect == 0))
		{
			gFilterSelect = 0;
			gBus.setFilter(2, 0);
			say("Filter disabled");
		}
		if (ImGui::RadioButton("Lowpass", gFilterSelect == 1))
		{
			gFilterSelect = 1;
			gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 1000, 2);
			gBus.setFilter(2, &gBQRFilter);
			say("Low pass filter");
		}
		if (ImGui::RadioButton("Highpass", gFilterSelect == 2))
		{
			gFilterSelect = 2;
			gBQRFilter.setParams(SoLoud::BiquadResonantFilter::HIGHPASS, 1000, 2);
			gBus.setFilter(2, &gBQRFilter);
			say("High pass filter");
		}
		if (ImGui::RadioButton("Bandpass", gFilterSelect == 3))
		{
			gFilterSelect = 3;
			gBQRFilter.setParams(SoLoud::BiquadResonantFilter::BANDPASS, 1000, 2);
			gBus.setFilter(2, &gBQRFilter);
			say("Band pass filter");
		}
		ImGui::SliderFloat("Wet##4", &filter_param0[2], 0, 1);
		filter_param1[2] = gSoloud.getFilterParameter(bushandle, 2, 2);
		if (ImGui::SliderFloat("Frequency##4", &filter_param1[2], 0, 8000))
		{
			gSoloud.setFilterParameter(bushandle, 2, 2, filter_param1[2]);
		}
		if (ImGui::SliderFloat("Resonance##4", &filter_param2[2], 1, 20))
		{
			gSoloud.setFilterParameter(bushandle, 2, 3, filter_param2[2]);
		}
		if (ImGui::Button("Oscillate +/- 1kHz"))
		{
			float from = filter_param1[2] - 500;
			if (from < 0) from = 0;
			gSoloud.oscillateFilterParameter(bushandle, 2, 2, from, from + 1000, 1);
		}
	}
	if (ImGui::CollapsingHeader("Lofi filter", 0, true, true))
	{
		ImGui::SliderFloat("Wet##2", &filter_param0[0], 0, 1);
		ImGui::SliderFloat("Rate##2", &filter_param1[0], 1000, 8000);
		ImGui::SliderFloat("Bit depth##2", &filter_param2[0], 0, 8);
	}
	if (ImGui::CollapsingHeader("Echo filter", 0, true, true))
	{
		ImGui::SliderFloat("Wet##3", &filter_param0[1], 0, 1);
	}
	if (ImGui::CollapsingHeader("DC Removal filter", 0, true, true))
	{
		ImGui::SliderFloat("Wet##1", &filter_param0[3], 0, 1);
	}


	ImGui::End();

	DemoUpdateEnd();
}
