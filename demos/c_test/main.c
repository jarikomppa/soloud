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

#include <stdio.h>
#include "soloud_c.h"

int main(int parc, char ** pars)
{
	int i = 0, spin = 0;
	Soloud *soloud = Soloud_create();
	Speech *speech = Speech_create();

	Speech_setText(speech, "1 2 3       A B C        Doooooo    Reeeeee    Miiiiii    Faaaaaa    Soooooo    Laaaaaa    Tiiiiii    Doooooo!");

	Soloud_initEx(soloud,SOLOUD_CLIP_ROUNDOFF | SOLOUD_ENABLE_VISUALIZATION, SOLOUD_AUTO, SOLOUD_AUTO, SOLOUD_AUTO, SOLOUD_AUTO);

	Soloud_setGlobalVolume(soloud, 4);
	Soloud_play(soloud, speech);

	printf("Playing..\n");

	while (Soloud_getVoiceCount(soloud) > 0)
	{
		int p;
		float * v = Soloud_calcFFT(soloud);
		printf("\r%c ", (int)("|\\-/"[spin & 3]));
		spin++;
		p = (int)(v[10] * 30);
		if (p > 59) p = 59;
		for (i = 0; i < p; i++)
			printf("=");
		for (i = p; i < 60; i++)
			printf(" ");
	}
	printf("\nFinished.\n");

	Soloud_deinit(soloud);
		
	Speech_destroy(speech);
	Soloud_destroy(soloud);

	printf("Cleanup done.\n");
	return 0;
}