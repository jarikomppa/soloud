/* DDCB/FDCB opcodes */
/* note: cpu.t and destination updated in step(), here process 'byte' */

#ifdef Z80_COMMON
Z80LOGIC oplx_00(Z80 *cpu, unsigned char byte) { // rlc (ix+nn)
   cpu->f = rlcf[byte]; return rol[byte];
}
Z80LOGIC oplx_08(Z80 *cpu, unsigned char byte) { // rrc (ix+nn)
   cpu->f = rrcf[byte]; return ror[byte];
}
Z80LOGIC oplx_10(Z80 *cpu, unsigned char byte) { // rl (ix+nn)
   if (cpu->f & CF) {
      cpu->f = rl1[byte]; return (byte << 1) + 1;
   } else {
      cpu->f = rl0[byte]; return (byte << 1);
   }
}
Z80LOGIC oplx_18(Z80 *cpu, unsigned char byte) { // rr (ix+nn)
   if (cpu->f & CF) {
      cpu->f = rr1[byte]; return (byte >> 1) + 0x80;
   } else {
      cpu->f = rr0[byte]; return (byte >> 1);
   }
}
Z80LOGIC oplx_20(Z80 *cpu, unsigned char byte) { // sla (ix+nn)
   cpu->f = rl0[byte]; return (byte << 1);
}
Z80LOGIC oplx_28(Z80 *cpu, unsigned char byte) { // sra (ix+nn)
   cpu->f = sraf[byte]; return (byte >> 1) + (byte & 0x80);
}
Z80LOGIC oplx_30(Z80 *cpu, unsigned char byte) { // sli (ix+nn)
   cpu->f = rl1[byte]; return (byte << 1) + 1;
}
Z80LOGIC oplx_38(Z80 *cpu, unsigned char byte) { // srl (ix+nn)
   cpu->f = rr0[byte]; return (byte >> 1);
}
Z80LOGIC oplx_40(Z80 *cpu, unsigned char byte) { // bit 0,(ix+nn)
   bit(cpu, byte, 0); return byte;
}
Z80LOGIC oplx_48(Z80 *cpu, unsigned char byte) { // bit 1,(ix+nn)
   bit(cpu, byte, 1); return byte;
}
Z80LOGIC oplx_50(Z80 *cpu, unsigned char byte) { // bit 2,(ix+nn)
   bit(cpu, byte, 2); return byte;
}
Z80LOGIC oplx_58(Z80 *cpu, unsigned char byte) { // bit 3,(ix+nn)
   bit(cpu, byte, 3); return byte;
}
Z80LOGIC oplx_60(Z80 *cpu, unsigned char byte) { // bit 4,(ix+nn)
   bit(cpu, byte, 4); return byte;
}
Z80LOGIC oplx_68(Z80 *cpu, unsigned char byte) { // bit 5,(ix+nn)
   bit(cpu, byte, 5); return byte;
}
Z80LOGIC oplx_70(Z80 *cpu, unsigned char byte) { // bit 6,(ix+nn)
   bit(cpu, byte, 6); return byte;
}
Z80LOGIC oplx_78(Z80 *cpu, unsigned char byte) { // bit 7,(ix+nn)
   bit(cpu, byte, 7); return byte;
}
Z80LOGIC oplx_80(Z80 *cpu, unsigned char byte) { // res 0,(ix+nn)
   return resbyte(byte, 0);
}
Z80LOGIC oplx_88(Z80 *cpu, unsigned char byte) { // res 1,(ix+nn)
   return resbyte(byte, 1);
}
Z80LOGIC oplx_90(Z80 *cpu, unsigned char byte) { // res 2,(ix+nn)
   return resbyte(byte, 2);
}
Z80LOGIC oplx_98(Z80 *cpu, unsigned char byte) { // res 3,(ix+nn)
   return resbyte(byte, 3);
}
Z80LOGIC oplx_A0(Z80 *cpu, unsigned char byte) { // res 4,(ix+nn)
   return resbyte(byte, 4);
}
Z80LOGIC oplx_A8(Z80 *cpu, unsigned char byte) { // res 5,(ix+nn)
   return resbyte(byte, 5);
}
Z80LOGIC oplx_B0(Z80 *cpu, unsigned char byte) { // res 6,(ix+nn)
   return resbyte(byte, 6);
}
Z80LOGIC oplx_B8(Z80 *cpu, unsigned char byte) { // res 7,(ix+nn)
   return resbyte(byte, 7);
}
Z80LOGIC oplx_C0(Z80 *cpu, unsigned char byte) { // set 0,(ix+nn)
   return setbyte(byte, 0);
}
Z80LOGIC oplx_C8(Z80 *cpu, unsigned char byte) { // set 1,(ix+nn)
   return setbyte(byte, 1);
}
Z80LOGIC oplx_D0(Z80 *cpu, unsigned char byte) { // set 2,(ix+nn)
   return setbyte(byte, 2);
}
Z80LOGIC oplx_D8(Z80 *cpu, unsigned char byte) { // set 3,(ix+nn)
   return setbyte(byte, 3);
}
Z80LOGIC oplx_E0(Z80 *cpu, unsigned char byte) { // set 4,(ix+nn)
   return setbyte(byte, 4);
}
Z80LOGIC oplx_E8(Z80 *cpu, unsigned char byte) { // set 5,(ix+nn)
   return setbyte(byte, 5);
}
Z80LOGIC oplx_F0(Z80 *cpu, unsigned char byte) { // set 6,(ix+nn)
   return setbyte(byte, 6);
}
Z80LOGIC oplx_F8(Z80 *cpu, unsigned char byte) { // set 7,(ix+nn)
   return setbyte(byte, 7);
}

LOGICFUNC const logic_ix_opcode[0x100] = {

   oplx_00, oplx_00, oplx_00, oplx_00, oplx_00, oplx_00, oplx_00, oplx_00,
   oplx_08, oplx_08, oplx_08, oplx_08, oplx_08, oplx_08, oplx_08, oplx_08,
   oplx_10, oplx_10, oplx_10, oplx_10, oplx_10, oplx_10, oplx_10, oplx_10,
   oplx_18, oplx_18, oplx_18, oplx_18, oplx_18, oplx_18, oplx_18, oplx_18,
   oplx_20, oplx_20, oplx_20, oplx_20, oplx_20, oplx_20, oplx_20, oplx_20,
   oplx_28, oplx_28, oplx_28, oplx_28, oplx_28, oplx_28, oplx_28, oplx_28,
   oplx_30, oplx_30, oplx_30, oplx_30, oplx_30, oplx_30, oplx_30, oplx_30,
   oplx_38, oplx_38, oplx_38, oplx_38, oplx_38, oplx_38, oplx_38, oplx_38,

   oplx_40, oplx_40, oplx_40, oplx_40, oplx_40, oplx_40, oplx_40, oplx_40,
   oplx_48, oplx_48, oplx_48, oplx_48, oplx_48, oplx_48, oplx_48, oplx_48,
   oplx_50, oplx_50, oplx_50, oplx_50, oplx_50, oplx_50, oplx_50, oplx_50,
   oplx_58, oplx_58, oplx_58, oplx_58, oplx_58, oplx_58, oplx_58, oplx_58,
   oplx_60, oplx_60, oplx_60, oplx_60, oplx_60, oplx_60, oplx_60, oplx_60,
   oplx_68, oplx_68, oplx_68, oplx_68, oplx_68, oplx_68, oplx_68, oplx_68,
   oplx_70, oplx_70, oplx_70, oplx_70, oplx_70, oplx_70, oplx_70, oplx_70,
   oplx_78, oplx_78, oplx_78, oplx_78, oplx_78, oplx_78, oplx_78, oplx_78,

   oplx_80, oplx_80, oplx_80, oplx_80, oplx_80, oplx_80, oplx_80, oplx_80,
   oplx_88, oplx_88, oplx_88, oplx_88, oplx_88, oplx_88, oplx_88, oplx_88,
   oplx_90, oplx_90, oplx_90, oplx_90, oplx_90, oplx_90, oplx_90, oplx_90,
   oplx_98, oplx_98, oplx_98, oplx_98, oplx_98, oplx_98, oplx_98, oplx_98,
   oplx_A0, oplx_A0, oplx_A0, oplx_A0, oplx_A0, oplx_A0, oplx_A0, oplx_A0,
   oplx_A8, oplx_A8, oplx_A8, oplx_A8, oplx_A8, oplx_A8, oplx_A8, oplx_A8,
   oplx_B0, oplx_B0, oplx_B0, oplx_B0, oplx_B0, oplx_B0, oplx_B0, oplx_B0,
   oplx_B8, oplx_B8, oplx_B8, oplx_B8, oplx_B8, oplx_B8, oplx_B8, oplx_B8,

   oplx_C0, oplx_C0, oplx_C0, oplx_C0, oplx_C0, oplx_C0, oplx_C0, oplx_C0,
   oplx_C8, oplx_C8, oplx_C8, oplx_C8, oplx_C8, oplx_C8, oplx_C8, oplx_C8,
   oplx_D0, oplx_D0, oplx_D0, oplx_D0, oplx_D0, oplx_D0, oplx_D0, oplx_D0,
   oplx_D8, oplx_D8, oplx_D8, oplx_D8, oplx_D8, oplx_D8, oplx_D8, oplx_D8,
   oplx_E0, oplx_E0, oplx_E0, oplx_E0, oplx_E0, oplx_E0, oplx_E0, oplx_E0,
   oplx_E8, oplx_E8, oplx_E8, oplx_E8, oplx_E8, oplx_E8, oplx_E8, oplx_E8,
   oplx_F0, oplx_F0, oplx_F0, oplx_F0, oplx_F0, oplx_F0, oplx_F0, oplx_F0,
   oplx_F8, oplx_F8, oplx_F8, oplx_F8, oplx_F8, oplx_F8, oplx_F8, oplx_F8,

};

// offsets to b,c,d,e,h,l,<unused>,a  from cpu.c
unsigned reg_offset[] = { 1,0, 5,4, 9,8, 2,13 };
#endif

#ifndef Z80_COMMON
Z80INLINE void Z80FAST ddfd(Z80 *cpu, unsigned char opcode)
{
   unsigned char op1; // last DD/FD prefix
   do {
      op1 = opcode;
      opcode = m1_cycle(cpu);
   } while ((opcode | 0x20) == 0xFD); // opcode == DD/FD

   if (opcode == 0xCB) {

      unsigned ptr; // pointer to DDCB operand
      ptr = ((op1 == 0xDD) ? cpu->ix:cpu->iy) + (signed char)rm(cpu->pc++);
      // DDCBnnXX,FDCBnnXX increment R by 2, not 3!
      opcode = m1_cycle(cpu); cpu->r_low--;
      unsigned char byte = (logic_ix_opcode[opcode])(cpu, rm(ptr));
      if ((opcode & 0xC0) == 0x40) { cpu->t += 8; return; } // bit n,rm

      // select destination register for shift/res/set
      *(&cpu->c + reg_offset[opcode & 7]) = byte;
      wm(ptr, byte);
      cpu->t += 11;
      return;
   }

   if (opcode == 0xED) {
      opcode = m1_cycle(cpu);
      (ext_opcode[opcode])(cpu);
      return;
   }

   // one prefix: DD/FD
   ((op1 == 0xDD) ? ix_opcode[opcode] : iy_opcode[opcode])(cpu);
}

Z80OPCODE op_DD(Z80 *cpu)
{
   ddfd(cpu, 0xDD);
}

Z80OPCODE op_FD(Z80 *cpu)
{
   ddfd(cpu, 0xFD);
}
#endif
