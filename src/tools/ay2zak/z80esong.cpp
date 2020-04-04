#include "z80esong.h"
#include <memory.h>

ChipPlayer *Z80EmulatedSong::CreatePlayer(unsigned sample_fq)
{
   init_z80tables();
   cpu.context = this;
   memset(memory, 0, 0x10000);
   sys_reset();

   load_memory();

   pl = new ChipPlayer();
   pl->set_timings(SNDR_DEFAULT_SYSTICK_RATE, SNDR_DEFAULT_AY_RATE, sample_fq);

   frame = 0; frame_start = 0; done = 0;

   while (!done) {
      z80_int_hook();
      z80_emulate_frame();
      frame_start += FRAME;
      cpu.t -= FRAME;
      frame++;
   }

   return pl;
}

void Z80EmulatedSong::reg_out(unsigned char activereg, unsigned char val)
{
   pl->reg_out(cpu.t + frame_start, activereg, val);
}

void Z80EmulatedSong::make_player(unsigned init, unsigned play)
{
   sys_wm(0x0038, 0xC9);
   const static unsigned char player[] =
   {
      0xF3,                // di
      0xCD, 0x00, 0x00,    // call init
      0xFB,                // ei
      0x76,                // halt
      0xCD, 0x00, 0x00,    // call play
      0x18, 0xF9           // jr $-5
   };
   memcpy(memory, player, sizeof(player));
   sys_wm(2, init & 0xFF);
   sys_wm(3, init >> 8);
   sys_wm(7, play & 0xFF);
   sys_wm(8, play >> 8);
   // cpu.im = 1, cpu.pc = 0; done in CreatePlayer() -> sys_reset()
}
