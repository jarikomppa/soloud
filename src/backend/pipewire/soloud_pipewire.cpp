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
#include "soloud_thread.h"

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
                ;
                m_OutputAudioStreamInformation.format = SPA_AUDIO_FORMAT_F32;
                m_OutputAudioStreamInformation.channels = newNumberOfChannels;
                m_OutputAudioStreamInformation.rate = newSampleRate;
                return true;
            }

            bool StartSteam()
            {
                //Ensure that a previous stream is not already running
                StopStream();
                m_PipewireLoop = pw_thread_loop_new("SoLoud Pipewire Thread", nullptr);
                m_PipewirePlaybackProperties = pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio",
                                                                 PW_KEY_MEDIA_CATEGORY, "Playback",
                                                                 PW_KEY_MEDIA_ROLE, "Game",
                                                                 PW_KEY_MODULE_NAME, "Soloud",
                                                                 nullptr);

                m_OutputAudioStream = pw_stream_new_simple(pw_thread_loop_get_loop(m_PipewireLoop),
                                                           "Soloud",
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

                pw_thread_loop_start(m_PipewireLoop);

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
                    pw_thread_loop_stop(m_PipewireLoop);
                    m_PipewireLoop = nullptr;
                }

                return true;
            }

        //protected:

        std::vector<float> m_MixedBuffer;
        struct spa_pod_builder m_MixedBufferPipewirePODWrapper;
        struct pw_thread_loop *m_PipewireLoop{nullptr}; //Used to grab data to run

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

        struct pw_buffer *b;
        int n_frames, stride;
        float *dst, val;

        if ((b = pw_stream_dequeue_buffer(soloudPipewireState->m_OutputAudioStream)) == NULL) {
                pw_log_warn("out of buffers: %m");
                return;
        }

        if ((dst = reinterpret_cast<float*>(b->buffer->datas[0].data)) == NULL)
                return;

        const int maxDatas = b->buffer->n_datas;
        const int maxSize = b->buffer->datas->maxsize;
        stride = sizeof(float) * soloudPipewireState->m_OutputAudioStreamInformation.channels;
        n_frames = b->buffer->datas[0].maxsize / stride;

        memset(dst, 0, b->buffer->datas[0].maxsize);
        soloudPipewireState->m_SoundLoadEngineInstance->mix(dst, n_frames);

        b->buffer->datas[0].chunk->offset = 0;
        b->buffer->datas[0].chunk->stride = stride;
        b->buffer->datas[0].chunk->size = n_frames * stride;

        int output = pw_stream_queue_buffer(soloudPipewireState->m_OutputAudioStream, b);
    }

    static std::unique_ptr<PipeWireBackendState> s_PipewireState;
    result pipewire_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
        pw_init(nullptr, nullptr); //Initialize the pipewire api
        s_PipewireState = std::make_unique<PipeWireBackendState>();
        s_PipewireState->SetSoloudEngineInstance(aSoloud);
        //Set the Audio Buffer size
        s_PipewireState->SetAudioBufferSize(aBuffer);
        s_PipewireState->SetOutputStream(aChannels, aSamplerate);

        aSoloud->postinit(aSamplerate, aBuffer * aChannels, aFlags, aChannels);
        s_PipewireState->StartSteam();

        return SO_NO_ERROR;
    }
}
#endif
