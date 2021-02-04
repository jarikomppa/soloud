/*
SoLoud audio engine
Copyright (c) 2013-2021 Jari Komppa

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
#include "soloud_ay.h"


#include "soloud_bassboostfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_dcremovalfilter.h"
#include "soloud_echofilter.h"
#include "soloud_fftfilter.h"
#include "soloud_flangerfilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_robotizefilter.h"
#include "soloud_waveshaperfilter.h"
#include "soloud_eqfilter.h"

#include "RtMidi.h"

struct plonked
{
	int mHandle;
	float mRel;
};

SoLoud::Soloud gSoloud;
SoLoud::Basicwave gWave;
SoLoud::Ay gAy;
SoLoud::Wav gLoadedWave;
SoLoud::Wav gPadsynth;
SoLoud::Bus gBus;
SoLoud::Filter* gFilter[11];
int gFilterSelect[4] = { 0, 0, 0, 0 };

float gAttack = 0.02f;
float gRelease = 0.5f;
int gSynthEngine = 0;
bool gSynthWindow = true;
bool gInfoWindow = true;
bool gFilterWindow = false;

float harm[7] = { 0.7f, 0.3f, 0.2f, 1.7f, 0.4f, 1.3f, 0.2f };
float bw = 0.25f;
float bws = 1.0f;

float filter_param0[4] = { 0, 0, 1, 1 };
float filter_param1[4] = { 8000, 0, 1000, 0 };
float filter_param2[4] = { 3, 0, 2, 0 };

int bushandle;


plonked gPlonked[128] = { 0 };
int gWaveSelect = SoLoud::Soloud::WAVE_SQUARE;
char *gInfo = (char*)"";

RtMidiIn *midi = NULL;

void plonk(float rel, float vol = 0x50)
{
	int i = 0;
	while (i < 128 && gPlonked[i].mHandle != 0) i++;
	if (i == 128) return;

	vol = (vol + 10) / (float)(0x7f + 10);
	vol *= vol;
	float pan = (float)sin(DemoTick() * 0.0234);
	int handle;
	switch (gSynthEngine)
	{
	default:
	case 0:
		gWave.setFreq(440.0f * rel * 2);
		handle = gBus.play(gWave, 0);
		break;
	case 1:
		handle = gBus.play(gPadsynth, 0);
		gSoloud.setRelativePlaySpeed(handle, 2 * rel);
		break;
	case 2:
		handle = gBus.play(gLoadedWave, 0);
		gSoloud.setRelativePlaySpeed(handle, 2 * rel);
		break;
	case 3:
		handle = gBus.play(gAy, 0);
		break;
	}	
	gSoloud.fadeVolume(handle, vol, gAttack);
	gPlonked[i].mHandle = handle;
	gPlonked[i].mRel = rel;
}

void unplonk(float rel)
{
	int i = 0;
	while (i < 128 && gPlonked[i].mRel != rel) i++;
	if (i == 128) return;
	gSoloud.fadeVolume(gPlonked[i].mHandle, 0, gRelease);
	gSoloud.scheduleStop(gPlonked[i].mHandle, gRelease);
	gPlonked[i].mHandle = 0;
}


void say(const char *text)
{
	gInfo = (char*)text;
}


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
	bushandle = gSoloud.play(gBus);


	gInfo = (char*)"Use keyboard or midi to play!";

	gLoadedWave.load("audio/AKWF_c604_0024.wav");
	gLoadedWave.setLooping(1);

	SoLoud::generatePadsynth(gPadsynth, 7, harm, bw, bws);

	gFilter[0] = new SoLoud::BassboostFilter;
	gFilter[1] = new SoLoud::BiquadResonantFilter;
	gFilter[2] = new SoLoud::DCRemovalFilter;
	gFilter[3] = new SoLoud::EchoFilter;
	gFilter[4] = new SoLoud::FFTFilter;
	gFilter[5] = new SoLoud::FlangerFilter;
	gFilter[6] = new SoLoud::FreeverbFilter;
	gFilter[7] = new SoLoud::LofiFilter;
	gFilter[8] = new SoLoud::RobotizeFilter;
	gFilter[9] = new SoLoud::WaveShaperFilter;
	gFilter[10] = new SoLoud::EqFilter;

	return 0;
}


void waveform_window()
{
	ONCE(ImGui::SetNextWindowPos(ImVec2(320, 20)));
	ONCE(ImGui::SetNextWindowSize(ImVec2(200, 350)));
	ImGui::Begin("Waveform");

	if (ImGui::Combo("Wave", &gWaveSelect,
		"Square wave\x00"
		"Saw wave\x00"
		"Sine wave\x00"
		"Triangle wave\x00"
		"Bounce wave\x00"
		"Jaws wave\x00"
		"Humps wave\x00"
		"Antialized square wave\x00"
		"Antialiazed sawe wave\x00"
		"\x00"))
	{
		gWave.setWaveform(gWaveSelect);
	}

	ImGui::DragFloat("Attack", &gWave.mADSR.mA, 0.01f);
	ImGui::DragFloat("Decay", &gWave.mADSR.mD, 0.01f);
	ImGui::DragFloat("Sustain", &gWave.mADSR.mS, 0.01f);
	ImGui::DragFloat("Release", &gRelease, 0.01f);

	ImGui::End();
}

void info_window()
{
	float* buf = gSoloud.getWave();
	float* fft = gSoloud.calcFFT();

	ONCE(ImGui::SetNextWindowPos(ImVec2(520, 20)));
	ImGui::Begin("Output");
	ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
	ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
	ImGui::Text("Active voices     : %d", gSoloud.getActiveVoiceCount());
	ImGui::Text("1 2 3   5 6   8 9 0");
	ImGui::Text(" Q W E R T Y U I O P");
	ImGui::Text(gInfo);
	ImGui::End();
}

void padsynth_window()
{
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
}

void filter_window()
{
	ONCE(ImGui::SetNextWindowPos(ImVec2(320, 20)));
	ONCE(ImGui::SetNextWindowSize(ImVec2(200, 350)));
	ImGui::Begin("Filters");
	for (int filterindex = 0; filterindex < 4; filterindex++)
	{
		if (filterindex != 0)
			ImGui::Separator();

		char* label[4] = { "Filter 1", "Filter 2", "Filter 3", "Filter 4" };

		if (ImGui::Combo(label[filterindex], &gFilterSelect[filterindex],
			"None\x00"
			"BassboostFilter\x00"
			"BiquadResonantFilter\x00"
			"DCRemovalFilter\x00"
			"EchoFilter\x00"
			"FFTFilter\x00"
			"FlangerFilter\x00"
			"FreeverbFilter\x00"
			"LofiFilter\x00"
			"RobotizeFilter\x00"
			"WaveShaperFilter\x00"
			"EqFilter\x00\x00"))
		{
			if (gFilterSelect[filterindex])
				gSoloud.setGlobalFilter(filterindex, gFilter[gFilterSelect[filterindex] - 1]);
			else
				gSoloud.setGlobalFilter(filterindex, 0);
		}

		if (gFilterSelect[filterindex] != 0)
		{
			SoLoud::Filter* f = gFilter[gFilterSelect[filterindex] - 1];
			int count = f->getParamCount();
			for (int i = 0; i < count; i++)
			{
				int filtertype = f->getParamType(i);
				float filtermin = f->getParamMin(i);
				float filtermax = f->getParamMax(i);

				if (filtertype == SoLoud::Filter::INT_PARAM)
				{
					int v = (int)gSoloud.getFilterParameter(0, filterindex, i);
					char temp[128];
					sprintf(temp, "%s##%d-%d", f->getParamName(i), filterindex, i);
					if (ImGui::SliderInt(temp, &v, (int)filtermin, (int)filtermax))
					{
						gSoloud.setFilterParameter(0, filterindex, i, (float)v);
					}
				}

				if (filtertype == SoLoud::Filter::FLOAT_PARAM)
				{
					float v = gSoloud.getFilterParameter(0, filterindex, i);
					char temp[128];
					sprintf(temp, "%s##%d-%d", f->getParamName(i), filterindex, i);
					if (ImGui::SliderFloat(temp, &v, filtermin, filtermax))
					{
						gSoloud.setFilterParameter(0, filterindex, i, v);
					}
				}

				if (filtertype == SoLoud::Filter::BOOL_PARAM)
				{
					float v = gSoloud.getFilterParameter(0, filterindex, i);
					bool bv = v > 0.5f;
					char temp[128];
					sprintf(temp, "%s##%d-%d", f->getParamName(i), filterindex, i);
					if (ImGui::Checkbox(temp, &bv))
					{
						gSoloud.setFilterParameter(0, filterindex, i, bv ? 1.0f : 0.0f);
					}
				}
			}
		}
	}
	ImGui::End();
}

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

	ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
	ONCE(ImGui::SetNextWindowSize(ImVec2(300, 350)));
	ImGui::Begin("Master Control");	
	if (ImGui::Combo("Synth engine", &gSynthEngine,
		"Basic wave\x00"
		"Padsynth\x00"
		"Basic sample\x00"
		"Ay\x00"
		"\x00"))
	{
	}

	ImGui::Checkbox("Synth window", &gSynthWindow);
	ImGui::Checkbox("Info window", &gInfoWindow);
	ImGui::Checkbox("Filter window", &gFilterWindow);

	if (gInfoWindow)
		info_window();

	if (gFilterWindow)
		filter_window();

	if (gSynthWindow)
	{
		switch (gSynthEngine)
		{
		case 0:
			waveform_window();
			break;
		case 1:
			padsynth_window();
			break;
		}
	}


	ImGui::End();

	DemoUpdateEnd();
}
