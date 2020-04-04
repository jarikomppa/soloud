
#include "z80emul.h"

#include "z80/init.cpp"

#define Z80_COMMON
#include "z80/cmd.cpp"
#undef Z80_COMMON

unsigned char memory[128 * 1024];
unsigned char * const RAM_BASE_M = memory;
unsigned char *bankr[4], *bankw[4];
const unsigned PAGE = 0x4000;

Z80INLINE unsigned char *am_r(unsigned addr)
{
	return bankr[(addr >> 14) & 3] + (addr & (PAGE - 1));
}

Z80INLINE unsigned char rm(unsigned addr)
{
	return *am_r(addr);
}

Z80INLINE void wm(unsigned addr, unsigned char val)
{
	unsigned char *a = bankw[(addr >> 14) & 3];
	a[addr & (PAGE - 1)] = val;
}

Z80INLINE unsigned char m1_cycle(Z80 *cpu)
{
	cpu->r_low++;// = (cpu->r & 0x80) + ((cpu->r+1) & 0x7F);
	cpu->t += 4;
	return rm(cpu->pc++);
}

Z80 cpu;
unsigned char p7FFD;
unsigned char activereg;
unsigned char regs[0x10];

void set_banks()
{
	bankw[0] = bankr[0] = RAM_BASE_M + 0 * PAGE;
	bankw[1] = bankr[1] = RAM_BASE_M + 5 * PAGE;
	bankw[2] = bankr[2] = RAM_BASE_M + 2 * PAGE;
	bankw[3] = bankr[3] = RAM_BASE_M + (p7FFD & 7)*PAGE;
}

void out(unsigned port, unsigned char val)
{
	if (!(port & 2)) { // #FD-ports
		if (!(port & 0x8000)) { p7FFD = val; set_banks(); return; }
		if (port & 0x4000) { // FFFD - select AY reg
			activereg = val & 0x0F;
		}
		else { // BFFD - write AY reg
			Z80EmulatedSong *song = (Z80EmulatedSong*)cpu.context;
			song->reg_out(activereg, val);
			if (activereg < 0x10) regs[activereg] = val;
		}
		return;
	}
}

unsigned char in(unsigned port)
{
	if ((port & 0xC002) == (0xFFFD & 0xC002)) {
		// AY FFFD port - read reg
		if (activereg >= 0x10) return 0xFF;
		return regs[activereg];
	}

	return 0xFF;
}

#include "z80/cmd.cpp"

void sys_reset()
{
	cpu.reset();
	p7FFD = 0;
	set_banks();
}

Z80INLINE void step()
{
	Z80EmulatedSong *song = (Z80EmulatedSong*)cpu.context;
	song->z80_step_hook();

	unsigned char opcode = m1_cycle(&cpu);
	(normal_opcode[opcode])(&cpu);
}


void z80_emulate_frame()
{
	cpu.haltpos = 0;

	// INT check separated from main Z80 loop to improve emulation speed
	while (cpu.t < INTLEN) {
		if (cpu.iff1 && cpu.t != cpu.eipos) // int enabled in CPU not issued after EI
			handle_int(&cpu, 0xFF);
		step();
		if (cpu.halted) break;
	}

	cpu.eipos = -1;

	while (cpu.t < FRAME) {
		if (cpu.halted) {
			//cpu.t += 4, cpu.r = (cpu.r & 0x80) + ((cpu.r+1) & 0x7F); continue;
			unsigned st = (FRAME - cpu.t - 1) / 4 + 1;
			cpu.t += 4 * st;
			cpu.r_low += st;
			break;
		}
		step();
	}
}

unsigned char sys_rm(unsigned addr) { return rm(addr); }
void sys_wm(unsigned addr, unsigned char val) { wm(addr, val); }

