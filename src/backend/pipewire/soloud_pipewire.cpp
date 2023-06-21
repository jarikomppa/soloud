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

#include "soloud.h"

#if !defined(WITH_PIPEWIRE)

namespace SoLoud
{
    result sdl2static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
    {
        return NOT_IMPLEMENTED;
    }
}

#else

#include "pipewire/pipewire.h"

#include <math.h>

#include <spa/param/audio/format-utils.h>

#define M_PI_M2 ( M_PI + M_PI )

#define DEFAULT_RATE            44100
#define DEFAULT_CHANNELS        2
#define DEFAULT_VOLUME          0.7

struct data {
        struct pw_main_loop *loop;
        struct pw_stream *stream;
        double accumulator;
};

/* [on_process] */
static void on_process(void *userdata)
{
        struct data *data = reinterpret_cast<struct data*>(userdata);
        struct pw_buffer *b;
        struct spa_buffer *buf;
        int i, c, n_frames, stride;
        int16_t *dst, val;

        if ((b = pw_stream_dequeue_buffer(data->stream)) == NULL) {
                pw_log_warn("out of buffers: %m");
                return;
        }

        buf = b->buffer;
        if ((dst = reinterpret_cast<int16_t*>(buf->datas[0].data)) == NULL)
                return;

        stride = sizeof(int16_t) * DEFAULT_CHANNELS;
        n_frames = buf->datas[0].maxsize / stride;

        for (i = 0; i < n_frames; i++) {
                data->accumulator += M_PI_M2 * 440 / DEFAULT_RATE;
                if (data->accumulator >= M_PI_M2)
                        data->accumulator -= M_PI_M2;

                val = sin(data->accumulator) * DEFAULT_VOLUME * 16767.f;
                for (c = 0; c < DEFAULT_CHANNELS; c++)
                        *dst++ = val;
        }

        buf->datas[0].chunk->offset = 0;
        buf->datas[0].chunk->stride = stride;
        buf->datas[0].chunk->size = n_frames * stride;

        pw_stream_queue_buffer(data->stream, b);
}
/* [on_process] */

static const struct pw_stream_events stream_events = {
        PW_VERSION_STREAM_EVENTS,
        .process = on_process,
};

int pwDemo(int argc, char *argv[])
{
        struct data data = { 0, };
        const struct spa_pod *params[1];
        uint8_t buffer[1024];
        struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

        pw_init(&argc, &argv);

        data.loop = pw_main_loop_new(NULL);

        data.stream = pw_stream_new_simple(
                        pw_main_loop_get_loop(data.loop),
                        "audio-src",
                        pw_properties_new(
                                PW_KEY_MEDIA_TYPE, "Audio",
                                PW_KEY_MEDIA_CATEGORY, "Playback",
                                PW_KEY_MEDIA_ROLE, "Music",
                                NULL),
                        &stream_events,
                        &data);

        spa_audio_info_raw dummy = SPA_AUDIO_INFO_RAW_INIT();// = SPA_AUDIO_INFO_RAW_INIT(.format = SPA_AUDIO_FORMAT_S16, .channels = DEFAULT_CHANNELS, .rate = DEFAULT_RATE );
        dummy.format = SPA_AUDIO_FORMAT_S16;
        dummy.channels = DEFAULT_CHANNELS;
        dummy.rate = DEFAULT_RATE;
        params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat,
                        &dummy);

        pw_stream_connect(data.stream,
                          PW_DIRECTION_OUTPUT,
                          PW_ID_ANY,
                          static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS),
                          params,
                          1);

        pw_main_loop_run(data.loop);

        pw_stream_destroy(data.stream);
        pw_main_loop_destroy(data.loop);

        return 0;
}

namespace SoLoud
{
    result pipewire_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
        pwDemo(0, nullptr);
        return NOT_IMPLEMENTED;
    }
}
#endif
