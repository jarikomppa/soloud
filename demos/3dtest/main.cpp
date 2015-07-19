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
#include "soloud_sfxr.h"
#include "soloud_speech.h"

SoLoud::Soloud gSoloud;
SoLoud::Sfxr gSfx_mouse, gSfx_orbit;
SoLoud::Speech gSfx_crazy;

int gSndHandle_mouse = 0;
int gSndHandle_orbit = 0;
int gSndHandle_crazy = 0;


int DemoEntry(int argc, char *argv[])
{
	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
	gSoloud.setGlobalVolume(4);

	gSfx_mouse.loadPreset(SoLoud::Sfxr::LASER, 3);
	gSfx_mouse.setLooping(1);
	gSfx_mouse.set3dMinMaxDistance(1, 200);
	gSfx_mouse.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
	gSndHandle_mouse = gSoloud.play3d(gSfx_mouse, 100, 0, 0);

	gSfx_orbit.loadPreset(SoLoud::Sfxr::COIN, 3);
	gSfx_orbit.setLooping(1);
	gSfx_orbit.set3dMinMaxDistance(1, 200);
	gSfx_orbit.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
	gSndHandle_orbit = gSoloud.play3d(gSfx_orbit, 50, 0, 0);

	gSfx_crazy.setText("I'm going into space with my space ship space ship space ship spaceeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
	gSfx_crazy.setLooping(1);
	gSfx_crazy.set3dMinMaxDistance(1, 400);
	gSfx_crazy.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.25);
	gSndHandle_crazy = gSoloud.play3d(gSfx_crazy, 50, 0, 0);
	return 0;
}


bool orbit_enable = 1;
bool crazy_enable = 1;
bool mouse_enable = 1;

void DemoMainloop()
{
	DemoUpdateStart();

	gSoloud.setPause(gSndHandle_crazy, !crazy_enable);
	gSoloud.setPause(gSndHandle_orbit, !orbit_enable);
	gSoloud.setPause(gSndHandle_mouse, !mouse_enable);

	float tick = DemoTick() / 1000.0f;

	float crazyx = (float)(sin(tick) * sin(tick * 0.234) * sin(tick * 4.234) * 150);
	float crazyz = (float)(cos(tick) * cos(tick * 0.234) * cos(tick * 4.234) * 150 - 50);
	float tickd = tick - 0.1f;
	float crazyxv = (float)(sin(tickd) * sin(tickd * 0.234) * sin(tickd * 4.234) * 150);
	float crazyzv = (float)(cos(tickd) * cos(tickd * 0.234) * cos(tickd * 4.234) * 150 - 50);
	crazyxv = crazyxv - crazyx;
	crazyzv = crazyzv - crazyz;

	gSoloud.set3dSourceParameters(gSndHandle_crazy, crazyx, 0, crazyz, crazyxv, 0, crazyzv);

	float orbitx = (float)sin(tick) * 50;
	float orbitz = (float)cos(tick) * 50;
	float orbitxv = (float)sin(tickd) * 50;
	float orbitzv = (float)cos(tickd) * 50;
	orbitxv = orbitxv - orbitx;
	orbitzv = orbitzv - orbitz;
		
	gSoloud.set3dSourceParameters(gSndHandle_orbit, orbitx, 0, orbitz, orbitxv, 0, orbitzv);

	float mousex = gMouseX - 400.0f;
	float mousez = gMouseY - 200.0f;

	gSoloud.set3dSourcePosition(gSndHandle_mouse, mousex, mousez, 0);

	gSoloud.update3dAudio();

	DemoTriangle(
		5+400, 5+200 - 20,
		5+400 - 20, 5+200 + 20,
		5+400 + 20, 5+200 + 20,
		0x77000000);
	DemoTriangle(
		400, 200 - 20,
		400 - 20, 200 + 20,
		400 + 20, 200 + 20,
		0xffeeeeee);

	DemoTriangle(
		5+400 + orbitx * 2, 5+200 + orbitz * 2 - 10,
		5+400 + orbitx * 2 - 10, 5+200 + orbitz * 2 + 10,
		5+400 + orbitx * 2 + 10, 5+200 + orbitz * 2 + 10,
		0x77000000);
	DemoTriangle(
		400 + orbitx*2, 200 + orbitz*2-10,
		400 + orbitx*2-10, 200 + orbitz*2+10,
		400 + orbitx*2+10, 200 + orbitz*2+10,
		0xffffff00);

	DemoTriangle(
		5 + 400 + crazyx * 2, 5 + 200 + crazyz * 2 - 10,
		5 + 400 + crazyx * 2 - 10, 5 + 200 + crazyz * 2 + 10,
		5 + 400 + crazyx * 2 + 10, 5 + 200 + crazyz * 2 + 10,
		0x77000000);
	DemoTriangle(
		400 + crazyx * 2, 200 + crazyz * 2 - 10,
		400 + crazyx * 2 - 10,200 + crazyz * 2 + 10,
		400 + crazyx * 2 + 10,200 + crazyz * 2 + 10,
		0xffff00ff);

	DemoTriangle(
		5 + 400 + mousex, 5 + 200 + mousez - 10,
		5 + 400 + mousex - 10, 5 + 200 + mousez + 10,
		5 + 400 + mousex + 10, 5 + 200 + mousez + 10,
		0x77000000);
	DemoTriangle(
		400 + mousex, 200 + mousez - 10,
		400 + mousex - 10, 200 + mousez + 10,
		400 + mousex + 10, 200 + mousez + 10,
		0xff00ffff);

	float *buf = gSoloud.getWave();
	float *fft = gSoloud.calcFFT();

	ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
	ImGui::Begin("Output");
	ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
	ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
	ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
	ImGui::End();

	ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
	ImGui::Begin("Control");
	ImGui::Checkbox("Orbit sound", &orbit_enable);
	ImGui::Checkbox("Crazy sound", &crazy_enable);
	ImGui::Checkbox("Mouse sound", &mouse_enable);

	ImGui::End();
	DemoUpdateEnd();
}
