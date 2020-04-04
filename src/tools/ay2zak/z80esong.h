
// base class for any song, played with Z80 emulator

#ifndef _Z80ESONG_H_INCLUDED
#define _Z80ESONG_H_INCLUDED

#include "chipsong.h"
#include "z80emul.h"

class Z80EmulatedSong : public ChipSong
{
 public:

   virtual ChipPlayer *CreatePlayer(unsigned sample_fq);
   Z80EmulatedSong(FILE *file) : ChipSong(file) {}

   void reg_out(unsigned char activereg, unsigned char val);
   virtual void z80_int_hook() {}
   virtual void z80_step_hook() {}

 protected:

   ChipPlayer *pl;
   unsigned frame;
   uint64_t frame_start;
   int done;

   virtual void load_memory() = 0;
   void make_player(unsigned init, unsigned play);
};


#endif // _Z80ESONG_H_INCLUDED
