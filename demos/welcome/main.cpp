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

#include <stdlib.h>
#include <stdio.h>

#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_modplug.h"
#include "soloud_wav.h"
#include "soloud_thread.h"


// Entry point
int main(int argc, char *argv[])
{
	// Declare some variables
	SoLoud::Soloud soloud; // Engine core
	SoLoud::Speech speech; // A sound source (speech, in this case)
	SoLoud::Wav wav;       // One sample source
	SoLoud::Modplug mod;   // One song source

	// Initialize SoLoud (automatic back-end selection)
	// also, enable visualization for FFT calc
	soloud.init();
	soloud.setVisualizationEnable(1);
	printf("%s\n", "Welcome to Soloud!");

	// Load background sample
	wav.load("audio/windy_ambience.ogg");       // Load a wave file
	int handle1 = soloud.play(wav);             // Play it
	soloud.setVolume(handle1, 0.5f);            // Set volume; 1.0f is "normal"
	soloud.setPan(handle1, -0.2f);              // Set pan; -1 is left, 1 is right
	soloud.setRelativePlaySpeed(handle1, 0.9f); // Play a bit slower; 1.0f is normal

	// Configure sound source
	printf("%s\n", "What is your name?");
	char name[512];
	scanf("%s",name);
	speech.setText(name);
	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait for voice to finish
	while (soloud.getActiveVoiceCount() > 1)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Load song
	SoLoud::result loaded = mod.load("audio/BRUCE.S3M");
	if( SoLoud::SO_NO_ERROR == loaded ) {
		soloud.play(mod);
	} else {
		printf("%s\n", "Cannot find audio/BRUCE.S3M (or --with-libmodplug build option may be missing)");
	}
	
	// Wait 
	while (soloud.getActiveVoiceCount() > 0)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Clean up SoLoud
	soloud.deinit();

	// All done.
	return 0;
}