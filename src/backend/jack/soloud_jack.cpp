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
#include <string>
#include <iostream>

namespace SoLoud
{
/*
    struct jack_client_t
    {
	    // other function
	    JackProcessCallback process;
    	void *process_arg;
    }*/

    static jack_client_t *jackClient = 0;
    static jack_port_t **jackPorts;
    static unsigned int ports;

    static void jack_cleanup(Soloud * /*aSoloud*/)
    {
    }

    int jack_callback(jack_nframes_t nframes, void*) {
        return 0;
    }

    result jack_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
        aSoloud->mBackendCleanupFunc = jack_cleanup;

        // Starting Jack client
        if ((jackClient = jack_client_open("Solound_Audio", JackNullOption, NULL)) == 0) return UNKNOWN_ERROR;

        ports = aChannels;
        jackPorts = new jack_port_t*[ports];
        // Registerring JACK Ports
        for (int i = 0; i < ports; i++) {
            jackPorts[i] = jack_port_register(jackClient, ("channel_" + std::to_string(i + 1)).c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        }

        // Activating Jack client
        jack_set_process_callback(jackClient, jack_callback, NULL);
        if (jack_activate(jackClient)) return UNKNOWN_ERROR;

        // Connecting to audio ports
        const char** audioPorts;
        if ((audioPorts = jack_get_ports(jackClient, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsPhysical | JackPortIsInput)) == NULL)
        {
            return UNKNOWN_ERROR; // Cannot find any physical audio playback ports
        } else
        {
            for (int n = 0; audioPorts[n] != NULL; n++)
            {
                int m = jack_connect(jackClient, jack_port_name(jackPorts[n % ports]), audioPorts[n]);
                // if (m) // Warning, cannot connect to audio output
            }
        }

        aSoloud->postinit(aSamplerate, aBuffer, aFlags, aChannels);
        aSoloud->mBackendString = "JACK driver";
        return SO_NO_ERROR;
    }
};
#endif
