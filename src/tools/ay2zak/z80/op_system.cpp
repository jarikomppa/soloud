
#ifndef Z80_COMMON
Z80INLINE void handle_int(Z80 *cpu, unsigned char vector)
{
   unsigned intad;
   if (cpu->im < 2) {
      intad = 0x38;
   } else { // im2
      unsigned vec = vector + cpu->i*0x100;
      intad = rm(vec) + 0x100*rm(vec+1);
   }

   cpu->t += (cpu->im < 2) ? 13 : 19;
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = intad;
   cpu->halted = 0;
   cpu->iff1 = cpu->iff2 = 0;
}
#endif
