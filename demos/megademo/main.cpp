/*
SoLoud audio engine
Copyright (c) 2013-2020 Jari Komppa

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

typedef void(*DemoMainloopFunc)();
DemoMainloopFunc DemoMainloopPtr;
int gArgc;
char **gArgv;

extern void DemoMainloop_multimusic();
extern int DemoEntry_multimusic(int argc, char *argv[]);
extern void DemoMainloop_monotone();
extern int DemoEntry_monotone(int argc, char *argv[]);
extern void DemoMainloop_mixbusses();
extern int DemoEntry_mixbusses(int argc, char *argv[]);
extern void DemoMainloop_test3d();
extern int DemoEntry_test3d(int argc, char *argv[]);
extern void DemoMainloop_pewpew();
extern int DemoEntry_pewpew(int argc, char *argv[]);
extern void DemoMainloop_radiogaga();
extern int DemoEntry_radiogaga(int argc, char *argv[]);
extern void DemoMainloop_space();
extern int DemoEntry_space(int argc, char *argv[]);
extern void DemoMainloop_speechfilter();
extern int DemoEntry_speechfilter(int argc, char *argv[]);
extern void DemoMainloop_tedsid();
extern int DemoEntry_tedsid(int argc, char *argv[]);
extern void DemoMainloop_virtualvoices();
extern int DemoEntry_virtualvoices(int argc, char *argv[]);
extern void DemoMainloop_wavformats();
extern int DemoEntry_wavformats(int argc, char *argv[]);
extern void DemoMainloop_speakers();
extern int DemoEntry_speakers(int argc, char *argv[]);
extern void DemoMainloop_thebutton();
extern int DemoEntry_thebutton(int argc, char *argv[]);
extern void DemoMainloop_annex();
extern int DemoEntry_annex(int argc, char* argv[]);
extern void DemoMainloop_filterfolio();
extern int DemoEntry_filterfolio(int argc, char* argv[]);

void DemoMainloop()
{
	DemoMainloopPtr();
}

void DemoMainloop_megademo()
{
	DemoUpdateStart();

	ONCE(ImGui::SetNextWindowPos(ImVec2(20, 0)));
	ImGui::Begin("Select sub-demo to run");

	if (ImGui::Button("Multimusic"))
	{
		DemoEntry_multimusic(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_multimusic;
	}
	ImGui::Text("Multimusic demo plays multiple music tracks\n"
		        "with interactive options to fade between them.");

	ImGui::Separator();

	if (ImGui::Button("Monotone"))
	{
		DemoEntry_monotone(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_monotone;
	}
	ImGui::Text("Monotone demo plays a \"monotone\" tracker song\n"
		        "with various interative options and filters.");

	ImGui::Separator();

	if (ImGui::Button("Mixbusses"))
	{
		DemoEntry_mixbusses(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_mixbusses;
	}
	ImGui::Text("Mixbusses creates three different mix busses,\n"
				"plays different sounds in each and lets user\n"
				"adjust the volume of the bus interactively.");

	ImGui::Separator();

	if (ImGui::Button("3d test"))
	{
		DemoEntry_test3d(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_test3d;
	}
	ImGui::Text("3d test demo is a test of the 3d sound.");

	ImGui::Separator();

	if (ImGui::Button("pewpew"))
	{
		DemoEntry_pewpew(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_pewpew;
	}
	ImGui::Text("pewpew demo shows the use of play_clocked\n"
				"and how delaying sound makes it feel like\n"
				"it plays earlier.");

	ImGui::Separator();

	if (ImGui::Button("radiogaga"))
	{
		DemoEntry_radiogaga(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_radiogaga;
	}
	ImGui::Text("radiogaga demo demonstrates audio queues\n"
				"to generate endless music by chaining random\n"
				"clips of music.\n");

	ImGui::Separator();

	if (ImGui::Button("space"))
	{
		DemoEntry_space(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_space;
	}
	ImGui::Text("space demo is a mockup of a conversation\n"
				"in a space game, and shows use of the\n"
				"visualization interfaces.");

	ImGui::Separator();

	if (ImGui::Button("speechfilter"))
	{
		DemoEntry_speechfilter(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_speechfilter;
	}
	ImGui::Text("speechfilter demonstrates various\n"
				"DSP effects on speech synthesis.");

	ImGui::Separator();

	if (ImGui::Button("tedsid"))
	{
		DemoEntry_tedsid(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_tedsid;
	}
	ImGui::Text("tedsid demonstrates the MOS TED and SID\n"
	            "synthesis engines.");

	ImGui::Separator();

	if (ImGui::Button("virtualvoices"))
	{
		DemoEntry_virtualvoices(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_virtualvoices;
	}
	ImGui::Text("virtualvoices demonstrates playing way\n"
	            "more sounds than is actually possible,\n"
				"and having the ones active that matter.");

	ImGui::Separator();

	if (ImGui::Button("wavformats"))
	{
		DemoEntry_wavformats(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_wavformats;
	}
	ImGui::Text("wavformats test plays files with\n"
				"all sorts of wave file formats.");

	ImGui::Separator();

	if (ImGui::Button("speakers"))
	{
		DemoEntry_speakers(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_speakers;
	}
	ImGui::Text("speakers test plays single sounds\n"
				"through surround speakers.\n");

	ImGui::Separator();

	if (ImGui::Button("thebutton"))
	{
		DemoEntry_thebutton(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_thebutton;
	}
	ImGui::Text("thebutton test shows one way of\n"
		        "avoiding actor speaking on top\n"
		        "of themselves.\n");

	ImGui::Separator();

	if (ImGui::Button("annex"))
	{
		DemoEntry_annex(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_annex;
	}
	ImGui::Text("annex test moves a live sound\n"
	"from one mixing bus to another.\n");

	ImGui::Separator();

	if (ImGui::Button("Filter folio"))
	{
		DemoEntry_filterfolio(gArgc, gArgv);
		DemoMainloopPtr = DemoMainloop_filterfolio;
	}
	ImGui::Text("Filter folio is a playground\n"
		"for various filters and their parameters.\n");

	ImGui::End();
	

	DemoUpdateEnd();
}

int DemoEntry(int argc, char *argv[])
{
	gArgc = argc;
	gArgv = argv;
	DemoMainloopPtr = DemoMainloop_megademo;
	return 0;
}

