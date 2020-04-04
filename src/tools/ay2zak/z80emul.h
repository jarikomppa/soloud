#ifndef _Z80EMUL_H_INCLUDED
#define _Z80EMUL_H_INCLUDED

#include "defs.h"
#include "z80esong.h"
#include "chipplayer.h"

const unsigned INTLEN = 32;
const unsigned FRAME = SNDR_DEFAULT_SYSTICK_RATE / 50;

void init_z80tables();
extern unsigned char memory[];
extern Z80 cpu;

void sys_reset();
unsigned char sys_rm(unsigned addr);
void sys_wm(unsigned addr, unsigned char val);

void z80_emulate_frame();


#endif // _Z80EMUL_H_INCLUDED
