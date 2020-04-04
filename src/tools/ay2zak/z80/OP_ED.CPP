/* ED opcodes */

#ifndef Z80_COMMON
Z80OPCODE ope_40(Z80 *cpu) { // in b,(c)
   cpu->t += 4;
   cpu->b = in(cpu->bc);
   cpu->f = log_f[cpu->b] | (cpu->f & CF);
}
Z80OPCODE ope_41(Z80 *cpu) { // out (c),b
   cpu->t += 4;
   out(cpu->bc, cpu->b);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_42(Z80 *cpu) { // sbc hl,bc
   unsigned char fl = NF;
   fl |= (((cpu->hl & 0x0FFF) - (cpu->bc & 0x0FFF) - (cpu->af & CF)) >> 8) & 0x10; /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF) - (cpu->bc & 0xFFFF) - (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   int ri = (int)(short)cpu->hl - (int)(short)cpu->bc - (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_43(Z80 *cpu) { // ld (nnnn),bc
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   wm(adr, cpu->c);
   wm(adr+1, cpu->b);
   cpu->t += 12;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_44(Z80 *cpu) { // neg
   cpu->f = sbcf[cpu->a];
   cpu->a = -cpu->a;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_45(Z80 *cpu) { // retn
   cpu->iff1 = cpu->iff2;
   cpu->pcl = rm(cpu->sp++);
   cpu->pch = rm(cpu->sp++);
   cpu->t += 6;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_46(Z80 *cpu) { // im 0
   cpu->im = 0;
}
Z80OPCODE ope_47(Z80 *cpu) { // ld i,a
   cpu->i = cpu->a;
   cpu->t++;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_48(Z80 *cpu) { // in c,(c)
   cpu->t += 4;
   cpu->c = in(cpu->bc);
   cpu->f = log_f[cpu->c] | (cpu->f & CF);
}
Z80OPCODE ope_49(Z80 *cpu) { // out (c),c
   cpu->t += 4;
   out(cpu->bc, cpu->c);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_4A(Z80 *cpu) { // adc hl,bc
   unsigned char fl = (((cpu->hl & 0x0FFF) + (cpu->bc & 0x0FFF) + (cpu->af & CF)) >> 8) & 0x10; /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF) + (cpu->bc & 0xFFFF) + (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   int ri = (int)(short)cpu->hl + (int)(short)cpu->bc + (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_4B(Z80 *cpu) { // ld bc,(nnnn)
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   cpu->c = rm(adr);
   cpu->b = rm(adr+1);
   cpu->t += 12;
}
#define ope_4C ope_44   // neg
Z80OPCODE ope_4D(Z80 *cpu) { // reti
   cpu->iff1 = cpu->iff2;
   cpu->pcl = rm(cpu->sp++);
   cpu->pch = rm(cpu->sp++);
   cpu->t += 6;
}
#endif

#define ope_4E ope_56  // im 0/1 -> im1

#ifdef Z80_COMMON
Z80OPCODE ope_4F(Z80 *cpu) { // ld r,a
   cpu->r_low = cpu->a;
   cpu->r_hi = cpu->a & 0x80;
   cpu->t++;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_50(Z80 *cpu) { // in d,(c)
   cpu->t += 4;
   cpu->d = in(cpu->bc);
   cpu->f = log_f[cpu->d] | (cpu->f & CF);
}
Z80OPCODE ope_51(Z80 *cpu) { // out (c),d
   cpu->t += 4;
   out(cpu->bc, cpu->d);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_52(Z80 *cpu) { // sbc hl,de
   unsigned char fl = NF;
   fl |= (((cpu->hl & 0x0FFF) - (cpu->de & 0x0FFF) - (cpu->af & CF)) >> 8) & 0x10; /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF) - (cpu->de & 0xFFFF) - (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   int ri = (int)(short)cpu->hl - (int)(short)cpu->de - (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_53(Z80 *cpu) { // ld (nnnn),de
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   wm(adr, cpu->e);
   wm(adr+1, cpu->d);
   cpu->t += 12;
}
#endif

#define ope_54 ope_44 // neg
#define ope_55 ope_45 // retn

#ifdef Z80_COMMON
Z80OPCODE ope_56(Z80 *cpu) { // im 1
   cpu->im = 1;
}
Z80OPCODE ope_57(Z80 *cpu) { // ld a,i
   cpu->a = cpu->i;
   cpu->f = (log_f[cpu->a] | (cpu->f & CF)) & ~PV;
   if (cpu->iff1) cpu->f |= PV;
   cpu->t++;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_58(Z80 *cpu) { // in e,(c)
   cpu->t += 4;
   cpu->e = in(cpu->bc);
   cpu->f = log_f[cpu->e] | (cpu->f & CF);
}
Z80OPCODE ope_59(Z80 *cpu) { // out (c),e
   cpu->t += 4;
   out(cpu->bc, cpu->e);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_5A(Z80 *cpu) { // adc hl,de
   unsigned char fl = (((cpu->hl & 0x0FFF) + (cpu->de & 0x0FFF) + (cpu->af & CF)) >> 8) & 0x10; /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF) + (cpu->de & 0xFFFF) + (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   int ri = (int)(short)cpu->hl + (int)(short)cpu->de + (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_5B(Z80 *cpu) { // ld de,(nnnn)
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   cpu->e = rm(adr);
   cpu->d = rm(adr+1);
   cpu->t += 12;
}
#endif

#define ope_5C ope_44   // neg
#define ope_5D ope_4D   // reti

#ifdef Z80_COMMON
Z80OPCODE ope_5E(Z80 *cpu) { // im 2
   cpu->im = 2;
}
Z80OPCODE ope_5F(Z80 *cpu) { // ld a,r
   cpu->a = (cpu->r_low & 0x7F) | cpu->r_hi;
   cpu->f = (log_f[cpu->a] | (cpu->f & CF)) & ~PV;
   if (cpu->iff2 && (cpu->eipos+8==cpu->t)) cpu->f |= PV;
   cpu->t++;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_60(Z80 *cpu) { // in h,(c)
   cpu->t += 4;
   cpu->h = in(cpu->bc);
   cpu->f = log_f[cpu->h] | (cpu->f & CF);
}
Z80OPCODE ope_61(Z80 *cpu) { // out (c),h
   cpu->t += 4;
   out(cpu->bc, cpu->h);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_62(Z80 *cpu) { // sbc hl,hl
   unsigned char fl = NF;
   fl |= (cpu->f & CF) << 4; /* HF - copy from CF */
   unsigned tmp = 0-(cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   // never set PV
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif

#define ope_63 op_22 // ld (nnnn),hl
#define ope_64 ope_44 // neg
#define ope_65 ope_45 // retn
#define ope_66 ope_46 // im 0

#ifndef Z80_COMMON
Z80OPCODE ope_67(Z80 *cpu) { // rrd
   unsigned char tmp = rm(cpu->hl);
   wm(cpu->hl, (cpu->a << 4) | (tmp >> 4));
   cpu->a = (cpu->a & 0xF0) | (tmp & 0x0F);
   cpu->f = log_f[cpu->a] | (cpu->f & CF);
   cpu->t += 10;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_68(Z80 *cpu) { // in l,(c)
   cpu->t += 4;
   cpu->l = in(cpu->bc);
   cpu->f = log_f[cpu->l] | (cpu->f & CF);
}
Z80OPCODE ope_69(Z80 *cpu) { // out (c),l
   cpu->t += 4;
   out(cpu->bc, cpu->l);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_6A(Z80 *cpu) { // adc hl,hl
   unsigned char fl = ((cpu->h << 1) & 0x10); /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF)*2 + (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   int ri = 2*(int)(short)cpu->hl + (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif

#define ope_6B op_2A // ld hl,(nnnn)
#define ope_6C ope_44   // neg
#define ope_6D ope_4D   // reti
#define ope_6E ope_56   // im 0/1 -> im 1

#ifndef Z80_COMMON
Z80OPCODE ope_6F(Z80 *cpu) { // rld
   unsigned char tmp = rm(cpu->hl);
   wm(cpu->hl, (cpu->a & 0x0F) | (tmp << 4));
   cpu->a = (cpu->a & 0xF0) | (tmp >> 4);
   cpu->f = log_f[cpu->a] | (cpu->f & CF);
   cpu->t += 10;
}
Z80OPCODE ope_70(Z80 *cpu) { // in (c)
   cpu->t += 4;
   cpu->f = log_f[in(cpu->bc)] | (cpu->f & CF);
}
Z80OPCODE ope_71(Z80 *cpu) { // out (c),0
   cpu->t += 4;
   out(cpu->bc, 0);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_72(Z80 *cpu) { // sbc hl,sp
   unsigned char fl = NF;
   fl |= (((cpu->hl & 0x0FFF) - (cpu->sp & 0x0FFF) - (cpu->af & CF)) >> 8) & 0x10; /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF) - (cpu->sp & 0xFFFF) - (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(tmp & 0xFFFF)) fl |= ZF;
   int ri = (int)(short)cpu->hl - (int)(short)cpu->sp - (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_73(Z80 *cpu) { // ld (nnnn),sp
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   wm(adr, cpu->spl);
   wm(adr+1, cpu->sph);
   cpu->t += 12;
}
#endif

#define ope_74 ope_44 // neg
#define ope_75 ope_45 // retn

#ifdef Z80_COMMON
Z80OPCODE ope_76(Z80 *cpu) { // im 1
   cpu->im = 1;
}
#endif

#define ope_77 op_00  // nop

#ifndef Z80_COMMON
Z80OPCODE ope_78(Z80 *cpu) { // in a,(c)
   cpu->t += 4;
   cpu->a = in(cpu->bc);
   cpu->f = log_f[cpu->a] | (cpu->f & CF);
}
Z80OPCODE ope_79(Z80 *cpu) { // out (c),a
   cpu->t += 4;
   out(cpu->bc, cpu->a);
}
#endif
#ifdef Z80_COMMON
Z80OPCODE ope_7A(Z80 *cpu) { // adc hl,sp
   unsigned char fl = (((cpu->hl & 0x0FFF) + (cpu->sp & 0x0FFF) + (cpu->af & CF)) >> 8) & 0x10; /* HF */
   unsigned tmp = (cpu->hl & 0xFFFF) + (cpu->sp & 0xFFFF) + (cpu->af & CF);
   if (tmp & 0x10000) fl |= CF;
   if (!(unsigned short)tmp) fl |= ZF;
   int ri = (int)(short)cpu->hl + (int)(short)cpu->sp + (int)(cpu->af & CF);
   if (ri < -0x8000 || ri >= 0x8000) fl |= PV;
   cpu->hl = tmp;
   cpu->f = fl | (cpu->h & (F3|F5|SF));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE ope_7B(Z80 *cpu) { // ld sp,(nnnn)
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   cpu->spl = rm(adr);
   cpu->sph = rm(adr+1);
   cpu->t += 12;
}
#endif

#define ope_7C ope_44   // neg
#define ope_7D ope_4D   // reti
#define ope_7E ope_5E   // im 2
#define ope_7F op_00    // nop

#ifndef Z80_COMMON
Z80OPCODE ope_A0(Z80 *cpu) { // ldi
   cpu->t += 8;
   wm(cpu->de++, rm(cpu->hl++));
   cpu->f &= ~(NF|HF|PV);
   if (--cpu->bc16) cpu->f |= PV;
}
Z80OPCODE ope_A1(Z80 *cpu) { // cpi
   cpu->t += 8;
   unsigned char cf = cpu->f & CF;
   cp8(cpu, rm(cpu->hl++));
   cpu->f = (cpu->f & ~(CF|PV)) | cf;
   if (--cpu->bc16) cpu->f |= PV;
}
Z80OPCODE ope_A2(Z80 *cpu) { // ini
   cpu->t += 8;
   wm(cpu->hl++, in(cpu->bc));
   dec8(cpu, cpu->b);
}
Z80OPCODE ope_A3(Z80 *cpu) { // outi
   cpu->t += 8;
   dec8(cpu, cpu->b);
   out(cpu->bc, rm(cpu->hl++));
   cpu->f &= ~CF; if (!cpu->l) cpu->f |= CF;
}
Z80OPCODE ope_A8(Z80 *cpu) { // ldd
   cpu->t += 8;
   wm(cpu->de--, rm(cpu->hl--));
   cpu->f &= ~(NF|HF|PV);
   if (--cpu->bc16) cpu->f |= PV;
}
Z80OPCODE ope_A9(Z80 *cpu) { // cpd
   cpu->t += 8;
   unsigned char cf = cpu->f & CF;
   cp8(cpu, rm(cpu->hl--));
   cpu->f = (cpu->f & ~(CF|PV)) | cf;
   if (--cpu->bc16) cpu->f |= PV;
}
Z80OPCODE ope_AA(Z80 *cpu) { // ind
   cpu->t += 8;
   wm(cpu->hl--, in(cpu->bc));
   dec8(cpu, cpu->b);
}
Z80OPCODE ope_AB(Z80 *cpu) { // outd
   cpu->t += 8;
   dec8(cpu, cpu->b);
   out(cpu->bc, rm(cpu->hl--));
   cpu->f &= ~CF; if (cpu->l == 0xFF) cpu->f |= CF;
}
Z80OPCODE ope_B0(Z80 *cpu) { // ldir
   cpu->t += 8;
   wm(cpu->de++, rm(cpu->hl++));
   cpu->f &= ~(NF|HF|PV);
   if (--cpu->bc16) cpu->f |= PV, cpu->pc -= 2, cpu->t += 5;
}
Z80OPCODE ope_B1(Z80 *cpu) { // cpir
   cpu->t += 8;
   unsigned char cf = cpu->f & CF;
   cp8(cpu, rm(cpu->hl++));
   cpu->f = (cpu->f & ~(CF|PV)) | cf;
   if (--cpu->bc16) {
      cpu->f |= PV;
      if (!(cpu->f & ZF)) cpu->pc -= 2, cpu->t += 5;
   }
}
Z80OPCODE ope_B2(Z80 *cpu) { // inir
   cpu->t += 8;
   wm(cpu->hl++, in(cpu->bc));
   dec8(cpu, cpu->b);
   if (cpu->b) cpu->f |= PV, cpu->pc -= 2, cpu->t += 5;
   else cpu->f &= ~PV;
}
Z80OPCODE ope_B3(Z80 *cpu) { // otir
   cpu->t += 8;
   dec8(cpu, cpu->b);
   out(cpu->bc, rm(cpu->hl++));
   if (cpu->b) cpu->f |= PV, cpu->pc -= 2, cpu->t += 5;
   else cpu->f &= ~PV;
   cpu->f &= ~CF; if (!cpu->l) cpu->f |= CF;
}
Z80OPCODE ope_B8(Z80 *cpu) { // lddr
   cpu->t += 8;
   wm(cpu->de--, rm(cpu->hl--));
   cpu->f &= ~(NF|HF|PV);
   if (--cpu->bc16) cpu->f |= PV, cpu->pc -= 2, cpu->t += 5;
   else cpu->f &= ~PV;
}
Z80OPCODE ope_B9(Z80 *cpu) { // cpdr
   cpu->t += 8;
   unsigned char cf = cpu->f & CF;
   cp8(cpu, rm(cpu->hl--));
   cpu->f = (cpu->f & ~(CF|PV)) | cf;
   if (--cpu->bc16) {
      cpu->f |= PV;
      if (!(cpu->f & ZF)) cpu->pc -= 2, cpu->t += 5;
   }
}
Z80OPCODE ope_BA(Z80 *cpu) { // indr
   cpu->t += 8;
   wm(cpu->hl--, in(cpu->bc));
   dec8(cpu, cpu->b);
   if (cpu->b) cpu->f |= PV, cpu->pc -= 2, cpu->t += 5;
   else cpu->f &= ~PV;
}
Z80OPCODE ope_BB(Z80 *cpu) { // otdr
   cpu->t += 8;
   dec8(cpu, cpu->b);
   out(cpu->bc, rm(cpu->hl--));
   if (cpu->b) cpu->f |= PV, cpu->pc -= 2, cpu->t += 5;
   else cpu->f &= ~PV;
   cpu->f &= ~CF; if (cpu->l == 0xFF) cpu->f |= CF;
}


STEPFUNC const ext_opcode[0x100] = {

   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,

   ope_40, ope_41, ope_42, ope_43, ope_44, ope_45, ope_46, ope_47,
   ope_48, ope_49, ope_4A, ope_4B, ope_4C, ope_4D, ope_4E, ope_4F,
   ope_50, ope_51, ope_52, ope_53, ope_54, ope_55, ope_56, ope_57,
   ope_58, ope_59, ope_5A, ope_5B, ope_5C, ope_5D, ope_5E, ope_5F,
   ope_60, ope_61, ope_62, ope_63, ope_64, ope_65, ope_66, ope_67,
   ope_68, ope_69, ope_6A, ope_6B, ope_6C, ope_6D, ope_6E, ope_6F,
   ope_70, ope_71, ope_72, ope_73, ope_74, ope_75, ope_76, ope_77,
   ope_78, ope_79, ope_7A, ope_7B, ope_7C, ope_7D, ope_7E, ope_7F,

   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   ope_A0, ope_A1, ope_A2, ope_A3, op_00, op_00, op_00, op_00,
   ope_A8, ope_A9, ope_AA, ope_AB, op_00, op_00, op_00, op_00,
   ope_B0, ope_B1, ope_B2, ope_B3, op_00, op_00, op_00, op_00,
   ope_B8, ope_B9, ope_BA, ope_BB, op_00, op_00, op_00, op_00,

   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,
   op_00, op_00, op_00, op_00, op_00, op_00, op_00, op_00,

};

Z80OPCODE op_ED(Z80 *cpu)
{
   unsigned char opcode = m1_cycle(cpu);
   (ext_opcode[opcode])(cpu);
}
#endif
