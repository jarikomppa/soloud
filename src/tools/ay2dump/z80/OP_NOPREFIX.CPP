/*  not prefixed opcodes */

#ifdef Z80_COMMON
Z80OPCODE op_00(Z80 *cpu) { // nop
   /* note: don't inc t: 4 cycles already wasted in m1_cycle() */
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_01(Z80 *cpu) { // ld bc,nnnn
   cpu->c = rm(cpu->pc++);
   cpu->b = rm(cpu->pc++);
   cpu->t += 6;
}
Z80OPCODE op_02(Z80 *cpu) { // ld (bc),a
   wm(cpu->bc, cpu->a);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_03(Z80 *cpu) { // inc bc
   cpu->bc++;
   cpu->t += 2;
}
Z80OPCODE op_04(Z80 *cpu) { // inc b
   inc8(cpu, cpu->b);
}
Z80OPCODE op_05(Z80 *cpu) { // dec b
   dec8(cpu, cpu->b);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_06(Z80 *cpu) { // ld b,nn
   cpu->b = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_07(Z80 *cpu) { // rlca
   cpu->f = rlcaf[cpu->a] | (cpu->f & (SF | ZF | PV));
   cpu->a = rol[cpu->a];
}
Z80OPCODE op_08(Z80 *cpu) { // ex af,af'
   unsigned tmp = cpu->af;
   cpu->af = cpu->alt.af;
   cpu->alt.af = tmp;
}
Z80OPCODE op_09(Z80 *cpu) { // add hl,bc
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= (((cpu->hl & 0x0FFF) + (cpu->bc & 0x0FFF)) >> 8) & 0x10; /* HF */
   cpu->hl = (cpu->hl & 0xFFFF) + (cpu->bc & 0xFFFF);
   if (cpu->hl & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->h & (F5 | F3));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_0A(Z80 *cpu) { // ld a,(bc)
   cpu->a = rm(cpu->bc);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_0B(Z80 *cpu) { // dec bc
   cpu->bc--;
   cpu->t += 2;
}
Z80OPCODE op_0C(Z80 *cpu) { // inc c
   inc8(cpu, cpu->c);
}
Z80OPCODE op_0D(Z80 *cpu) { // dec c
   dec8(cpu, cpu->c);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_0E(Z80 *cpu) { // ld c,nn
   cpu->c = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_0F(Z80 *cpu) { // rrca
   cpu->f = rrcaf[cpu->a] | (cpu->f & (SF | ZF | PV));
   cpu->a = ror[cpu->a];
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_10(Z80 *cpu) { // djnz rr
   if (--cpu->b) {
      signed char offs = (char)rm(cpu->pc);
      cpu->pc += offs+1, cpu->t += 9;
   } else cpu->pc++, cpu->t += 4;
}
Z80OPCODE op_11(Z80 *cpu) { // ld de,nnnn
   cpu->e = rm(cpu->pc++);
   cpu->d = rm(cpu->pc++);
   cpu->t += 6;
}
Z80OPCODE op_12(Z80 *cpu) { // ld (de),a
   wm(cpu->de, cpu->a);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_13(Z80 *cpu) { // inc de
   cpu->de++;
   cpu->t += 2;
}
Z80OPCODE op_14(Z80 *cpu) { // inc d
   inc8(cpu, cpu->d);
}
Z80OPCODE op_15(Z80 *cpu) { // dec d
   dec8(cpu, cpu->d);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_16(Z80 *cpu) { // ld d,nn
   cpu->d = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_17(Z80 *cpu) { // rla
   unsigned char new_a = (cpu->a << 1) + (cpu->f & 1);
   cpu->f = rlcaf[cpu->a] | (cpu->f & (SF | ZF | PV)); // use same table with rlca
   cpu->a = new_a;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_18(Z80 *cpu) { // jr rr
   signed char offs = (char)rm(cpu->pc);
   cpu->pc += offs+1;
   cpu->t += 8;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_19(Z80 *cpu) { // add hl,de
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= (((cpu->hl & 0x0FFF) + (cpu->de & 0x0FFF)) >> 8) & 0x10; /* HF */
   cpu->hl = (cpu->hl & 0xFFFF) + (cpu->de & 0xFFFF);
   if (cpu->hl & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->h & (F5 | F3));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_1A(Z80 *cpu) { // ld a,(de)
   cpu->a = rm(cpu->de);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_1B(Z80 *cpu) { // dec de
   cpu->de--;
   cpu->t += 2;
}
Z80OPCODE op_1C(Z80 *cpu) { // inc e
   inc8(cpu, cpu->e);
}
Z80OPCODE op_1D(Z80 *cpu) { // dec e
   dec8(cpu, cpu->e);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_1E(Z80 *cpu) { // ld e,nn
   cpu->e = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_1F(Z80 *cpu) { // rra
   unsigned char new_a = (cpu->a >> 1) + (cpu->f << 7);
   cpu->f = rrcaf[cpu->a] | (cpu->f & (SF | ZF | PV)); // use same table with rrca
   cpu->a = new_a;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_20(Z80 *cpu) { // jr nz, rr
   if (!(cpu->f & ZF)) {
      signed char offs = (char)rm(cpu->pc);
      cpu->pc += offs+1, cpu->t += 8;
   } else cpu->pc++, cpu->t += 3;
}
Z80OPCODE op_21(Z80 *cpu) { // ld hl,nnnn
   cpu->l = rm(cpu->pc++);
   cpu->h = rm(cpu->pc++);
   cpu->t += 6;
}
Z80OPCODE op_22(Z80 *cpu) { // ld (nnnn),hl
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   wm(adr, cpu->l);
   wm(adr+1, cpu->h);
   cpu->t += 12;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_23(Z80 *cpu) { // inc hl
   cpu->hl++;
   cpu->t += 2;
}
Z80OPCODE op_24(Z80 *cpu) { // inc h
   inc8(cpu, cpu->h);
}
Z80OPCODE op_25(Z80 *cpu) { // dec h
   dec8(cpu, cpu->h);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_26(Z80 *cpu) { // ld h,nn
   cpu->h = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_27(Z80 *cpu) { // daa
   cpu->af = *(unsigned short*)
      (daatab+(cpu->a+0x100*((cpu->f & 3) + ((cpu->f >> 2) & 4)))*2);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_28(Z80 *cpu) { // jr z,rr
   if ((cpu->f & ZF)) {
      signed char offs = (char)rm(cpu->pc);
      cpu->pc += offs+1, cpu->t += 8;
   } else cpu->pc++, cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_29(Z80 *cpu) { // add hl,hl
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= ((cpu->h << 1) & 0x10); /* HF */
   cpu->f |= ((cpu->hl >> 7) & 0x10); /* HF */
   cpu->hl = (cpu->hl & 0xFFFF)*2;
   if (cpu->hl & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->h & (F5 | F3));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_2A(Z80 *cpu) { // ld hl,(nnnn)
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   cpu->l = rm(adr);
   cpu->h = rm(adr+1);
   cpu->t += 12;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_2B(Z80 *cpu) { // dec hl
   cpu->hl--;
   cpu->t += 2;
}
Z80OPCODE op_2C(Z80 *cpu) { // inc l
   inc8(cpu, cpu->l);
}
Z80OPCODE op_2D(Z80 *cpu) { // dec l
   dec8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_2E(Z80 *cpu) { // ld l,nn
   cpu->l = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_2F(Z80 *cpu) { // cpl
   cpu->a = cpu->a ^ 0xFF;
   cpu->f = (cpu->f & ~(F3 | F5)) | NF | HF | (cpu->a & (F3 | F5));
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_30(Z80 *cpu) { // jr nc, rr
   if (!(cpu->f & CF)) {
      signed char offs = (char)rm(cpu->pc);
      cpu->pc += offs+1, cpu->t += 8;
   } else cpu->pc++, cpu->t += 3;
}
Z80OPCODE op_31(Z80 *cpu) { // ld sp,nnnn
   cpu->spl = rm(cpu->pc++);
   cpu->sph = rm(cpu->pc++);
   cpu->t += 6;
}
Z80OPCODE op_32(Z80 *cpu) { // ld (nnnn),a
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   wm(adr, cpu->a);
   cpu->t += 9;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_33(Z80 *cpu) { // inc sp
   cpu->sp++;
   cpu->t += 2;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_34(Z80 *cpu) { // inc (hl)
   unsigned char hl = rm(cpu->hl);
   inc8(cpu, hl);
   wm(cpu->hl, hl);
   cpu->t += 7;
}
Z80OPCODE op_35(Z80 *cpu) { // dec (hl)
   unsigned char hl = rm(cpu->hl);
   dec8(cpu, hl);
   wm(cpu->hl, hl);
   cpu->t += 7;
}
Z80OPCODE op_36(Z80 *cpu) { // ld (hl),nn
   wm(cpu->hl, rm(cpu->pc++));
   cpu->t += 6;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_37(Z80 *cpu) { // scf
   cpu->f = (cpu->f & ~(F3|F5|HF|NF)) | (cpu->a & (F3|F5)) | CF;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_38(Z80 *cpu) { // jr c,rr
   if ((cpu->f & CF)) {
      signed char offs = (char)rm(cpu->pc);
      cpu->pc += offs+1, cpu->t += 8;
   } else cpu->pc++, cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_39(Z80 *cpu) { // add hl,sp
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= (((cpu->hl & 0x0FFF) + (cpu->sp & 0x0FFF)) >> 8) & 0x10; /* HF */
   cpu->hl = (cpu->hl & 0xFFFF) + (cpu->sp & 0xFFFF);
   if (cpu->hl & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->h & (F5 | F3));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_3A(Z80 *cpu) { // ld a,(nnnn)
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   cpu->a = rm(adr);
   cpu->t += 9;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_3B(Z80 *cpu) { // dec sp
   cpu->sp--;
   cpu->t += 2;
}
Z80OPCODE op_3C(Z80 *cpu) { // inc a
   inc8(cpu, cpu->a);
}
Z80OPCODE op_3D(Z80 *cpu) { // dec a
   dec8(cpu, cpu->a);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_3E(Z80 *cpu) { // ld a,nn
   cpu->a = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_3F(Z80 *cpu) { // ccf
   cpu->f = ((cpu->f & ~(F3|F5|NF)) | (cpu->a & (F3|F5))) ^ CF;
}
#endif

#define op_40 op_00     // ld b,b
#define op_49 op_00     // ld c,c
#define op_52 op_00     // ld d,d
#define op_5B op_00     // ld e,e
#define op_64 op_00     // ld h,h
#define op_6D op_00     // ld l,l
#define op_7F op_00     // ld a,a

#ifdef Z80_COMMON
Z80OPCODE op_41(Z80 *cpu) { // ld b,c
   cpu->b = cpu->c;
}
Z80OPCODE op_42(Z80 *cpu) { // ld b,d
   cpu->b = cpu->d;
}
Z80OPCODE op_43(Z80 *cpu) { // ld b,e
   cpu->b = cpu->e;
}
Z80OPCODE op_44(Z80 *cpu) { // ld b,h
   cpu->b = cpu->h;
}
Z80OPCODE op_45(Z80 *cpu) { // ld b,l
   cpu->b = cpu->l;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_46(Z80 *cpu) { // ld b,(hl)
   cpu->b = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_47(Z80 *cpu) { // ld b,a
   cpu->b = cpu->a;
}
Z80OPCODE op_48(Z80 *cpu) { // ld c,b
   cpu->c = cpu->b;
}
Z80OPCODE op_4A(Z80 *cpu) { // ld c,d
   cpu->c = cpu->d;
}
Z80OPCODE op_4B(Z80 *cpu) { // ld c,e
   cpu->c = cpu->e;
}
Z80OPCODE op_4C(Z80 *cpu) { // ld c,h
   cpu->c = cpu->h;
}
Z80OPCODE op_4D(Z80 *cpu) { // ld c,l
   cpu->c = cpu->l;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_4E(Z80 *cpu) { // ld c,(hl)
   cpu->c = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_4F(Z80 *cpu) { // ld c,a
   cpu->c = cpu->a;
}
Z80OPCODE op_50(Z80 *cpu) { // ld d,b
   cpu->d = cpu->b;
}
Z80OPCODE op_51(Z80 *cpu) { // ld d,c
   cpu->d = cpu->c;
}
Z80OPCODE op_53(Z80 *cpu) { // ld d,e
   cpu->d = cpu->e;
}
Z80OPCODE op_54(Z80 *cpu) { // ld d,h
   cpu->d = cpu->h;
}
Z80OPCODE op_55(Z80 *cpu) { // ld d,l
   cpu->d = cpu->l;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_56(Z80 *cpu) { // ld d,(hl)
   cpu->d = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_57(Z80 *cpu) { // ld d,a
   cpu->d = cpu->a;
}
Z80OPCODE op_58(Z80 *cpu) { // ld e,b
   cpu->e = cpu->b;
}
Z80OPCODE op_59(Z80 *cpu) { // ld e,c
   cpu->e = cpu->c;
}
Z80OPCODE op_5A(Z80 *cpu) { // ld e,d
   cpu->e = cpu->d;
}
Z80OPCODE op_5C(Z80 *cpu) { // ld e,h
   cpu->e = cpu->h;
}
Z80OPCODE op_5D(Z80 *cpu) { // ld e,l
   cpu->e = cpu->l;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_5E(Z80 *cpu) { // ld e,(hl)
   cpu->e = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_5F(Z80 *cpu) { // ld e,a
   cpu->e = cpu->a;
}
Z80OPCODE op_60(Z80 *cpu) { // ld h,b
   cpu->h = cpu->b;
}
Z80OPCODE op_61(Z80 *cpu) { // ld h,c
   cpu->h = cpu->c;
}
Z80OPCODE op_62(Z80 *cpu) { // ld h,d
   cpu->h = cpu->d;
}
Z80OPCODE op_63(Z80 *cpu) { // ld h,e
   cpu->h = cpu->e;
}
Z80OPCODE op_65(Z80 *cpu) { // ld h,l
   cpu->h = cpu->l;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_66(Z80 *cpu) { // ld h,(hl)
   cpu->h = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_67(Z80 *cpu) { // ld h,a
   cpu->h = cpu->a;
}
Z80OPCODE op_68(Z80 *cpu) { // ld l,b
   cpu->l = cpu->b;
}
Z80OPCODE op_69(Z80 *cpu) { // ld l,c
   cpu->l = cpu->c;
}
Z80OPCODE op_6A(Z80 *cpu) { // ld l,d
   cpu->l = cpu->d;
}
Z80OPCODE op_6B(Z80 *cpu) { // ld l,e
   cpu->l = cpu->e;
}
Z80OPCODE op_6C(Z80 *cpu) { // ld l,h
   cpu->l = cpu->h;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_6E(Z80 *cpu) { // ld l,(hl)
   cpu->l = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_6F(Z80 *cpu) { // ld l,a
   cpu->l = cpu->a;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_70(Z80 *cpu) { // ld (hl),b
   wm(cpu->hl, cpu->b);
   cpu->t += 3;
}
Z80OPCODE op_71(Z80 *cpu) { // ld (hl),c
   wm(cpu->hl, cpu->c);
   cpu->t += 3;
}
Z80OPCODE op_72(Z80 *cpu) { // ld (hl),d
   wm(cpu->hl, cpu->d);
   cpu->t += 3;
}
Z80OPCODE op_73(Z80 *cpu) { // ld (hl),e
   wm(cpu->hl, cpu->e);
   cpu->t += 3;
}
Z80OPCODE op_74(Z80 *cpu) { // ld (hl),h
   wm(cpu->hl, cpu->h);
   cpu->t += 3;
}
Z80OPCODE op_75(Z80 *cpu) { // ld (hl),l
   wm(cpu->hl, cpu->l);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_76(Z80 *cpu) { // halt
   cpu->halted = 1;
   cpu->haltpos = cpu->t;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_77(Z80 *cpu) { // ld (hl),a
   wm(cpu->hl, cpu->a);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_78(Z80 *cpu) { // ld a,b
   cpu->a = cpu->b;
}
Z80OPCODE op_79(Z80 *cpu) { // ld a,c
   cpu->a = cpu->c;
}
Z80OPCODE op_7A(Z80 *cpu) { // ld a,d
   cpu->a = cpu->d;
}
Z80OPCODE op_7B(Z80 *cpu) { // ld a,e
   cpu->a = cpu->e;
}
Z80OPCODE op_7C(Z80 *cpu) { // ld a,h
   cpu->a = cpu->h;
}
Z80OPCODE op_7D(Z80 *cpu) { // ld a,l
   cpu->a = cpu->l;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_7E(Z80 *cpu) { // ld a,(hl)
   cpu->a = rm(cpu->hl);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_80(Z80 *cpu) { // add a,b
   add8(cpu, cpu->b);
}
Z80OPCODE op_81(Z80 *cpu) { // add a,c
   add8(cpu, cpu->c);
}
Z80OPCODE op_82(Z80 *cpu) { // add a,d
   add8(cpu, cpu->d);
}
Z80OPCODE op_83(Z80 *cpu) { // add a,e
   add8(cpu, cpu->e);
}
Z80OPCODE op_84(Z80 *cpu) { // add a,h
   add8(cpu, cpu->h);
}
Z80OPCODE op_85(Z80 *cpu) { // add a,l
   add8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_86(Z80 *cpu) { // add a,(hl)
   add8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_87(Z80 *cpu) { // add a,a
   add8(cpu, cpu->a);
}
Z80OPCODE op_88(Z80 *cpu) { // adc a,b
   adc8(cpu, cpu->b);
}
Z80OPCODE op_89(Z80 *cpu) { // adc a,c
   adc8(cpu, cpu->c);
}
Z80OPCODE op_8A(Z80 *cpu) { // adc a,d
   adc8(cpu, cpu->d);
}
Z80OPCODE op_8B(Z80 *cpu) { // adc a,e
   adc8(cpu, cpu->e);
}
Z80OPCODE op_8C(Z80 *cpu) { // adc a,h
   adc8(cpu, cpu->h);
}
Z80OPCODE op_8D(Z80 *cpu) { // adc a,l
   adc8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_8E(Z80 *cpu) { // adc a,(hl)
   adc8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_8F(Z80 *cpu) { // adc a,a
   adc8(cpu, cpu->a);
}
Z80OPCODE op_90(Z80 *cpu) { // sub b
   sub8(cpu, cpu->b);
}
Z80OPCODE op_91(Z80 *cpu) { // sub c
   sub8(cpu, cpu->c);
}
Z80OPCODE op_92(Z80 *cpu) { // sub d
   sub8(cpu, cpu->d);
}
Z80OPCODE op_93(Z80 *cpu) { // sub e
   sub8(cpu, cpu->e);
}
Z80OPCODE op_94(Z80 *cpu) { // sub h
   sub8(cpu, cpu->h);
}
Z80OPCODE op_95(Z80 *cpu) { // sub l
   sub8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_96(Z80 *cpu) { // sub (hl)
   sub8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_97(Z80 *cpu) { // sub a
   cpu->af = ZF | NF;
}
Z80OPCODE op_98(Z80 *cpu) { // sbc a,b
   sbc8(cpu, cpu->b);
}
Z80OPCODE op_99(Z80 *cpu) { // sbc a,c
   sbc8(cpu, cpu->c);
}
Z80OPCODE op_9A(Z80 *cpu) { // sbc a,d
   sbc8(cpu, cpu->d);
}
Z80OPCODE op_9B(Z80 *cpu) { // sbc a,e
   sbc8(cpu, cpu->e);
}
Z80OPCODE op_9C(Z80 *cpu) { // sbc a,h
   sbc8(cpu, cpu->h);
}
Z80OPCODE op_9D(Z80 *cpu) { // sbc a,l
   sbc8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_9E(Z80 *cpu) { // sbc a,(hl)
   sbc8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_9F(Z80 *cpu) { // sbc a,a
   sbc8(cpu, cpu->a);
}
Z80OPCODE op_A0(Z80 *cpu) { // and b
   and8(cpu, cpu->b);
}
Z80OPCODE op_A1(Z80 *cpu) { // and c
   and8(cpu, cpu->c);
}
Z80OPCODE op_A2(Z80 *cpu) { // and d
   and8(cpu, cpu->d);
}
Z80OPCODE op_A3(Z80 *cpu) { // and e
   and8(cpu, cpu->e);
}
Z80OPCODE op_A4(Z80 *cpu) { // and h
   and8(cpu, cpu->h);
}
Z80OPCODE op_A5(Z80 *cpu) { // and l
   and8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_A6(Z80 *cpu) { // and (hl)
   and8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_A7(Z80 *cpu) { // and a
   and8(cpu, cpu->a); // already optimized by compiler
}
Z80OPCODE op_A8(Z80 *cpu) { // xor b
   xor8(cpu, cpu->b);
}
Z80OPCODE op_A9(Z80 *cpu) { // xor c
   xor8(cpu, cpu->c);
}
Z80OPCODE op_AA(Z80 *cpu) { // xor d
   xor8(cpu, cpu->d);
}
Z80OPCODE op_AB(Z80 *cpu) { // xor e
   xor8(cpu, cpu->e);
}
Z80OPCODE op_AC(Z80 *cpu) { // xor h
   xor8(cpu, cpu->h);
}
Z80OPCODE op_AD(Z80 *cpu) { // xor l
   xor8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_AE(Z80 *cpu) { // xor (hl)
   xor8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_AF(Z80 *cpu) { // xor a
   cpu->af = ZF | PV;
}
Z80OPCODE op_B0(Z80 *cpu) { // or b
   or8(cpu, cpu->b);
}
Z80OPCODE op_B1(Z80 *cpu) { // or c
   or8(cpu, cpu->c);
}
Z80OPCODE op_B2(Z80 *cpu) { // or d
   or8(cpu, cpu->d);
}
Z80OPCODE op_B3(Z80 *cpu) { // or e
   or8(cpu, cpu->e);
}
Z80OPCODE op_B4(Z80 *cpu) { // or h
   or8(cpu, cpu->h);
}
Z80OPCODE op_B5(Z80 *cpu) { // or l
   or8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_B6(Z80 *cpu) { // or (hl)
   or8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_B7(Z80 *cpu) { // or a
   or8(cpu, cpu->a);  // already optimized by compiler
}
Z80OPCODE op_B8(Z80 *cpu) { // cp b
   cp8(cpu, cpu->b);
}
Z80OPCODE op_B9(Z80 *cpu) { // cp c
   cp8(cpu, cpu->c);
}
Z80OPCODE op_BA(Z80 *cpu) { // cp d
   cp8(cpu, cpu->d);
}
Z80OPCODE op_BB(Z80 *cpu) { // cp e
   cp8(cpu, cpu->e);
}
Z80OPCODE op_BC(Z80 *cpu) { // cp h
   cp8(cpu, cpu->h);
}
Z80OPCODE op_BD(Z80 *cpu) { // cp l
   cp8(cpu, cpu->l);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_BE(Z80 *cpu) { // cp (hl)
   cp8(cpu, rm(cpu->hl));
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_BF(Z80 *cpu) { // cp a
   cp8(cpu, cpu->a); // can't optimize: F3,F5 depends on A
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_C0(Z80 *cpu) { // ret nz
   if (!(cpu->f & ZF)) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
Z80OPCODE op_C1(Z80 *cpu) { // pop bc
   cpu->c = rm(cpu->sp++);
   cpu->b = rm(cpu->sp++);
   cpu->t += 6;
}
Z80OPCODE op_C2(Z80 *cpu) { // jp nz,nnnn
   cpu->t += 6;
   if (!(cpu->f & ZF)) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
Z80OPCODE op_C3(Z80 *cpu) { // jp nnnn
   unsigned lo = rm(cpu->pc++);
   cpu->pc = lo + 0x100*rm(cpu->pc);
   cpu->t += 6;
}
Z80OPCODE op_C4(Z80 *cpu) { // call nz,nnnn
   cpu->pc += 2;
   if (!(cpu->f & ZF)) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_C5(Z80 *cpu) { // push bc
   wm(--cpu->sp, cpu->b);
   wm(--cpu->sp, cpu->c);
   cpu->t += 7;
}
Z80OPCODE op_C6(Z80 *cpu) { // add a,nn
   add8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_C7(Z80 *cpu) { // rst 00
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x00;
   cpu->t += 7;
}
Z80OPCODE op_C8(Z80 *cpu) { // ret z
   if (cpu->f & ZF) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
Z80OPCODE op_C9(Z80 *cpu) { // ret
   cpu->pcl = rm(cpu->sp++);
   cpu->pch = rm(cpu->sp++);
   cpu->t += 6;
}
Z80OPCODE op_CA(Z80 *cpu) { // jp z,nnnn
   cpu->t += 6;
   if (cpu->f & ZF) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
Z80OPCODE op_CC(Z80 *cpu) { // call z,nnnn
   cpu->pc += 2;
   if (cpu->f & ZF) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_CD(Z80 *cpu) { // call
   unsigned addr = rm(cpu->pc++);
   addr += 0x100*rm(cpu->pc++);
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = addr;
   cpu->t += 13;
}
Z80OPCODE op_CE(Z80 *cpu) { // adc a,nn
   adc8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_CF(Z80 *cpu) { // rst 08
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x08;
   cpu->t += 7;
}
Z80OPCODE op_D0(Z80 *cpu) { // ret nc
   if (!(cpu->f & CF)) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
Z80OPCODE op_D1(Z80 *cpu) { // pop de
   cpu->e = rm(cpu->sp++);
   cpu->d = rm(cpu->sp++);
   cpu->t += 6;
}
Z80OPCODE op_D2(Z80 *cpu) { // jp nc,nnnn
   cpu->t += 6;
   if (!(cpu->f & CF)) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
Z80OPCODE op_D3(Z80 *cpu) { // out (nn),a
   unsigned port = rm(cpu->pc++);
   cpu->t += 7;
   out(port + cpu->a * 0x100, cpu->a);
}
Z80OPCODE op_D4(Z80 *cpu) { // call nc,nnnn
   cpu->pc += 2;
   if (!(cpu->f & CF)) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_D5(Z80 *cpu) { // push de
   wm(--cpu->sp, cpu->d);
   wm(--cpu->sp, cpu->e);
   cpu->t += 7;
}
Z80OPCODE op_D6(Z80 *cpu) { // sub nn
   sub8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_D7(Z80 *cpu) { // rst 10
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x10;
   cpu->t += 7;
}
Z80OPCODE op_D8(Z80 *cpu) { // ret c
   if (cpu->f & CF) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_D9(Z80 *cpu) { // exx
   unsigned tmp = cpu->bc; cpu->bc = cpu->alt.bc; cpu->alt.bc = tmp;
   tmp = cpu->de; cpu->de = cpu->alt.de; cpu->alt.de = tmp;
   tmp = cpu->hl; cpu->hl = cpu->alt.hl; cpu->alt.hl = tmp;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_DA(Z80 *cpu) { // jp c,nnnn
   cpu->t += 6;
   if (cpu->f & CF) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
Z80OPCODE op_DB(Z80 *cpu) { // in a,(nn)
   unsigned port = rm(cpu->pc++);
   cpu->t += 7;
   cpu->a = in(port + cpu->a * 0x100);
}
Z80OPCODE op_DC(Z80 *cpu) { // call c,nnnn
   cpu->pc += 2;
   if (cpu->f & CF) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_DE(Z80 *cpu) { // sbc a,nn
   sbc8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_DF(Z80 *cpu) { // rst 18
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x18;
   cpu->t += 7;
}
Z80OPCODE op_E0(Z80 *cpu) { // ret po
   if (!(cpu->f & PV)) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
Z80OPCODE op_E1(Z80 *cpu) { // pop hl
   cpu->l = rm(cpu->sp++);
   cpu->h = rm(cpu->sp++);
   cpu->t += 6;
}
Z80OPCODE op_E2(Z80 *cpu) { // jp po,nnnn
   cpu->t += 6;
   if (!(cpu->f & PV)) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
Z80OPCODE op_E3(Z80 *cpu) { // ex (sp),hl
   unsigned tmp = rm(cpu->sp) + 0x100*rm(cpu->sp + 1);
   wm(cpu->sp, cpu->l);
   wm(cpu->sp+1, cpu->h);
   cpu->hl = tmp;
   cpu->t += 15;
}
Z80OPCODE op_E4(Z80 *cpu) { // call po,nnnn
   cpu->pc += 2;
   if (!(cpu->f & PV)) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_E5(Z80 *cpu) { // push hl
   wm(--cpu->sp, cpu->h);
   wm(--cpu->sp, cpu->l);
   cpu->t += 7;
}
Z80OPCODE op_E6(Z80 *cpu) { // and nn
   and8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_E7(Z80 *cpu) { // rst 20
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x20;
   cpu->t += 7;
}
Z80OPCODE op_E8(Z80 *cpu) { // ret pe
   if (cpu->f & PV) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_E9(Z80 *cpu) { // jp (hl)
   cpu->pc = cpu->hl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_EA(Z80 *cpu) { // jp pe,nnnn
   cpu->t += 6;
   if (cpu->f & PV) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_EB(Z80 *cpu) { // ex de,hl
   unsigned tmp = cpu->de;
   cpu->de = cpu->hl;
   cpu->hl = tmp;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_EC(Z80 *cpu) { // call pe,nnnn
   cpu->pc += 2;
   if (cpu->f & PV) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_EE(Z80 *cpu) { // xor nn
   xor8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_EF(Z80 *cpu) { // rst 28
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x28;
   cpu->t += 7;
}
Z80OPCODE op_F0(Z80 *cpu) { // ret p
   if (!(cpu->f & SF)) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
Z80OPCODE op_F1(Z80 *cpu) { // pop af
   cpu->f = rm(cpu->sp++);
   cpu->a = rm(cpu->sp++);
   cpu->t += 6;
}
Z80OPCODE op_F2(Z80 *cpu) { // jp p,nnnn
   cpu->t += 6;
   if (!(cpu->f & SF)) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_F3(Z80 *cpu) { // di
   cpu->iff1 = cpu->iff2 = 0;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_F4(Z80 *cpu) { // call p,nnnn
   cpu->pc += 2;
   if (!(cpu->f & SF)) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_F5(Z80 *cpu) { // push af
   wm(--cpu->sp, cpu->a);
   wm(--cpu->sp, cpu->f);
   cpu->t += 7;
}
Z80OPCODE op_F6(Z80 *cpu) { // or nn
   or8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_F7(Z80 *cpu) { // rst 30
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x30;
   cpu->t += 7;
}
Z80OPCODE op_F8(Z80 *cpu) { // ret m
   if (cpu->f & SF) {
      cpu->pcl = rm(cpu->sp++);
      cpu->pch = rm(cpu->sp++);
      cpu->t += 7;
   } else cpu->t += 1;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_F9(Z80 *cpu) { // ld sp,hl
   cpu->sp = cpu->hl;
   cpu->t += 2;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_FA(Z80 *cpu) { // jp m,nnnn
   cpu->t += 6;
   if (cpu->f & SF) {
      unsigned addr = rm(cpu->pc);
      addr += 0x100*rm(cpu->pc+1);
      cpu->pc = addr;
   } else cpu->pc += 2;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE op_FB(Z80 *cpu) { // ei
   cpu->iff1 = cpu->iff2 = 1;
   cpu->eipos = cpu->t;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE op_FC(Z80 *cpu) { // call m,nnnn
   cpu->pc += 2;
   if (cpu->f & SF) {
      unsigned addr = rm(cpu->pc-2);
      addr += 0x100*rm(cpu->pc-1);
      wm(--cpu->sp, cpu->pch);
      wm(--cpu->sp, cpu->pcl);
      cpu->pc = addr;
      cpu->t += 13;
   } else cpu->t += 6;
}
Z80OPCODE op_FE(Z80 *cpu) { // cp nn
   cp8(cpu, rm(cpu->pc++));
   cpu->t += 3;
}
Z80OPCODE op_FF(Z80 *cpu) { // rst 38
   wm(--cpu->sp, cpu->pch);
   wm(--cpu->sp, cpu->pcl);
   cpu->pc = 0x38;
   cpu->t += 7;
}

Z80OPCODE op_CB(Z80 *cpu);
Z80OPCODE op_ED(Z80 *cpu);
Z80OPCODE op_DD(Z80 *cpu);
Z80OPCODE op_FD(Z80 *cpu);

STEPFUNC const normal_opcode[0x100] = {

   op_00, op_01, op_02, op_03, op_04, op_05, op_06, op_07,
   op_08, op_09, op_0A, op_0B, op_0C, op_0D, op_0E, op_0F,
   op_10, op_11, op_12, op_13, op_14, op_15, op_16, op_17,
   op_18, op_19, op_1A, op_1B, op_1C, op_1D, op_1E, op_1F,
   op_20, op_21, op_22, op_23, op_24, op_25, op_26, op_27,
   op_28, op_29, op_2A, op_2B, op_2C, op_2D, op_2E, op_2F,
   op_30, op_31, op_32, op_33, op_34, op_35, op_36, op_37,
   op_38, op_39, op_3A, op_3B, op_3C, op_3D, op_3E, op_3F,

   op_40, op_41, op_42, op_43, op_44, op_45, op_46, op_47,
   op_48, op_49, op_4A, op_4B, op_4C, op_4D, op_4E, op_4F,
   op_50, op_51, op_52, op_53, op_54, op_55, op_56, op_57,
   op_58, op_59, op_5A, op_5B, op_5C, op_5D, op_5E, op_5F,
   op_60, op_61, op_62, op_63, op_64, op_65, op_66, op_67,
   op_68, op_69, op_6A, op_6B, op_6C, op_6D, op_6E, op_6F,
   op_70, op_71, op_72, op_73, op_74, op_75, op_76, op_77,
   op_78, op_79, op_7A, op_7B, op_7C, op_7D, op_7E, op_7F,

   op_80, op_81, op_82, op_83, op_84, op_85, op_86, op_87,
   op_88, op_89, op_8A, op_8B, op_8C, op_8D, op_8E, op_8F,
   op_90, op_91, op_92, op_93, op_94, op_95, op_96, op_97,
   op_98, op_99, op_9A, op_9B, op_9C, op_9D, op_9E, op_9F,
   op_A0, op_A1, op_A2, op_A3, op_A4, op_A5, op_A6, op_A7,
   op_A8, op_A9, op_AA, op_AB, op_AC, op_AD, op_AE, op_AF,
   op_B0, op_B1, op_B2, op_B3, op_B4, op_B5, op_B6, op_B7,
   op_B8, op_B9, op_BA, op_BB, op_BC, op_BD, op_BE, op_BF,

   op_C0, op_C1, op_C2, op_C3, op_C4, op_C5, op_C6, op_C7,
   op_C8, op_C9, op_CA, op_CB, op_CC, op_CD, op_CE, op_CF,
   op_D0, op_D1, op_D2, op_D3, op_D4, op_D5, op_D6, op_D7,
   op_D8, op_D9, op_DA, op_DB, op_DC, op_DD, op_DE, op_DF,
   op_E0, op_E1, op_E2, op_E3, op_E4, op_E5, op_E6, op_E7,
   op_E8, op_E9, op_EA, op_EB, op_EC, op_ED, op_EE, op_EF,
   op_F0, op_F1, op_F2, op_F3, op_F4, op_F5, op_F6, op_F7,
   op_F8, op_F9, op_FA, op_FB, op_FC, op_FD, op_FE, op_FF,

};
#endif