/* CB opcodes */

#ifdef Z80_COMMON
Z80OPCODE opl_00(Z80 *cpu) { // rlc b
   cpu->f = rlcf[cpu->b];
   cpu->b = rol[cpu->b];
}
Z80OPCODE opl_01(Z80 *cpu) { // rlc c
   cpu->f = rlcf[cpu->c];
   cpu->c = rol[cpu->c];
}
Z80OPCODE opl_02(Z80 *cpu) { // rlc d
   cpu->f = rlcf[cpu->d];
   cpu->d = rol[cpu->d];
}
Z80OPCODE opl_03(Z80 *cpu) { // rlc e
   cpu->f = rlcf[cpu->e];
   cpu->e = rol[cpu->e];
}
Z80OPCODE opl_04(Z80 *cpu) { // rlc h
   cpu->f = rlcf[cpu->h];
   cpu->h = rol[cpu->h];
}
Z80OPCODE opl_05(Z80 *cpu) { // rlc l
   cpu->f = rlcf[cpu->l];
   cpu->l = rol[cpu->l];
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_06(Z80 *cpu) { // rlc (hl)
   unsigned char t = rm(cpu->hl);
   cpu->f = rlcf[t];
   wm(cpu->hl, rol[t]);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_07(Z80 *cpu) { // rlc a
   cpu->f = rlcf[cpu->a];
   cpu->a = rol[cpu->a];
}
Z80OPCODE opl_08(Z80 *cpu) { // rrc b
   cpu->f = rrcf[cpu->b];
   cpu->b = ror[cpu->b];
}
Z80OPCODE opl_09(Z80 *cpu) { // rrc c
   cpu->f = rrcf[cpu->c];
   cpu->c = ror[cpu->c];
}
Z80OPCODE opl_0A(Z80 *cpu) { // rrc d
   cpu->f = rrcf[cpu->d];
   cpu->d = ror[cpu->d];
}
Z80OPCODE opl_0B(Z80 *cpu) { // rrc e
   cpu->f = rrcf[cpu->e];
   cpu->e = ror[cpu->e];
}
Z80OPCODE opl_0C(Z80 *cpu) { // rrc h
   cpu->f = rrcf[cpu->h];
   cpu->h = ror[cpu->h];
}
Z80OPCODE opl_0D(Z80 *cpu) { // rrc l
   cpu->f = rrcf[cpu->l];
   cpu->l = ror[cpu->l];
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_0E(Z80 *cpu) { // rrc (hl)
   unsigned char t = rm(cpu->hl);
   cpu->f = rrcf[t];
   wm(cpu->hl, ror[t]);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_0F(Z80 *cpu) { // rrc a
   cpu->f = rrcf[cpu->a];
   cpu->a = ror[cpu->a];
}
Z80OPCODE opl_10(Z80 *cpu) { // rl b
   if (cpu->f & CF)
      cpu->f = rl1[cpu->b], cpu->b = (cpu->b << 1) + 1;
   else
      cpu->f = rl0[cpu->b], cpu->b = (cpu->b << 1);
}
Z80OPCODE opl_11(Z80 *cpu) { // rl c
   if (cpu->f & CF)
      cpu->f = rl1[cpu->c], cpu->c = (cpu->c << 1) + 1;
   else
      cpu->f = rl0[cpu->c], cpu->c = (cpu->c << 1);
}
Z80OPCODE opl_12(Z80 *cpu) { // rl d
   if (cpu->f & CF)
      cpu->f = rl1[cpu->d], cpu->d = (cpu->d << 1) + 1;
   else
      cpu->f = rl0[cpu->d], cpu->d = (cpu->d << 1);
}
Z80OPCODE opl_13(Z80 *cpu) { // rl e
   if (cpu->f & CF)
      cpu->f = rl1[cpu->e], cpu->e = (cpu->e << 1) + 1;
   else
      cpu->f = rl0[cpu->e], cpu->e = (cpu->e << 1);
}
Z80OPCODE opl_14(Z80 *cpu) { // rl h
   if (cpu->f & CF)
      cpu->f = rl1[cpu->h], cpu->h = (cpu->h << 1) + 1;
   else
      cpu->f = rl0[cpu->h], cpu->h = (cpu->h << 1);
}
Z80OPCODE opl_15(Z80 *cpu) { // rl l
   if (cpu->f & CF)
      cpu->f = rl1[cpu->l], cpu->l = (cpu->l << 1) + 1;
   else
      cpu->f = rl0[cpu->l], cpu->l = (cpu->l << 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_16(Z80 *cpu) { // rl (hl)
   unsigned char t = rm(cpu->hl);
   if (cpu->f & CF)
      cpu->f = rl1[t], t = (t << 1) + 1;
   else
      cpu->f = rl0[t], t = (t << 1);
   wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_17(Z80 *cpu) { // rl a
   if (cpu->f & CF)
      cpu->f = rl1[cpu->a], cpu->a = (cpu->a << 1) + 1;
   else
      cpu->f = rl0[cpu->a], cpu->a = (cpu->a << 1);
}
Z80OPCODE opl_18(Z80 *cpu) { // rr b
   if (cpu->f & CF)
      cpu->f = rr1[cpu->b], cpu->b = (cpu->b >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->b], cpu->b = (cpu->b >> 1);
}
Z80OPCODE opl_19(Z80 *cpu) { // rr c
   if (cpu->f & CF)
      cpu->f = rr1[cpu->c], cpu->c = (cpu->c >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->c], cpu->c = (cpu->c >> 1);
}
Z80OPCODE opl_1A(Z80 *cpu) { // rr d
   if (cpu->f & CF)
      cpu->f = rr1[cpu->d], cpu->d = (cpu->d >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->d], cpu->d = (cpu->d >> 1);
}
Z80OPCODE opl_1B(Z80 *cpu) { // rr e
   if (cpu->f & CF)
      cpu->f = rr1[cpu->e], cpu->e = (cpu->e >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->e], cpu->e = (cpu->e >> 1);
}
Z80OPCODE opl_1C(Z80 *cpu) { // rr h
   if (cpu->f & CF)
      cpu->f = rr1[cpu->h], cpu->h = (cpu->h >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->h], cpu->h = (cpu->h >> 1);
}
Z80OPCODE opl_1D(Z80 *cpu) { // rr l
   if (cpu->f & CF)
      cpu->f = rr1[cpu->l], cpu->l = (cpu->l >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->l], cpu->l = (cpu->l >> 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_1E(Z80 *cpu) { // rr (hl)
   unsigned char t = rm(cpu->hl);
   if (cpu->f & CF)
      cpu->f = rr1[t], t = (t >> 1) | 0x80;
   else
      cpu->f = rr0[t], t = (t >> 1);
   wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_1F(Z80 *cpu) { // rr a
   if (cpu->f & CF)
      cpu->f = rr1[cpu->a], cpu->a = (cpu->a >> 1) + 0x80;
   else
      cpu->f = rr0[cpu->a], cpu->a = (cpu->a >> 1);
}
Z80OPCODE opl_20(Z80 *cpu) { // sla b
   cpu->f = rl0[cpu->b], cpu->b = (cpu->b << 1);
}
Z80OPCODE opl_21(Z80 *cpu) { // sla c
   cpu->f = rl0[cpu->c], cpu->c = (cpu->c << 1);
}
Z80OPCODE opl_22(Z80 *cpu) { // sla d
   cpu->f = rl0[cpu->d], cpu->d = (cpu->d << 1);
}
Z80OPCODE opl_23(Z80 *cpu) { // sla e
   cpu->f = rl0[cpu->e], cpu->e = (cpu->e << 1);
}
Z80OPCODE opl_24(Z80 *cpu) { // sla h
   cpu->f = rl0[cpu->h], cpu->h = (cpu->h << 1);
}
Z80OPCODE opl_25(Z80 *cpu) { // sla l
   cpu->f = rl0[cpu->l], cpu->l = (cpu->l << 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_26(Z80 *cpu) { // sla (hl)
   unsigned char t = rm(cpu->hl);
   cpu->f = rl0[t], t = (t << 1);
   wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_27(Z80 *cpu) { // sla a
   cpu->f = rl0[cpu->a], cpu->a = (cpu->a << 1);
}
Z80OPCODE opl_28(Z80 *cpu) { // sra b
   cpu->f = sraf[cpu->b], cpu->b = (cpu->b >> 1) + (cpu->b & 0x80);
}
Z80OPCODE opl_29(Z80 *cpu) { // sra c
   cpu->f = sraf[cpu->c], cpu->c = (cpu->c >> 1) + (cpu->c & 0x80);
}
Z80OPCODE opl_2A(Z80 *cpu) { // sra d
   cpu->f = sraf[cpu->d], cpu->d = (cpu->d >> 1) + (cpu->d & 0x80);
}
Z80OPCODE opl_2B(Z80 *cpu) { // sra e
   cpu->f = sraf[cpu->e], cpu->e = (cpu->e >> 1) + (cpu->e & 0x80);
}
Z80OPCODE opl_2C(Z80 *cpu) { // sra h
   cpu->f = sraf[cpu->h], cpu->h = (cpu->h >> 1) + (cpu->h & 0x80);
}
Z80OPCODE opl_2D(Z80 *cpu) { // sra l
   cpu->f = sraf[cpu->l], cpu->l = (cpu->l >> 1) + (cpu->l & 0x80);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_2E(Z80 *cpu) { // sra (hl)
   unsigned char t = rm(cpu->hl);
   cpu->f = sraf[t], t = (t >> 1) + (t & 0x80);
   wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_2F(Z80 *cpu) { // sra a
   cpu->f = sraf[cpu->a], cpu->a = (cpu->a >> 1) + (cpu->a & 0x80);
}
Z80OPCODE opl_30(Z80 *cpu) { // sli b
   cpu->f = rl1[cpu->b], cpu->b = (cpu->b << 1) + 1;
}
Z80OPCODE opl_31(Z80 *cpu) { // sli c
   cpu->f = rl1[cpu->c], cpu->c = (cpu->c << 1) + 1;
}
Z80OPCODE opl_32(Z80 *cpu) { // sli d
   cpu->f = rl1[cpu->d], cpu->d = (cpu->d << 1) + 1;
}
Z80OPCODE opl_33(Z80 *cpu) { // sli e
   cpu->f = rl1[cpu->e], cpu->e = (cpu->e << 1) + 1;
}
Z80OPCODE opl_34(Z80 *cpu) { // sli h
   cpu->f = rl1[cpu->h], cpu->h = (cpu->h << 1) + 1;
}
Z80OPCODE opl_35(Z80 *cpu) { // sli l
   cpu->f = rl1[cpu->l], cpu->l = (cpu->l << 1) + 1;
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_36(Z80 *cpu) { // sli (hl)
   unsigned char t = rm(cpu->hl);
   cpu->f = rl1[t], t = (t << 1) + 1;
   wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_37(Z80 *cpu) { // sli a
   cpu->f = rl1[cpu->a], cpu->a = (cpu->a << 1) + 1;
}
Z80OPCODE opl_38(Z80 *cpu) { // srl b
   cpu->f = rr0[cpu->b], cpu->b = (cpu->b >> 1);
}
Z80OPCODE opl_39(Z80 *cpu) { // srl c
   cpu->f = rr0[cpu->c], cpu->c = (cpu->c >> 1);
}
Z80OPCODE opl_3A(Z80 *cpu) { // srl d
   cpu->f = rr0[cpu->d], cpu->d = (cpu->d >> 1);
}
Z80OPCODE opl_3B(Z80 *cpu) { // srl e
   cpu->f = rr0[cpu->e], cpu->e = (cpu->e >> 1);
}
Z80OPCODE opl_3C(Z80 *cpu) { // srl h
   cpu->f = rr0[cpu->h], cpu->h = (cpu->h >> 1);
}
Z80OPCODE opl_3D(Z80 *cpu) { // srl l
   cpu->f = rr0[cpu->l], cpu->l = (cpu->l >> 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_3E(Z80 *cpu) { // srl (hl)
   unsigned char t = rm(cpu->hl);
   cpu->f = rr0[t], t = (t >> 1);
   wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_3F(Z80 *cpu) { // srl a
   cpu->f = rr0[cpu->a], cpu->a = (cpu->a >> 1);
}
Z80OPCODE opl_40(Z80 *cpu) { // bit 0,b
   bit(cpu, cpu->b, 0);
}
Z80OPCODE opl_41(Z80 *cpu) { // bit 0,c
   bit(cpu, cpu->c, 0);
}
Z80OPCODE opl_42(Z80 *cpu) { // bit 0,d
   bit(cpu, cpu->d, 0);
}
Z80OPCODE opl_43(Z80 *cpu) { // bit 0,e
   bit(cpu, cpu->e, 0);
}
Z80OPCODE opl_44(Z80 *cpu) { // bit 0,h
   bit(cpu, cpu->h, 0);
}
Z80OPCODE opl_45(Z80 *cpu) { // bit 0,l
   bit(cpu, cpu->l, 0);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_46(Z80 *cpu) { // bit 0,(hl)
   bit(cpu, rm(cpu->hl), 0);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_47(Z80 *cpu) { // bit 0,a
   bit(cpu, cpu->a, 0);
}
Z80OPCODE opl_48(Z80 *cpu) { // bit 1,b
   bit(cpu, cpu->b, 1);
}
Z80OPCODE opl_49(Z80 *cpu) { // bit 1,c
   bit(cpu, cpu->c, 1);
}
Z80OPCODE opl_4A(Z80 *cpu) { // bit 1,d
   bit(cpu, cpu->d, 1);
}
Z80OPCODE opl_4B(Z80 *cpu) { // bit 1,e
   bit(cpu, cpu->e, 1);
}
Z80OPCODE opl_4C(Z80 *cpu) { // bit 1,h
   bit(cpu, cpu->h, 1);
}
Z80OPCODE opl_4D(Z80 *cpu) { // bit 1,l
   bit(cpu, cpu->l, 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_4E(Z80 *cpu) { // bit 1,(hl)
   bit(cpu, rm(cpu->hl), 1);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_4F(Z80 *cpu) { // bit 1,a
   bit(cpu, cpu->a, 1);
}
Z80OPCODE opl_50(Z80 *cpu) { // bit 2,b
   bit(cpu, cpu->b, 2);
}
Z80OPCODE opl_51(Z80 *cpu) { // bit 2,c
   bit(cpu, cpu->c, 2);
}
Z80OPCODE opl_52(Z80 *cpu) { // bit 2,d
   bit(cpu, cpu->d, 2);
}
Z80OPCODE opl_53(Z80 *cpu) { // bit 2,e
   bit(cpu, cpu->e, 2);
}
Z80OPCODE opl_54(Z80 *cpu) { // bit 2,h
   bit(cpu, cpu->h, 2);
}
Z80OPCODE opl_55(Z80 *cpu) { // bit 2,l
   bit(cpu, cpu->l, 2);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_56(Z80 *cpu) { // bit 2,(hl)
   bit(cpu, rm(cpu->hl), 2);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_57(Z80 *cpu) { // bit 2,a
   bit(cpu, cpu->a, 2);
}
Z80OPCODE opl_58(Z80 *cpu) { // bit 3,b
   bit(cpu, cpu->b, 3);
}
Z80OPCODE opl_59(Z80 *cpu) { // bit 3,c
   bit(cpu, cpu->c, 3);
}
Z80OPCODE opl_5A(Z80 *cpu) { // bit 3,d
   bit(cpu, cpu->d, 3);
}
Z80OPCODE opl_5B(Z80 *cpu) { // bit 3,e
   bit(cpu, cpu->e, 3);
}
Z80OPCODE opl_5C(Z80 *cpu) { // bit 3,h
   bit(cpu, cpu->h, 3);
}
Z80OPCODE opl_5D(Z80 *cpu) { // bit 3,l
   bit(cpu, cpu->l, 3);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_5E(Z80 *cpu) { // bit 3,(hl)
   bit(cpu, rm(cpu->hl), 3);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_5F(Z80 *cpu) { // bit 3,a
   bit(cpu, cpu->a, 3);
}
Z80OPCODE opl_60(Z80 *cpu) { // bit 4,b
   bit(cpu, cpu->b, 4);
}
Z80OPCODE opl_61(Z80 *cpu) { // bit 4,c
   bit(cpu, cpu->c, 4);
}
Z80OPCODE opl_62(Z80 *cpu) { // bit 4,d
   bit(cpu, cpu->d, 4);
}
Z80OPCODE opl_63(Z80 *cpu) { // bit 4,e
   bit(cpu, cpu->e, 4);
}
Z80OPCODE opl_64(Z80 *cpu) { // bit 4,h
   bit(cpu, cpu->h, 4);
}
Z80OPCODE opl_65(Z80 *cpu) { // bit 4,l
   bit(cpu, cpu->l, 4);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_66(Z80 *cpu) { // bit 4,(hl)
   bit(cpu, rm(cpu->hl), 4);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_67(Z80 *cpu) { // bit 4,a
   bit(cpu, cpu->a, 4);
}
Z80OPCODE opl_68(Z80 *cpu) { // bit 5,b
   bit(cpu, cpu->b, 5);
}
Z80OPCODE opl_69(Z80 *cpu) { // bit 5,c
   bit(cpu, cpu->c, 5);
}
Z80OPCODE opl_6A(Z80 *cpu) { // bit 5,d
   bit(cpu, cpu->d, 5);
}
Z80OPCODE opl_6B(Z80 *cpu) { // bit 5,e
   bit(cpu, cpu->e, 5);
}
Z80OPCODE opl_6C(Z80 *cpu) { // bit 5,h
   bit(cpu, cpu->h, 5);
}
Z80OPCODE opl_6D(Z80 *cpu) { // bit 5,l
   bit(cpu, cpu->l, 5);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_6E(Z80 *cpu) { // bit 5,(hl)
   bit(cpu, rm(cpu->hl), 5);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_6F(Z80 *cpu) { // bit 5,a
   bit(cpu, cpu->a, 5);
}
Z80OPCODE opl_70(Z80 *cpu) { // bit 6,b
   bit(cpu, cpu->b, 6);
}
Z80OPCODE opl_71(Z80 *cpu) { // bit 6,c
   bit(cpu, cpu->c, 6);
}
Z80OPCODE opl_72(Z80 *cpu) { // bit 6,d
   bit(cpu, cpu->d, 6);
}
Z80OPCODE opl_73(Z80 *cpu) { // bit 6,e
   bit(cpu, cpu->e, 6);
}
Z80OPCODE opl_74(Z80 *cpu) { // bit 6,h
   bit(cpu, cpu->h, 6);
}
Z80OPCODE opl_75(Z80 *cpu) { // bit 6,l
   bit(cpu, cpu->l, 6);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_76(Z80 *cpu) { // bit 6,(hl)
   bit(cpu, rm(cpu->hl), 6);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_77(Z80 *cpu) { // bit 6,a
   bit(cpu, cpu->a, 6);
}
Z80OPCODE opl_78(Z80 *cpu) { // bit 7,b
   bit(cpu, cpu->b, 7);
}
Z80OPCODE opl_79(Z80 *cpu) { // bit 7,c
   bit(cpu, cpu->c, 7);
}
Z80OPCODE opl_7A(Z80 *cpu) { // bit 7,d
   bit(cpu, cpu->d, 7);
}
Z80OPCODE opl_7B(Z80 *cpu) { // bit 7,e
   bit(cpu, cpu->e, 7);
}
Z80OPCODE opl_7C(Z80 *cpu) { // bit 7,h
   bit(cpu, cpu->h, 7);
}
Z80OPCODE opl_7D(Z80 *cpu) { // bit 7,l
   bit(cpu, cpu->l, 7);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_7E(Z80 *cpu) { // bit 7,(hl)
   bit(cpu, rm(cpu->hl), 7);
   cpu->t += 4;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_7F(Z80 *cpu) { // bit 7,a
   bit(cpu, cpu->a, 7);
}
Z80OPCODE opl_80(Z80 *cpu) { // res 0,b
   res(cpu->b, 0);
}
Z80OPCODE opl_81(Z80 *cpu) { // res 0,c
   res(cpu->c, 0);
}
Z80OPCODE opl_82(Z80 *cpu) { // res 0,d
   res(cpu->d, 0);
}
Z80OPCODE opl_83(Z80 *cpu) { // res 0,e
   res(cpu->e, 0);
}
Z80OPCODE opl_84(Z80 *cpu) { // res 0,h
   res(cpu->h, 0);
}
Z80OPCODE opl_85(Z80 *cpu) { // res 0,l
   res(cpu->l, 0);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_86(Z80 *cpu) { // res 0,(hl)
   unsigned char t = rm(cpu->hl); res(t, 0); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_87(Z80 *cpu) { // res 0,a
   res(cpu->a, 0);
}
Z80OPCODE opl_88(Z80 *cpu) { // res 1,b
   res(cpu->b, 1);
}
Z80OPCODE opl_89(Z80 *cpu) { // res 1,c
   res(cpu->c, 1);
}
Z80OPCODE opl_8A(Z80 *cpu) { // res 1,d
   res(cpu->d, 1);
}
Z80OPCODE opl_8B(Z80 *cpu) { // res 1,e
   res(cpu->e, 1);
}
Z80OPCODE opl_8C(Z80 *cpu) { // res 1,h
   res(cpu->h, 1);
}
Z80OPCODE opl_8D(Z80 *cpu) { // res 1,l
   res(cpu->l, 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_8E(Z80 *cpu) { // res 1,(hl)
   unsigned char t = rm(cpu->hl); res(t, 1); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_8F(Z80 *cpu) { // res 1,a
   res(cpu->a, 1);
}
Z80OPCODE opl_90(Z80 *cpu) { // res 2,b
   res(cpu->b, 2);
}
Z80OPCODE opl_91(Z80 *cpu) { // res 2,c
   res(cpu->c, 2);
}
Z80OPCODE opl_92(Z80 *cpu) { // res 2,d
   res(cpu->d, 2);
}
Z80OPCODE opl_93(Z80 *cpu) { // res 2,e
   res(cpu->e, 2);
}
Z80OPCODE opl_94(Z80 *cpu) { // res 2,h
   res(cpu->h, 2);
}
Z80OPCODE opl_95(Z80 *cpu) { // res 2,l
   res(cpu->l, 2);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_96(Z80 *cpu) { // res 2,(hl)
   unsigned char t = rm(cpu->hl); res(t, 2); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_97(Z80 *cpu) { // res 2,a
   res(cpu->a, 2);
}
Z80OPCODE opl_98(Z80 *cpu) { // res 3,b
   res(cpu->b, 3);
}
Z80OPCODE opl_99(Z80 *cpu) { // res 3,c
   res(cpu->c, 3);
}
Z80OPCODE opl_9A(Z80 *cpu) { // res 3,d
   res(cpu->d, 3);
}
Z80OPCODE opl_9B(Z80 *cpu) { // res 3,e
   res(cpu->e, 3);
}
Z80OPCODE opl_9C(Z80 *cpu) { // res 3,h
   res(cpu->h, 3);
}
Z80OPCODE opl_9D(Z80 *cpu) { // res 3,l
   res(cpu->l, 3);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_9E(Z80 *cpu) { // res 3,(hl)
   unsigned char t = rm(cpu->hl); res(t, 3); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_9F(Z80 *cpu) { // res 3,a
   res(cpu->a, 3);
}
Z80OPCODE opl_A0(Z80 *cpu) { // res 4,b
   res(cpu->b, 4);
}
Z80OPCODE opl_A1(Z80 *cpu) { // res 4,c
   res(cpu->c, 4);
}
Z80OPCODE opl_A2(Z80 *cpu) { // res 4,d
   res(cpu->d, 4);
}
Z80OPCODE opl_A3(Z80 *cpu) { // res 4,e
   res(cpu->e, 4);
}
Z80OPCODE opl_A4(Z80 *cpu) { // res 4,h
   res(cpu->h, 4);
}
Z80OPCODE opl_A5(Z80 *cpu) { // res 4,l
   res(cpu->l, 4);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_A6(Z80 *cpu) { // res 4,(hl)
   unsigned char t = rm(cpu->hl); res(t, 4); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_A7(Z80 *cpu) { // res 4,a
   res(cpu->a, 4);
}
Z80OPCODE opl_A8(Z80 *cpu) { // res 5,b
   res(cpu->b, 5);
}
Z80OPCODE opl_A9(Z80 *cpu) { // res 5,c
   res(cpu->c, 5);
}
Z80OPCODE opl_AA(Z80 *cpu) { // res 5,d
   res(cpu->d, 5);
}
Z80OPCODE opl_AB(Z80 *cpu) { // res 5,e
   res(cpu->e, 5);
}
Z80OPCODE opl_AC(Z80 *cpu) { // res 5,h
   res(cpu->h, 5);
}
Z80OPCODE opl_AD(Z80 *cpu) { // res 5,l
   res(cpu->l, 5);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_AE(Z80 *cpu) { // res 5,(hl)
   unsigned char t = rm(cpu->hl); res(t, 5); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_AF(Z80 *cpu) { // res 5,a
   res(cpu->a, 5);
}
Z80OPCODE opl_B0(Z80 *cpu) { // res 6,b
   res(cpu->b, 6);
}
Z80OPCODE opl_B1(Z80 *cpu) { // res 6,c
   res(cpu->c, 6);
}
Z80OPCODE opl_B2(Z80 *cpu) { // res 6,d
   res(cpu->d, 6);
}
Z80OPCODE opl_B3(Z80 *cpu) { // res 6,e
   res(cpu->e, 6);
}
Z80OPCODE opl_B4(Z80 *cpu) { // res 6,h
   res(cpu->h, 6);
}
Z80OPCODE opl_B5(Z80 *cpu) { // res 6,l
   res(cpu->l, 6);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_B6(Z80 *cpu) { // res 6,(hl)
   unsigned char t = rm(cpu->hl); res(t, 6); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_B7(Z80 *cpu) { // res 6,a
   res(cpu->a, 6);
}
Z80OPCODE opl_B8(Z80 *cpu) { // res 7,b
   res(cpu->b, 7);
}
Z80OPCODE opl_B9(Z80 *cpu) { // res 7,c
   res(cpu->c, 7);
}
Z80OPCODE opl_BA(Z80 *cpu) { // res 7,d
   res(cpu->d, 7);
}
Z80OPCODE opl_BB(Z80 *cpu) { // res 7,e
   res(cpu->e, 7);
}
Z80OPCODE opl_BC(Z80 *cpu) { // res 7,h
   res(cpu->h, 7);
}
Z80OPCODE opl_BD(Z80 *cpu) { // res 7,l
   res(cpu->l, 7);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_BE(Z80 *cpu) { // res 7,(hl)
   unsigned char t = rm(cpu->hl); res(t, 7); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_BF(Z80 *cpu) { // res 7,a
   res(cpu->a, 7);
}
Z80OPCODE opl_C0(Z80 *cpu) { // set 0,b
   set(cpu->b, 0);
}
Z80OPCODE opl_C1(Z80 *cpu) { // set 0,c
   set(cpu->c, 0);
}
Z80OPCODE opl_C2(Z80 *cpu) { // set 0,d
   set(cpu->d, 0);
}
Z80OPCODE opl_C3(Z80 *cpu) { // set 0,e
   set(cpu->e, 0);
}
Z80OPCODE opl_C4(Z80 *cpu) { // set 0,h
   set(cpu->h, 0);
}
Z80OPCODE opl_C5(Z80 *cpu) { // set 0,l
   set(cpu->l, 0);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_C6(Z80 *cpu) { // set 0,(hl)
   unsigned char t = rm(cpu->hl); set(t, 0); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_C7(Z80 *cpu) { // set 0,a
   set(cpu->a, 0);
}
Z80OPCODE opl_C8(Z80 *cpu) { // set 1,b
   set(cpu->b, 1);
}
Z80OPCODE opl_C9(Z80 *cpu) { // set 1,c
   set(cpu->c, 1);
}
Z80OPCODE opl_CA(Z80 *cpu) { // set 1,d
   set(cpu->d, 1);
}
Z80OPCODE opl_CB(Z80 *cpu) { // set 1,e
   set(cpu->e, 1);
}
Z80OPCODE opl_CC(Z80 *cpu) { // set 1,h
   set(cpu->h, 1);
}
Z80OPCODE opl_CD(Z80 *cpu) { // set 1,l
   set(cpu->l, 1);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_CE(Z80 *cpu) { // set 1,(hl)
   unsigned char t = rm(cpu->hl); set(t, 1); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_CF(Z80 *cpu) { // set 1,a
   set(cpu->a, 1);
}
Z80OPCODE opl_D0(Z80 *cpu) { // set 2,b
   set(cpu->b, 2);
}
Z80OPCODE opl_D1(Z80 *cpu) { // set 2,c
   set(cpu->c, 2);
}
Z80OPCODE opl_D2(Z80 *cpu) { // set 2,d
   set(cpu->d, 2);
}
Z80OPCODE opl_D3(Z80 *cpu) { // set 2,e
   set(cpu->e, 2);
}
Z80OPCODE opl_D4(Z80 *cpu) { // set 2,h
   set(cpu->h, 2);
}
Z80OPCODE opl_D5(Z80 *cpu) { // set 2,l
   set(cpu->l, 2);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_D6(Z80 *cpu) { // set 2,(hl)
   unsigned char t = rm(cpu->hl); set(t, 2); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_D7(Z80 *cpu) { // set 2,a
   set(cpu->a, 2);
}
Z80OPCODE opl_D8(Z80 *cpu) { // set 3,b
   set(cpu->b, 3);
}
Z80OPCODE opl_D9(Z80 *cpu) { // set 3,c
   set(cpu->c, 3);
}
Z80OPCODE opl_DA(Z80 *cpu) { // set 3,d
   set(cpu->d, 3);
}
Z80OPCODE opl_DB(Z80 *cpu) { // set 3,e
   set(cpu->e, 3);
}
Z80OPCODE opl_DC(Z80 *cpu) { // set 3,h
   set(cpu->h, 3);
}
Z80OPCODE opl_DD(Z80 *cpu) { // set 3,l
   set(cpu->l, 3);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_DE(Z80 *cpu) { // set 3,(hl)
   unsigned char t = rm(cpu->hl); set(t, 3); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_DF(Z80 *cpu) { // set 3,a
   set(cpu->a, 3);
}
Z80OPCODE opl_E0(Z80 *cpu) { // set 4,b
   set(cpu->b, 4);
}
Z80OPCODE opl_E1(Z80 *cpu) { // set 4,c
   set(cpu->c, 4);
}
Z80OPCODE opl_E2(Z80 *cpu) { // set 4,d
   set(cpu->d, 4);
}
Z80OPCODE opl_E3(Z80 *cpu) { // set 4,e
   set(cpu->e, 4);
}
Z80OPCODE opl_E4(Z80 *cpu) { // set 4,h
   set(cpu->h, 4);
}
Z80OPCODE opl_E5(Z80 *cpu) { // set 4,l
   set(cpu->l, 4);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_E6(Z80 *cpu) { // set 4,(hl)
   unsigned char t = rm(cpu->hl); set(t, 4); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_E7(Z80 *cpu) { // set 4,a
   set(cpu->a, 4);
}
Z80OPCODE opl_E8(Z80 *cpu) { // set 5,b
   set(cpu->b, 5);
}
Z80OPCODE opl_E9(Z80 *cpu) { // set 5,c
   set(cpu->c, 5);
}
Z80OPCODE opl_EA(Z80 *cpu) { // set 5,d
   set(cpu->d, 5);
}
Z80OPCODE opl_EB(Z80 *cpu) { // set 5,e
   set(cpu->e, 5);
}
Z80OPCODE opl_EC(Z80 *cpu) { // set 5,h
   set(cpu->h, 5);
}
Z80OPCODE opl_ED(Z80 *cpu) { // set 5,l
   set(cpu->l, 5);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_EE(Z80 *cpu) { // set 5,(hl)
   unsigned char t = rm(cpu->hl); set(t, 5); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_EF(Z80 *cpu) { // set 5,a
   set(cpu->a, 5);
}
Z80OPCODE opl_F0(Z80 *cpu) { // set 6,b
   set(cpu->b, 6);
}
Z80OPCODE opl_F1(Z80 *cpu) { // set 6,c
   set(cpu->c, 6);
}
Z80OPCODE opl_F2(Z80 *cpu) { // set 6,d
   set(cpu->d, 6);
}
Z80OPCODE opl_F3(Z80 *cpu) { // set 6,e
   set(cpu->e, 6);
}
Z80OPCODE opl_F4(Z80 *cpu) { // set 6,h
   set(cpu->h, 6);
}
Z80OPCODE opl_F5(Z80 *cpu) { // set 6,l
   set(cpu->l, 6);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_F6(Z80 *cpu) { // set 6,(hl)
   unsigned char t = rm(cpu->hl); set(t, 6); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_F7(Z80 *cpu) { // set 6,a
   set(cpu->a, 6);
}
Z80OPCODE opl_F8(Z80 *cpu) { // set 7,b
   set(cpu->b, 7);
}
Z80OPCODE opl_F9(Z80 *cpu) { // set 7,c
   set(cpu->c, 7);
}
Z80OPCODE opl_FA(Z80 *cpu) { // set 7,d
   set(cpu->d, 7);
}
Z80OPCODE opl_FB(Z80 *cpu) { // set 7,e
   set(cpu->e, 7);
}
Z80OPCODE opl_FC(Z80 *cpu) { // set 7,h
   set(cpu->h, 7);
}
Z80OPCODE opl_FD(Z80 *cpu) { // set 7,l
   set(cpu->l, 7);
}
#endif
#ifndef Z80_COMMON
Z80OPCODE opl_FE(Z80 *cpu) { // set 7,(hl)
   unsigned char t = rm(cpu->hl); set(t, 7); wm(cpu->hl, t);
   cpu->t += 7;
}
#endif
#ifdef Z80_COMMON
Z80OPCODE opl_FF(Z80 *cpu) { // set 7,a
   set(cpu->a, 7);
}
#endif
#ifndef Z80_COMMON

STEPFUNC const logic_opcode[0x100] = {

   opl_00, opl_01, opl_02, opl_03, opl_04, opl_05, opl_06, opl_07,
   opl_08, opl_09, opl_0A, opl_0B, opl_0C, opl_0D, opl_0E, opl_0F,
   opl_10, opl_11, opl_12, opl_13, opl_14, opl_15, opl_16, opl_17,
   opl_18, opl_19, opl_1A, opl_1B, opl_1C, opl_1D, opl_1E, opl_1F,
   opl_20, opl_21, opl_22, opl_23, opl_24, opl_25, opl_26, opl_27,
   opl_28, opl_29, opl_2A, opl_2B, opl_2C, opl_2D, opl_2E, opl_2F,
   opl_30, opl_31, opl_32, opl_33, opl_34, opl_35, opl_36, opl_37,
   opl_38, opl_39, opl_3A, opl_3B, opl_3C, opl_3D, opl_3E, opl_3F,

   opl_40, opl_41, opl_42, opl_43, opl_44, opl_45, opl_46, opl_47,
   opl_48, opl_49, opl_4A, opl_4B, opl_4C, opl_4D, opl_4E, opl_4F,
   opl_50, opl_51, opl_52, opl_53, opl_54, opl_55, opl_56, opl_57,
   opl_58, opl_59, opl_5A, opl_5B, opl_5C, opl_5D, opl_5E, opl_5F,
   opl_60, opl_61, opl_62, opl_63, opl_64, opl_65, opl_66, opl_67,
   opl_68, opl_69, opl_6A, opl_6B, opl_6C, opl_6D, opl_6E, opl_6F,
   opl_70, opl_71, opl_72, opl_73, opl_74, opl_75, opl_76, opl_77,
   opl_78, opl_79, opl_7A, opl_7B, opl_7C, opl_7D, opl_7E, opl_7F,

   opl_80, opl_81, opl_82, opl_83, opl_84, opl_85, opl_86, opl_87,
   opl_88, opl_89, opl_8A, opl_8B, opl_8C, opl_8D, opl_8E, opl_8F,
   opl_90, opl_91, opl_92, opl_93, opl_94, opl_95, opl_96, opl_97,
   opl_98, opl_99, opl_9A, opl_9B, opl_9C, opl_9D, opl_9E, opl_9F,
   opl_A0, opl_A1, opl_A2, opl_A3, opl_A4, opl_A5, opl_A6, opl_A7,
   opl_A8, opl_A9, opl_AA, opl_AB, opl_AC, opl_AD, opl_AE, opl_AF,
   opl_B0, opl_B1, opl_B2, opl_B3, opl_B4, opl_B5, opl_B6, opl_B7,
   opl_B8, opl_B9, opl_BA, opl_BB, opl_BC, opl_BD, opl_BE, opl_BF,

   opl_C0, opl_C1, opl_C2, opl_C3, opl_C4, opl_C5, opl_C6, opl_C7,
   opl_C8, opl_C9, opl_CA, opl_CB, opl_CC, opl_CD, opl_CE, opl_CF,
   opl_D0, opl_D1, opl_D2, opl_D3, opl_D4, opl_D5, opl_D6, opl_D7,
   opl_D8, opl_D9, opl_DA, opl_DB, opl_DC, opl_DD, opl_DE, opl_DF,
   opl_E0, opl_E1, opl_E2, opl_E3, opl_E4, opl_E5, opl_E6, opl_E7,
   opl_E8, opl_E9, opl_EA, opl_EB, opl_EC, opl_ED, opl_EE, opl_EF,
   opl_F0, opl_F1, opl_F2, opl_F3, opl_F4, opl_F5, opl_F6, opl_F7,
   opl_F8, opl_F9, opl_FA, opl_FB, opl_FC, opl_FD, opl_FE, opl_FF,

};

Z80OPCODE op_CB(Z80 *cpu)
{
   unsigned char opcode = m1_cycle(cpu);
   (logic_opcode[opcode])(cpu);
}
#endif