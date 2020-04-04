#include "sndbuffer.h"
#include "sndrender.h"

#include "malloc.h"
#include "memory.h"

SNDBUFFER::SNDBUFFER(unsigned size) {
        // size must be power of 2 for faster calc cyclic position: pos = (pos+1) & (size-1)
        if (size & (size-1)) {
                unsigned i = 1;
                for (i = 1; i < size; i *= 2);
                size = i;
        }
        SNDBUFFER::size = size;
        buffer = (SNDSAMPLE*)malloc(size * sizeof(SNDSAMPLE));
        reset();
}

SNDBUFFER::~SNDBUFFER()
{
        free(buffer);
}

void SNDBUFFER::reset()
{
        read_position = 0;
        memset(buffer, 0, size * sizeof(SNDSAMPLE));
}

void SNDBUFFER::get_buffers(bufptr_t& pos1, unsigned& size1, bufptr_t& pos2, unsigned& size2, unsigned count)
{
        pos1 = read_position;
        pos2 = 0;
        size1 = size - read_position;
        if (count < size1) {
                size1 = count;
                size2 = 0;
        } else {
                size2 = count-size1;
        }
}

void SNDBUFFER::samples_read(unsigned count)
{
        unsigned size1, size2, pos1, pos2;
        get_buffers(pos1, size1, pos2, size2, count);
        memset(buffer + pos1, 0, size1 * sizeof(SNDSAMPLE));
        if (size2) {
                memset(buffer + pos2, 0, size2 * sizeof(SNDSAMPLE));
        }
        read_position = (read_position + count) & (size-1);
}

void SNDBUFFER::count_start()
{
        samples_ready = size;
}

unsigned SNDBUFFER::count_single(const SNDRENDER& render)
{
        return (render.dstpos - read_position) & (size-1);
}

void SNDBUFFER::count(const SNDRENDER& render)
{
        unsigned samples = count_single(render);
        if (samples < samples_ready) samples_ready = samples;
}

unsigned SNDBUFFER::count_end()
{
        return samples_ready;
}
