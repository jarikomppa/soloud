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

#include "soloud.h"

#if !defined(WITH_JACK)

namespace SoLoud
{
    result jack_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
        return NOT_IMPLEMENTED;
    }
};

#else

#include <jack/jack.h>

namespace SoLoud
{
    static jack_client_t *jackAudioClient;
    static jack_port_t *jackAudioPort;

    static void jack_cleanup(Soloud * /*aSoloud*/)
    {
    }

    int jack_callback(jack_nframes_t nframes, void*) {

    }

    result jack_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
        aSoloud->mBackendCleanupFunc = jack_cleanup;

        // Connecting to JACK server
        jackAudioPort = jack_port_register(jackAudioClient, "Solound_Audio", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        jack_set_process_callback(jackAudioClient, jack_callback, NULL);
        if (jack_activate(jackAudioClient))
        {
            // Can't activate JACK
            return UNKNOWN_ERROR;
        }
        // Connecting to audio ports
        const char** audioPorts;
        if ((audioPorts = jack_get_ports(jackAudioClient, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsPhysical|JackPortIsInput)) == NULL)
        {
            // Cannot find any physical audio playback ports
            return UNKNOWN_ERROR;
        } else
        {
            for (int n = 0; audioPorts[n] != NULL; n++) // Connecting to all available physical audio playback ports
            {
                int m = jack_connect(jackAudioClient, jack_port_name(jackAudioPort), audioPorts[n]);
                // if (m) // Warning, cannot connect to audio output
            }
        }

        aSoloud->postinit(aSamplerate, aBuffer, aFlags, aChannels);
        aSoloud->mBackendString = "JACK driver";
        return SO_NO_ERROR;
    }
};
#endif
