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
#if !defined(WITH_PIPEWIRE)

namespace SoLoud
{
    result sdl2static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
    {
        return NOT_IMPLEMENTED;
    }
}

#else
#include "soloud_thread.h"

#include "pipewire/pipewire.h"
#include <spa/param/audio/format-utils.h>

namespace SoLoud
{
    class PipeWireBackendState
    {
        public:
            PipeWireBackendState()
            {
                m_PipewireParameterPODWrapper = spa_pod_builder();
                m_PipewireParameterPODWrapper.data = m_PipewireParameterBuffer;
                m_PipewireParameterPODWrapper.size = c_PipewireParameterBufferSize;

                m_PODParameters[0] = nullptr;
            }

            ~PipeWireBackendState()
            {
                StopProcessing();
            }

            bool SetSoloudEngineInstance(Soloud* connectedEngine)
            {
                m_SoundLoadEngineInstance = connectedEngine;
                return true;
            }

            bool SetOutputStream(unsigned int newNumberOfChannels, unsigned int newSampleRate)
            {
                m_OutputAudioStreamInformation = SPA_AUDIO_INFO_RAW_INIT();
                m_OutputAudioStreamInformation.format = SPA_AUDIO_FORMAT_F32;
                m_OutputAudioStreamInformation.channels = newNumberOfChannels;
                m_OutputAudioStreamInformation.rate = newSampleRate;
                return true;
            }

            bool StartProcessing()
            {
                //Ensure that a previous stream is not already running
                StopProcessing();
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

                m_PODParameters[0] = spa_format_audio_raw_build(&m_PipewireParameterPODWrapper,
                                                                SPA_PARAM_EnumFormat,
                                                                &m_OutputAudioStreamInformation);

                int errorCode = pw_stream_connect(m_OutputAudioStream,
                                  PW_DIRECTION_OUTPUT,
                                  PW_ID_ANY,
                                  static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS),
                                  m_PODParameters,
                                  c_PODParametersSize);

                pw_thread_loop_start(m_PipewireLoop);

                return false;
            }

            bool StopProcessing()
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

                if(m_PipewirePlaybackProperties)
                {
                    pw_properties_free(m_PipewirePlaybackProperties);
                    m_PipewirePlaybackProperties = nullptr;
                }

                m_PODParameters[0] = nullptr;

                return true;
            }

            bool clear()
            {
                m_SoundLoadEngineInstance = nullptr;
                m_DesiredBufferSize = 0;
                return true;
            }

            bool SetDesiredBufferSize(unsigned int newDesiredBufferSize)
            {
                m_DesiredBufferSize = newDesiredBufferSize;
                return true;
            }

            const uint32_t& GetNumerOfPlaybackChannels() const
            {
                return m_OutputAudioStreamInformation.channels;
            }

            static void PipewireMixSoloudData(void *userdata)
            {
                PipeWireBackendState* soloudPipewireState = reinterpret_cast<PipeWireBackendState*>(userdata);

                struct pw_buffer *pipewireBuffer;
                int numberOfFrames, stride;
                float* destinationBuffer;

                if ((pipewireBuffer = pw_stream_dequeue_buffer(soloudPipewireState->m_OutputAudioStream)) == NULL)
                {
                        pw_log_warn("out of buffers: %m");
                        return;
                }

                if ((destinationBuffer = reinterpret_cast<float*>(pipewireBuffer->buffer->datas[0].data)) == NULL)
                        return;

                stride = sizeof(float) * soloudPipewireState->GetNumerOfPlaybackChannels();
                numberOfFrames = soloudPipewireState->m_DesiredBufferSize / stride;

                soloudPipewireState->m_SoundLoadEngineInstance->mix(destinationBuffer, numberOfFrames);

                pipewireBuffer->buffer->datas[0].chunk->offset = 0;
                pipewireBuffer->buffer->datas[0].chunk->stride = stride;
                pipewireBuffer->buffer->datas[0].chunk->size = numberOfFrames * stride;

                int output = pw_stream_queue_buffer(soloudPipewireState->m_OutputAudioStream, pipewireBuffer);
            }

            constexpr static const struct pw_stream_events cs_StreamSoloudData ={
                    .destroy = nullptr,
                    .state_changed = nullptr,
                    .control_info = nullptr,
                    .io_changed = nullptr,
                    .param_changed = nullptr,
                    .add_buffer = nullptr,
                    .remove_buffer = nullptr,
                    .process = PipewireMixSoloudData,
                    .drained = nullptr,
                    .command = nullptr,
                    .trigger_done = nullptr
            };

        protected:
            static const size_t c_PipewireParameterBufferSize = 1024;
            uint8_t m_PipewireParameterBuffer[c_PipewireParameterBufferSize];
            struct spa_pod_builder m_PipewireParameterPODWrapper;
            struct pw_thread_loop *m_PipewireLoop{nullptr}; //Used to grab data to run

            struct pw_properties *m_PipewirePlaybackProperties{nullptr};

            struct pw_stream *m_OutputAudioStream{nullptr};
            struct spa_audio_info_raw m_OutputAudioStreamInformation{};

            static const unsigned int c_PODParametersSize = 1;
            const struct spa_pod* m_PODParameters[c_PODParametersSize];

            Soloud* m_SoundLoadEngineInstance{nullptr};
            unsigned int m_DesiredBufferSize{0};
    };

    static PipeWireBackendState s_PipewireState;

    static void pipewire_init_deinit(SoLoud::Soloud *aSoloud)
    {
        s_PipewireState.StopProcessing();
    }

    result pipewire_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
        pw_init(nullptr, nullptr); //Initialize the pipewire api
        s_PipewireState.SetSoloudEngineInstance(aSoloud);
        s_PipewireState.SetDesiredBufferSize(aBuffer);
        s_PipewireState.SetOutputStream(aChannels, aSamplerate);

        aSoloud->postinit(aSamplerate, aBuffer * aChannels, aFlags, aChannels);
        aSoloud->mBackendCleanupFunc = pipewire_init_deinit;
        aSoloud->mBackendString = "Pipewire";
        //aSoloud->mBackendData;
        s_PipewireState.StartProcessing();

        return SO_NO_ERROR;
    }
}
#endif
