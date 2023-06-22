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
#include <vector>
#include <memory>

#include <math.h>
#include <spa/param/audio/format-utils.h>

#define ENABLE_DEMO 0

#if ENABLE_DEMO
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

        constexpr const float M_PI_M2 = ( M_PI + M_PI );
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
        params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &dummy);

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
#endif
namespace SoLoud
{
    static void PipewireMixSoloudData(void *userdata);
    static const struct pw_stream_events cs_StreamSoloudData ={
            PW_VERSION_STREAM_EVENTS,
            .process = PipewireMixSoloudData,
    };

    class PipeWireBackendState
    {
        public:
            PipeWireBackendState() = default;
            ~PipeWireBackendState()
            {
                StopStream();
            }

            bool SetSoloudEngineInstance(Soloud* connectedEngine)
            {
                m_SoundLoadEngineInstance = connectedEngine;
                return true;
            }

            bool SetAudioBufferSize(const std::size_t newSize)
            {
                m_MixedBuffer.resize(newSize);
                m_MixedBuffer.shrink_to_fit();
                //Reinitialize the POD Wrapper
                m_MixedBufferPipewirePODWrapper = spa_pod_builder();
                m_MixedBufferPipewirePODWrapper.data = m_MixedBuffer.data();
                m_MixedBufferPipewirePODWrapper.size = m_MixedBuffer.size();

                return true;
            }

            bool SetOutputStream(unsigned int newNumberOfChannels, unsigned int newSampleRate)
            {
                m_OutputAudioStreamInformation = SPA_AUDIO_INFO_RAW_INIT();
                m_OutputAudioStreamInformation.format = SPA_AUDIO_FORMAT_S16;
                m_OutputAudioStreamInformation.channels = newNumberOfChannels;
                m_OutputAudioStreamInformation.rate = newSampleRate;
                return true;
            }

            bool StartSteam()
            {
                //Ensure that a previous stream is not already running
                StopStream();
                m_PipewireLoop = pw_main_loop_new(nullptr);
                m_PipewirePlaybackProperties = pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio",
                                                                 PW_KEY_MEDIA_CATEGORY, "Playback",
                                                                 PW_KEY_MEDIA_ROLE, "Game",
                                                                 PW_KEY_MODULE_NAME, "Soloud",
                                                                 nullptr);

                m_OutputAudioStream = pw_stream_new_simple(pw_main_loop_get_loop(m_PipewireLoop),
                                                           "audio-src",
                                                           m_PipewirePlaybackProperties,
                                                           &cs_StreamSoloudData,
                                                           this);

                m_PODParameters.emplace_back(spa_format_audio_raw_build(&m_MixedBufferPipewirePODWrapper, SPA_PARAM_EnumFormat, &m_OutputAudioStreamInformation));

                int errorCode = pw_stream_connect(m_OutputAudioStream,
                                  PW_DIRECTION_OUTPUT,
                                  PW_ID_ANY,
                                  static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS),
                                  m_PODParameters.data(),
                                  m_PODParameters.size());

                if(errorCode != 0)
                {
                    printf("WRC");
                }

                pw_main_loop_run(m_PipewireLoop);

                return false;
            }

            bool StopStream()
            {
                if(m_OutputAudioStream != nullptr)
                {
                    pw_stream_destroy(m_OutputAudioStream);
                    m_OutputAudioStream = nullptr;
                }

                if(m_PipewireLoop)
                {
                    pw_main_loop_destroy(m_PipewireLoop);
                    m_PipewireLoop = nullptr;
                }

                return true;
            }

        //protected:

        std::vector<uint8_t> m_MixedBuffer;
        struct spa_pod_builder m_MixedBufferPipewirePODWrapper;
        struct pw_main_loop *m_PipewireLoop{nullptr}; //Used to grab data to run

        struct pw_properties *m_PipewirePlaybackProperties{nullptr};

        struct pw_stream *m_OutputAudioStream{nullptr};
        struct spa_audio_info_raw m_OutputAudioStreamInformation{};
        std::vector<const struct spa_pod*> m_PODParameters;

        Soloud* m_SoundLoadEngineInstance{nullptr};

        double m_PipewireDemoAccumulator{0};

    };

    static void PipewireMixSoloudData(void *userdata)
    {
        PipeWireBackendState* soloudPipewireState = reinterpret_cast<PipeWireBackendState*>(userdata);

#if 1
        struct pw_buffer *b;
        struct spa_buffer *buf;
        int i, c, n_frames, stride;
        int16_t *dst, val;

        if ((b = pw_stream_dequeue_buffer(soloudPipewireState->m_OutputAudioStream)) == NULL) {
                pw_log_warn("out of buffers: %m");
                return;
        }

        buf = b->buffer;
        if ((dst = reinterpret_cast<int16_t*>(buf->datas[0].data)) == NULL)
                return;

        stride = sizeof(int16_t) * soloudPipewireState->m_OutputAudioStreamInformation.channels;
        n_frames = buf->datas[0].maxsize / stride;

        constexpr const float M_PI_M2 = ( M_PI + M_PI );
        for (i = 0; i < n_frames; i++) {
                soloudPipewireState->m_PipewireDemoAccumulator += M_PI_M2 * 440 / soloudPipewireState->m_OutputAudioStreamInformation.rate;
                if (soloudPipewireState->m_PipewireDemoAccumulator >= M_PI_M2)
                        soloudPipewireState->m_PipewireDemoAccumulator -= M_PI_M2;

                constexpr const float c_DefaultVolume = 0.7;
                val = sin(soloudPipewireState->m_PipewireDemoAccumulator) * c_DefaultVolume * 16767.f;
                for (c = 0; c < soloudPipewireState->m_OutputAudioStreamInformation.channels; c++)
                        *dst++ = val;
        }

        buf->datas[0].chunk->offset = 0;
        buf->datas[0].chunk->stride = stride;
        buf->datas[0].chunk->size = n_frames * stride;

        int output = pw_stream_queue_buffer(soloudPipewireState->m_OutputAudioStream, b);
#endif

    }

    static std::unique_ptr<PipeWireBackendState> s_PipewireState;

    result pipewire_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
#if ENABLE_DEMO
        pwDemo(0, nullptr);
#else
        pw_init(nullptr, nullptr); //Initialize the pipewire api
        s_PipewireState = std::make_unique<PipeWireBackendState>();
        s_PipewireState->SetSoloudEngineInstance(aSoloud);
        //Set the Audio Buffer size
        s_PipewireState->SetAudioBufferSize(aBuffer);
        s_PipewireState->SetOutputStream(aChannels, aSamplerate);
        s_PipewireState->StartSteam();

#endif
        return SO_NO_ERROR;
    }
}
#endif
