#ifndef _CHIPPLAYER_H_INCLUDED
#define _CHIPPLAYER_H_INCLUDED

#include "sndchip.h"

struct REG_OUT;

class ChipPlayer
{
 public:

   // play next song chunk to buffer
   void play(SNDSAMPLE *dst, unsigned need_samples);
   void set_volume(unsigned global_vol) {
      chip.set_volume(global_vol);
      chip2.set_volume(global_vol);
   }

   void seek(unsigned time_ms);
   unsigned get_pos(); // in ms
   unsigned get_len(); // in ms
   void set_loop_enabled(bool loop_enabled) { ChipPlayer::loop_enabled = loop_enabled; }


   // for internal use by ChipSong class
   void set_timings(unsigned system_clock_rate, unsigned chip_clock_rate, unsigned sample_rate);
   void set_volumes(SNDCHIP::CHIP_TYPE t, unsigned global_vol, const SNDCHIP_VOLTAB *vt, const SNDCHIP_PANTAB *pt);

   ChipPlayer();
   ~ChipPlayer();

   void reg_out(uint64_t tick, unsigned char reg, unsigned char val);
   void set_loop() { loop = dumpsize; }

   bool ts_mode;

 //private:
   SNDBUFFER buffer;
   SNDCHIP chip;
   SNDCHIP chip2;

   REG_OUT *dump;
   unsigned dumpsize, dump_alloc;
   unsigned locate(uint64_t tick);
   uint64_t playtick;
   unsigned playpos, loop;
   unsigned system_clock_rate;
   unsigned ticks_per_buffer;
   bool loop_enabled;

   unsigned get_time(uint64_t tick);
};

struct REG_OUT
{
   unsigned char num, val;
   unsigned int time;
};

#endif // _CHIPPLAYER_H_INCLUDED
