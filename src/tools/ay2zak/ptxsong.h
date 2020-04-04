#ifndef _PTXSONG_H_INCLUDED
#define _PTXSONG_H_INCLUDED

#include "z80esong.h"

class PtXSong : public Z80EmulatedSong
{
 public:

   PtXSong(FILE *file);

 protected:

   virtual void load_memory();
   virtual void z80_step_hook();
};


#endif // _PTXSONG_H_INCLUDED