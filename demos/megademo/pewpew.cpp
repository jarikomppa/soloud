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

namespace pewpew
{

	SoLoud::Soloud gSoloud;
	SoLoud::Sfxr gSfx;

#define MAX_BULLETS 64
	int bulletidx = 0;
	float bulletx[MAX_BULLETS]; // bullet x coordinate
	float bullety[MAX_BULLETS]; // bullet y coordinate
	float bulletv[MAX_BULLETS]; // bullet y velocity
	int bulletc[MAX_BULLETS];   // bullet spawned via 'clocked'

	int lasttick;

	int DemoEntry(int argc, char *argv[])
	{
		int i;
		for (i = 0; i < MAX_BULLETS; i++)
			bullety[i] = 0;

		// Use a slightly larger audio buffer to exaggarate the effect
		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, 0, 0, 4096);
		gSfx.loadPreset(SoLoud::Sfxr::LASER, 3);

		lasttick = DemoTick();
		return 0;
	}


	bool fire1 = 0;
	bool fire2 = 0;
	bool fire3 = 0;

	float x = 0;

	void DemoMainloop()
	{
		int i;
		int tick = DemoTick();
		x = (gMouseX - 400.0f) / 200.0f;


		if (lasttick >= tick)
		{
			DemoYield();
		}
		else
		{
			while (lasttick < tick)
			{
				if (x < -1) x = -1;
				if (x > 1) x = 1;
				gSfx.loadPreset(SoLoud::Sfxr::LASER, 3);
				if (fire1)
				{
					gSoloud.playClocked(DemoTick() / 1000.0f, gSfx, 1, x);
					bulletc[bulletidx] = 1;
				}

				if (fire2)
				{
					gSoloud.play(gSfx, 1, x);
					bulletc[bulletidx] = 0;
				}

				if (fire3)
				{
					gSoloud.playClocked(DemoTick() / 1000.0f, gSfx, 1, x);
					bulletc[bulletidx] = 1;
				}

				if (fire1 || fire2 || fire3)
				{
					bullety[bulletidx] = 350;
					bulletx[bulletidx] = x;
					bulletv[bulletidx] = -25;
					bulletidx = (bulletidx + 1) % MAX_BULLETS;
				}

				if (fire1) fire1 = 0;

				for (i = 0; i < MAX_BULLETS; i++)
				{
					if (bullety[i])
					{
						bullety[i] += bulletv[i];
						bulletv[i] += 1;
					}
					if (bullety[i] > 400)
					{
						bullety[i] = 0;
						int v;
						if (bulletc[i])
							v = gSoloud.playClocked(lasttick / 1000.0f, gSfx, 1, bulletx[i]);
						else
							v = gSoloud.play(gSfx, 1, bulletx[i]);
						gSoloud.setRelativePlaySpeed(v, 0.5f);
					}
				}

				lasttick += 10;
			}

			DemoUpdateStart();
			DemoTriangle(400 + x * 200, 350,
				375 + x * 200, 400,
				425 + x * 200, 400,
				0xff3399ff);

			for (i = 0; i < MAX_BULLETS; i++)
			{
				if (bullety[i] != 0)
				{
					DemoTriangle(400 + bulletx[i] * 200, bullety[i],
						400 - 2.5f + bulletx[i] * 200, 10 + bullety[i],
						400 + 2.5f + bulletx[i] * 200, 10 + bullety[i],
						0xff773333);
				}
			}

			float *buf = gSoloud.getWave();
			float *fft = gSoloud.calcFFT();

			ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
			ImGui::Begin("Output");
			ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
			ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
			ImGui::Text("Active voices     : %d", gSoloud.getActiveVoiceCount());
			ImGui::Text("----|----|----|----|----|");

			char temp[200];
			for (i = 0; i < (signed)gSoloud.getActiveVoiceCount(); i++)
				temp[i] = '-';
			temp[i] = 0;
			ImGui::Text(temp);

			ImGui::End();

			ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
			ImGui::Begin("Control");
			ImGui::Text("Click to play a single sound:");
			if (ImGui::Button("Play (single)"))
			{
				fire1 = 1;
			}
			ImGui::Separator();
			ImGui::Text("\nCheckbox for repeated calls:");
			ImGui::Checkbox("Play", &fire2);
			ImGui::Checkbox("PlayClocked", &fire3);

			ImGui::End();
			DemoUpdateEnd();
		}
	}
}

int DemoEntry_pewpew(int argc, char *argv[])
{
	return pewpew::DemoEntry(argc, argv);
}

void DemoMainloop_pewpew()
{
	return pewpew::DemoMainloop();
}