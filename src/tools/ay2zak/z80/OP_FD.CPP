/* FD prefix opcodes */

#ifdef Z80_COMMON
Z80OPCODE opy_09(Z80 *cpu) { // add iy,bc
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= (((cpu->iy & 0x0FFF) + (cpu->bc & 0x0FFF)) >> 8) & 0x10; /* HF */
   cpu->iy = (cpu->iy & 0xFFFF) + (cpu->bc & 0xFFFF);
   if (cpu->iy & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->yh & (F5 | F3));
   cpu->t += 7;
}
Z80OPCODE opy_19(Z80 *cpu) { // add iy,de
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= (((cpu->iy & 0x0FFF) + (cpu->de & 0x0FFF)) >> 8) & 0x10; /* HF */
   cpu->iy = (cpu->iy & 0xFFFF) + (cpu->de & 0xFFFF);
   if (cpu->iy & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->yh & (F5 | F3));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_21(Z80 *cpu) { // ld iy,nnnn
   cpu->yl = rm(cpu->pc++);
   cpu->yh = rm(cpu->pc++);
   cpu->t += 6;
}
Z80OPCODE opy_22(Z80 *cpu) { // ld (nnnn),iy
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   wm(adr, cpu->yl);
   wm(adr+1, cpu->yh);
   cpu->t += 12;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_23(Z80 *cpu) { // inc iy
   cpu->iy++;
   cpu->t += 2;
}
Z80OPCODE opy_24(Z80 *cpu) { // inc yh
   inc8(cpu, cpu->yh);
}
Z80OPCODE opy_25(Z80 *cpu) { // dec yh
   dec8(cpu, cpu->yh);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_26(Z80 *cpu) { // ld yh,nn
   cpu->yh = rm(cpu->pc++);
   cpu->t += 3;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_29(Z80 *cpu) { // add iy,iy
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= ((cpu->yh << 1) & 0x10); /* HF */
   cpu->iy = (cpu->iy & 0xFFFF)*2;
   if (cpu->iy & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->yh & (F5 | F3));
   cpu->t += 7;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_2A(Z80 *cpu) { // ld iy,(nnnn)
   unsigned adr = rm(cpu->pc++);
   adr += rm(cpu->pc++)*0x100;
   cpu->yl = rm(adr);
   cpu->yh = rm(adr+1);
   cpu->t += 12;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_2B(Z80 *cpu) { // dec iy
   cpu->iy--;
   cpu->t += 2;
}
Z80OPCODE opy_2C(Z80 *cpu) { // inc yl
   inc8(cpu, cpu->yl);
}
Z80OPCODE opy_2D(Z80 *cpu) { // dec yl
   dec8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_2E(Z80 *cpu) { // ld yl,nn
   cpu->yl = rm(cpu->pc++);
   cpu->t += 3;
}
Z80OPCODE opy_34(Z80 *cpu) { // inc (iy+nn)
   signed char ofs = rm(cpu->pc++);
   unsigned char t = rm(cpu->iy + ofs);
   inc8(cpu, t);
   wm(cpu->iy + ofs, t);
   cpu->t += 15;
}
Z80OPCODE opy_35(Z80 *cpu) { // dec (iy+nn)
   signed char ofs = rm(cpu->pc++);
   unsigned char t = rm(cpu->iy + ofs);
   dec8(cpu, t);
   wm(cpu->iy + ofs, t);
   cpu->t += 15;
}
Z80OPCODE opy_36(Z80 *cpu) { // ld (iy+nn),nn
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, rm(cpu->pc++));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_39(Z80 *cpu) { // add iy,sp
   cpu->f = (cpu->f & ~(NF | CF | F5 | F3 | HF));
   cpu->f |= (((cpu->iy & 0x0FFF) + (cpu->sp & 0x0FFF)) >> 8) & 0x10; /* HF */
   cpu->iy = (cpu->iy & 0xFFFF) + (cpu->sp & 0xFFFF);
   if (cpu->iy & 0x10000) cpu->f |= CF;
   cpu->f |= (cpu->yl & (F5 | F3));
   cpu->t += 7;
}
Z80OPCODE opy_44(Z80 *cpu) { // ld b,yh
   cpu->b = cpu->yh;
}
Z80OPCODE opy_45(Z80 *cpu) { // ld b,yl
   cpu->b = cpu->yl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_46(Z80 *cpu) { // ld b,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->b = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_4C(Z80 *cpu) { // ld c,yh
   cpu->c = cpu->yh;
}
Z80OPCODE opy_4D(Z80 *cpu) { // ld c,yl
   cpu->c = cpu->yl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_4E(Z80 *cpu) { // ld c,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->c = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_54(Z80 *cpu) { // ld d,yh
   cpu->d = cpu->yh;
}
Z80OPCODE opy_55(Z80 *cpu) { // ld d,yl
   cpu->d = cpu->yl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_56(Z80 *cpu) { // ld d,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->d = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_5C(Z80 *cpu) { // ld e,yh
   cpu->e = cpu->yh;
}
Z80OPCODE opy_5D(Z80 *cpu) { // ld e,yl
   cpu->e = cpu->yl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_5E(Z80 *cpu) { // ld e,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->e = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_60(Z80 *cpu) { // ld yh,b
   cpu->yh = cpu->b;
}
Z80OPCODE opy_61(Z80 *cpu) { // ld yh,c
   cpu->yh = cpu->c;
}
Z80OPCODE opy_62(Z80 *cpu) { // ld yh,d
   cpu->yh = cpu->d;
}
Z80OPCODE opy_63(Z80 *cpu) { // ld yh,e
   cpu->yh = cpu->e;
}
Z80OPCODE opy_65(Z80 *cpu) { // ld yh,yl
   cpu->yh = cpu->yl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_66(Z80 *cpu) { // ld h,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->h = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_67(Z80 *cpu) { // ld yh,a
   cpu->yh = cpu->a;
}
Z80OPCODE opy_68(Z80 *cpu) { // ld yl,b
   cpu->yl = cpu->b;
}
Z80OPCODE opy_69(Z80 *cpu) { // ld yl,c
   cpu->yl = cpu->c;
}
Z80OPCODE opy_6A(Z80 *cpu) { // ld yl,d
   cpu->yl = cpu->d;
}
Z80OPCODE opy_6B(Z80 *cpu) { // ld yl,e
   cpu->yl = cpu->e;
}
Z80OPCODE opy_6C(Z80 *cpu) { // ld yl,yh
   cpu->yl = cpu->yh;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_6E(Z80 *cpu) { // ld l,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->l = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_6F(Z80 *cpu) { // ld yl,a
   cpu->yl = cpu->a;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_70(Z80 *cpu) { // ld (iy+nn),b
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->b);
   cpu->t += 11;
}
Z80OPCODE opy_71(Z80 *cpu) { // ld (iy+nn),c
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->c);
   cpu->t += 11;
}
Z80OPCODE opy_72(Z80 *cpu) { // ld (iy+nn),d
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->d);
   cpu->t += 11;
}
Z80OPCODE opy_73(Z80 *cpu) { // ld (iy+nn),e
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->e);
   cpu->t += 11;
}
Z80OPCODE opy_74(Z80 *cpu) { // ld (iy+nn),h
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->h);
   cpu->t += 11;
}
Z80OPCODE opy_75(Z80 *cpu) { // ld (iy+nn),l
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->l);
   cpu->t += 11;
}
Z80OPCODE opy_77(Z80 *cpu) { // ld (iy+nn),a
   signed char ofs = rm(cpu->pc++);
   wm(cpu->iy + ofs, cpu->a);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_7C(Z80 *cpu) { // ld a,yh
   cpu->a = cpu->yh;
}
Z80OPCODE opy_7D(Z80 *cpu) { // ld a,yl
   cpu->a = cpu->yl;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_7E(Z80 *cpu) { // ld a,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   cpu->a = rm(cpu->iy + ofs);
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_84(Z80 *cpu) { // add a,yh
   add8(cpu, cpu->yh);
}
Z80OPCODE opy_85(Z80 *cpu) { // add a,yl
   add8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_86(Z80 *cpu) { // add a,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   add8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_8C(Z80 *cpu) { // adc a,yh
   adc8(cpu, cpu->yh);
}
Z80OPCODE opy_8D(Z80 *cpu) { // adc a,yl
   adc8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_8E(Z80 *cpu) { // adc a,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   adc8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_94(Z80 *cpu) { // sub yh
   sub8(cpu, cpu->yh);
}
Z80OPCODE opy_95(Z80 *cpu) { // sub yl
   sub8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_96(Z80 *cpu) { // sub (iy+nn)
   signed char ofs = rm(cpu->pc++);
   sub8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_9C(Z80 *cpu) { // sbc a,yh
   sbc8(cpu, cpu->yh);
}
Z80OPCODE opy_9D(Z80 *cpu) { // sbc a,yl
   sbc8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_9E(Z80 *cpu) { // sbc a,(iy+nn)
   signed char ofs = rm(cpu->pc++);
   sbc8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_A4(Z80 *cpu) { // and yh
   and8(cpu, cpu->yh);
}
Z80OPCODE opy_A5(Z80 *cpu) { // and yl
   and8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_A6(Z80 *cpu) { // and (iy+nn)
   signed char ofs = rm(cpu->pc++);
   and8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_AC(Z80 *cpu) { // xor yh
   xor8(cpu, cpu->yh);
}
Z80OPCODE opy_AD(Z80 *cpu) { // xor yl
   xor8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_AE(Z80 *cpu) { // xor (iy+nn)
   signed char ofs = rm(cpu->pc++);
   xor8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_B4(Z80 *cpu) { // or yh
   or8(cpu, cpu->yh);
}
Z80OPCODE opy_B5(Z80 *cpu) { // or yl
   or8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_B6(Z80 *cpu) { // or (iy+nn)
   signed char ofs = rm(cpu->pc++);
   or8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_BC(Z80 *cpu) { // cp yh
   cp8(cpu, cpu->yh);
}
Z80OPCODE opy_BD(Z80 *cpu) { // cp yl
   cp8(cpu, cpu->yl);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opy_BE(Z80 *cpu) { // cp (iy+nn)
   signed char ofs = rm(cpu->pc++);
   cp8(cpu, rm(cpu->iy + ofs));
   cpu->t += 11;
}
Z80OPCODE opy_E1(Z80 *cpu) { // pop iy
   cpu->yl = rm(cpu->sp++);
   cpu->yh = rm(cpu->sp++);
   cpu->t += 6;
}
Z80OPCODE opy_E3(Z80 *cpu) { // ex (sp),iy
   unsigned tmp = rm(cpu->sp) + 0x100*rm(cpu->sp + 1);
   wm(cpu->sp, cpu->yl);
   wm(cpu->sp+1, cpu->yh);
   cpu->iy = tmp;
   cpu->t += 15;
}
Z80OPCODE opy_E5(Z80 *cpu) { // push iy
   wm(--cpu->sp, cpu->yh);
   wm(--cpu->sp, cpu->yl);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opy_E9(Z80 *cpu) { // jp (iy)
   cpu->pc = cpu->iy;
}
Z80OPCODE opy_F9(Z80 *cpu) { // ld sp,iy
   cpu->sp = cpu->iy;
   cpu->t += 2;
}
#endif
#ifndef Z80_COMMON

STEPFUNC const iy_opcode[0x100] = {

    op_00,  op_01,  op_02,  op_03,  op_04,  op_05,  op_06,  op_07,
    op_08, opy_09,  op_0A,  op_0B,  op_0C,  op_0D,  op_0E,  op_0F,
    op_10,  op_11,  op_12,  op_13,  op_14,  op_15,  op_16,  op_17,
    op_18, opy_19,  op_1A,  op_1B,  op_1C,  op_1D,  op_1E,  op_1F,
    op_20, opy_21, opy_22, opy_23, opy_24, opy_25, opy_26,  op_27,
    op_28, opy_29, opy_2A, opy_2B, opy_2C, opy_2D, opy_2E,  op_2F,
    op_30,  op_31,  op_32,  op_33, opy_34, opy_35, opy_36,  op_37,
    op_38, opy_39,  op_3A,  op_3B,  op_3C,  op_3D,  op_3E,  op_3F,

    op_40,  op_41,  op_42,  op_43, opy_44, opy_45, opy_46,  op_47,
    op_48,  op_49,  op_4A,  op_4B, opy_4C, opy_4D, opy_4E,  op_4F,
    op_50,  op_51,  op_52,  op_53, opy_54, opy_55, opy_56,  op_57,
    op_58,  op_59,  op_5A,  op_5B, opy_5C, opy_5D, opy_5E,  op_5F,
   opy_60, opy_61, opy_62, opy_63,  op_64, opy_65, opy_66, opy_67,
   opy_68, opy_69, opy_6A, opy_6B, opy_6C,  op_6D, opy_6E, opy_6F,
   opy_70, opy_71, opy_72, opy_73, opy_74, opy_75,  op_76, opy_77,
    op_78,  op_79,  op_7A,  op_7B, opy_7C, opy_7D, opy_7E,  op_7F,

    op_80,  op_81,  op_82,  op_83, opy_84, opy_85, opy_86,  op_87,
    op_88,  op_89,  op_8A,  op_8B, opy_8C, opy_8D, opy_8E,  op_8F,
    op_90,  op_91,  op_92,  op_93, opy_94, opy_95, opy_96,  op_97,
    op_98,  op_99,  op_9A,  op_9B, opy_9C, opy_9D, opy_9E,  op_9F,
    op_A0,  op_A1,  op_A2,  op_A3, opy_A4, opy_A5, opy_A6,  op_A7,
    op_A8,  op_A9,  op_AA,  op_AB, opy_AC, opy_AD, opy_AE,  op_AF,
    op_B0,  op_B1,  op_B2,  op_B3, opy_B4, opy_B5, opy_B6,  op_B7,
    op_B8,  op_B9,  op_BA,  op_BB, opy_BC, opy_BD, opy_BE,  op_BF,

    op_C0,  op_C1,  op_C2,  op_C3,  op_C4,  op_C5,  op_C6,  op_C7,
    op_C8,  op_C9,  op_CA,  op_CB,  op_CC,  op_CD,  op_CE,  op_CF,
    op_D0,  op_D1,  op_D2,  op_D3,  op_D4,  op_D5,  op_D6,  op_D7,
    op_D8,  op_D9,  op_DA,  op_DB,  op_DC,  op_DD,  op_DE,  op_DF,
    op_E0, opy_E1,  op_E2, opy_E3,  op_E4, opy_E5,  op_E6,  op_E7,
    op_E8, opy_E9,  op_EA,  op_EB,  op_EC,  op_ED,  op_EE,  op_EF,
    op_F0,  op_F1,  op_F2,  op_F3,  op_F4,  op_F5,  op_F6,  op_F7,
    op_F8, opy_F9,  op_FA,  op_FB,  op_FC,  op_FD,  op_FE,  op_FF,

};
#endif