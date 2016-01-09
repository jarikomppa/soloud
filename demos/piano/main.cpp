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
#include "soloud_basicwave.h"
#include "soloud_echofilter.h"
#include "soloud_speech.h"
#include "soloud_fftfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_dcremovalfilter.h"

#ifdef USE_PORTMIDI
#include "portmidi.h"
#include <Windows.h>
#endif

struct plonked
{
	int mHandle;
	float mRel;
};

SoLoud::Speech gSpeech;
SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Basicwave gWave;		// Simple wave audio source
SoLoud::Wav gLoadedWave;
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
char *gInfo = "";

#ifdef USE_PORTMIDI
PmStream *midi = NULL;
#endif

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
		handle = gBus.play(gLoadedWave, 0);
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

void say(char *text)
{
	gInfo = text;
	gSpeech.setText(text);
	gSoloud.play(gSpeech, 4);
}


#ifdef USE_PORTMIDI
PmEvent buffer[1];
#endif

int bushandle;

int DemoEntry(int argc, char *argv[])
{

#ifdef USE_PORTMIDI
	Pm_OpenInput(&midi, Pm_GetDefaultInputDeviceID(), NULL, 100, NULL, NULL);
	if (midi)
	{
		Pm_SetFilter(midi, PM_FILT_REALTIME);
		while (Pm_Poll(midi)) {
			Pm_Read(midi, buffer, 1);
		}
	}
#endif

	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);
	//	gBus.setFilter(0, &gBQRFilter);
	//	gBus.setFilter(1, &gFilter);
	gEchoFilter.setParams(0.5f, 0.5f);
	bushandle = gSoloud.play(gBus);

	gSpeech.setFilter(1, &gFftFilter);

	gSpeech.setText(". . . . . . . . . . . . . . . Use keyboard to play!");
	gInfo = "Use keyboard to play!";
	gSoloud.play(gSpeech, 4);

	gLoadedWave.load("audio/AKWF_c604_0024.wav");
	gLoadedWave.setLooping(1);

	gBus.setFilter(0, &gLofiFilter);
	gBus.setFilter(1, &gEchoFilter);
	gBus.setFilter(3, &gDCRemovalFilter);

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


#ifdef USE_PORTMIDI
	if (midi)
	{
		int i = Pm_Poll(midi);
		if (i)
		{
			i = Pm_Read(midi, buffer, 1);
			if (i)
			{
				char temp[200];
				sprintf(temp, "\n%x %x %x", Pm_MessageStatus(buffer[0].message), Pm_MessageData1(buffer[0].message), Pm_MessageData2(buffer[0].message));
				OutputDebugStringA(temp);
				if (Pm_MessageStatus(buffer[0].message) == 0x90)
				{
					// some keyboards send volume 0 play instead of note off..
					if (Pm_MessageData2(buffer[0].message) != 0)
					{
						plonk((float)pow(0.943875f, 0x3c - Pm_MessageData1(buffer[0].message)), (float)Pm_MessageData2(buffer[0].message));
					}
					else
					{
						unplonk((float)pow(0.943875f, 0x3c - Pm_MessageData1(buffer[0].message)));
					}
				}
				// note off
				if (Pm_MessageStatus(buffer[0].message) == 0x80)
				{
					unplonk((float)pow(0.943875f, 0x3c - Pm_MessageData1(buffer[0].message)));
				}
				// aftertouch
				if (Pm_MessageStatus(buffer[0].message) == 0xd0)
				{
					replonk((float)Pm_MessageData1(buffer[0].message));
				}
			}
		}
	}
#endif
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
			gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 1000, 2);
			gBus.setFilter(2, &gBQRFilter);
			say("Low pass filter");
		}
		if (ImGui::RadioButton("Highpass", gFilterSelect == 2))
		{
			gFilterSelect = 2;
			gBQRFilter.setParams(SoLoud::BiquadResonantFilter::HIGHPASS, 44100, 1000, 2);
			gBus.setFilter(2, &gBQRFilter);
			say("High pass filter");
		}
		if (ImGui::RadioButton("Bandpass", gFilterSelect == 3))
		{
			gFilterSelect = 3;
			gBQRFilter.setParams(SoLoud::BiquadResonantFilter::BANDPASS, 44100, 1000, 2);
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
