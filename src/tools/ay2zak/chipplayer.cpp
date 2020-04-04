#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "chipplayer.h"

void ChipPlayer::reg_out(uint64_t tick, unsigned char reg, unsigned char val)
{
    static int lasttick = 0;
        if (dumpsize >= dump_alloc)
                dump = (REG_OUT*)realloc(dump, (dump_alloc = 2*dump_alloc + 0x10000) * sizeof(REG_OUT));
        REG_OUT *item = dump + dumpsize; // `volatile` disables optimizations, that can ruin writes to overlapped union
        if (tick != (int)tick) printf("Tick too big\n");
        lasttick = (int)tick;
        item->time = (int)tick;
        item->num = reg;
        item->val = val;
        dumpsize++;
}

ChipPlayer::ChipPlayer()
        : buffer(8192), chip(buffer), chip2(buffer)
{
        loop = 0; loop_enabled = false;
        ts_mode = false;

        dumpsize = dump_alloc = 0;
        dump = NULL;
        set_timings(SNDR_DEFAULT_SYSTICK_RATE, SNDR_DEFAULT_AY_RATE, SNDR_DEFAULT_SAMPLE_RATE);
        set_volumes(SNDCHIP::CHIP_YM, 0x7FFF, SNDR_VOL_YM, SNDR_PAN_ABC);
}

ChipPlayer::~ChipPlayer()
{
        free(dump);
}

unsigned ChipPlayer::get_time(uint64_t tick)
{
        return (unsigned) ((tick*1000)/system_clock_rate);
}

unsigned ChipPlayer::get_pos()
{
        if (playpos >= dumpsize) return get_len();
        return get_time(dump[playpos].time);
}

unsigned ChipPlayer::get_len()
{
        if (!dumpsize) return 0;
        return get_time(dump[dumpsize-1].time);
}

void ChipPlayer::seek(unsigned time_ms)
{
        chip.reset();
        chip2.reset();
        playtick = (uint64_t)time_ms * system_clock_rate / 1000;
        playpos = locate(playtick);
        unsigned regs_filled = 0;
        unsigned mask = ts_mode ? 0x3FFF3FFF : 0x3FFF; // 1st 14 regs in one or two chips
        // go back on song and get AY registers, written before seek point
        for (unsigned pos = playpos; pos; pos--) {
                unsigned char reg = dump[pos].num;
                if (regs_filled & (1 << reg)) continue;
                if (ts_mode && reg >= 0x10) {
                        chip2.select(reg-0x10);
                        chip2.write(0, dump[pos].val);
                } else {
                        chip.select(reg);
                        chip.write(0, dump[pos].val);
                }
                regs_filled |= (1 << reg);
                if ((regs_filled & mask) == mask) break;
        }
}

// locate first REG_OUT with timestamp >= tick,
// using binary search
unsigned ChipPlayer::locate(uint64_t tick)
{
        unsigned imin = 0, imax = dumpsize;
        for (;;) {
                if (imin >= imax || dump[imin].time >= tick) break;
                unsigned midpos = (imin+imax)/2;
                if (dump[midpos].time < tick) imin = midpos+1; else imax = midpos;
        }
        while (imin && dump[imin-1].time >= tick) imin--;
        return imin;
}

void ChipPlayer::set_timings(unsigned system_clock_rate, unsigned chip_clock_rate, unsigned sample_rate)
{
        ChipPlayer::system_clock_rate = system_clock_rate;
        ticks_per_buffer = (unsigned) ((uint64_t)system_clock_rate * (buffer.size - 20) / sample_rate);
        ticks_per_buffer /= 2; // for safety
        chip.set_timings(system_clock_rate, chip_clock_rate, sample_rate);
        chip2.set_timings(system_clock_rate, chip_clock_rate, sample_rate);
        playtick = 0; playpos = 0;
        buffer.reset();
}

const int VOL_DIV = 2; // mixer just adds samples. to avoid overflow, we lower volume then revert back with saturation

void ChipPlayer::set_volumes(SNDCHIP::CHIP_TYPE t, unsigned global_vol, const SNDCHIP_VOLTAB *vt, const SNDCHIP_PANTAB *pt)
{
        chip.set_chip(t);
        chip.set_volumes(global_vol / VOL_DIV, vt, pt);
        chip2.set_chip(t);
        chip2.set_volumes(global_vol / VOL_DIV, vt, pt);
}


inline int _min(int a, int b) { return (a < b)? a : b; }

void ChipPlayer::play(SNDSAMPLE *dst, unsigned need_samples)
{
        while (need_samples) {

                // read from buffer
                buffer.count_start();
                buffer.count(chip);
                if (ts_mode) buffer.count(chip2);
                unsigned ready = buffer.count_end();
                unsigned from_buffer = _min(ready, need_samples);

                for (unsigned i = 0; i < from_buffer; i++) {
                        // normalize (volume divided by 4)
                        SNDSAMPLE& s = buffer.buffer[(buffer.read_position + i) & (buffer.size - 1)];
                        //*dst++ = s;
                        int l = VOL_DIV * (int)s.ch.left;
                        if (l > 0x7FFF) l = 0x7FFF;
                        if (l < -0x7FFF) l = -0x7FFF;
                        int r = VOL_DIV * (int)s.ch.right;
                        if (r > 0x7FFF) r = 0x7FFF;
                        if (r < -0x7FFF) r = -0x7FFF;
                        *(uint32_t*)dst++ = (uint16_t)l + ((uint16_t)r << 16);
                }
                buffer.samples_read(from_buffer);

                need_samples -= from_buffer;
                if (!need_samples) return;

                if (playpos >= dumpsize) { // EOF - can't emulate more
                        while (need_samples) *(uint32_t*)dst++ = 0, need_samples--;
                        return;
                }

                // render to buffer
                chip.start_frame();
                if (ts_mode) chip2.start_frame();
                uint64_t frame_tick = playtick;
                while ((unsigned)(playtick-frame_tick) < ticks_per_buffer) {
                        if (playpos >= dumpsize) {
                                break;
                        }
                        unsigned char reg = dump[playpos].num;
                        unsigned char val = dump[playpos].val;
                        playtick = dump[playpos].time;
                        unsigned ts = (unsigned)(playtick - frame_tick);
                        if (ts_mode && (reg & 0x10)) {
                                chip2.select(reg & 0x0F);
                                chip2.write(ts, val);
                        } else {
                                chip.select(reg);
                                chip.write(ts, val);
                        }
                        if (++playpos >= dumpsize && loop_enabled) {
                                playpos = loop;
                                break;
                        }
                }
                chip.end_frame((unsigned)(playtick-frame_tick));
                if (ts_mode) chip2.end_frame((unsigned)(playtick-frame_tick));
        }
}
