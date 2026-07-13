.include "macros.inc"
.file "runtime.c"

# 0x800EE178..0x800EE854 | size: 0x6DC
.text
.balign 4

# .text:0x0 | 0x800EE178 | size: 0x5C
.fn __cvt_fp2unsigned, global
/* 800EE178 000EAF18  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800EE17C 000EAF1C  3C 80 80 21 */	lis r4, __constants@h
/* 800EE180 000EAF20  60 84 66 E8 */	ori r4, r4, __constants@l
/* 800EE184 000EAF24  38 60 00 00 */	li r3, 0x0
/* 800EE188 000EAF28  C8 04 00 00 */	lfd f0, 0x0(r4)
/* 800EE18C 000EAF2C  C8 64 00 08 */	lfd f3, 0x8(r4)
/* 800EE190 000EAF30  C8 84 00 10 */	lfd f4, 0x10(r4)
/* 800EE194 000EAF34  FC 01 00 00 */	fcmpu cr0, f1, f0
/* 800EE198 000EAF38  FF 01 18 00 */	fcmpu cr6, f1, f3
/* 800EE19C 000EAF3C  41 80 00 30 */	blt .L_800EE1CC
/* 800EE1A0 000EAF40  38 63 FF FF */	subi r3, r3, 0x1
/* 800EE1A4 000EAF44  40 98 00 28 */	bge cr6, .L_800EE1CC
/* 800EE1A8 000EAF48  FF 81 20 00 */	fcmpu cr7, f1, f4
/* 800EE1AC 000EAF4C  FC 40 08 90 */	fmr f2, f1
/* 800EE1B0 000EAF50  41 9C 00 08 */	blt cr7, .L_800EE1B8
/* 800EE1B4 000EAF54  FC 41 20 28 */	fsub f2, f1, f4
.L_800EE1B8:
/* 800EE1B8 000EAF58  FC 40 10 1E */	fctiwz f2, f2
/* 800EE1BC 000EAF5C  D8 41 00 08 */	stfd f2, 0x8(r1)
/* 800EE1C0 000EAF60  80 61 00 0C */	lwz r3, 0xc(r1)
/* 800EE1C4 000EAF64  41 9C 00 08 */	blt cr7, .L_800EE1CC
/* 800EE1C8 000EAF68  3C 63 80 00 */	addis r3, r3, 0x8000
.L_800EE1CC:
/* 800EE1CC 000EAF6C  38 21 00 10 */	addi r1, r1, 0x10
/* 800EE1D0 000EAF70  4E 80 00 20 */	blr
.endfn __cvt_fp2unsigned

# .text:0x5C | 0x800EE1D4 | size: 0x4C
.fn __save_fpr, global
# .text:0x5C | 0x800EE1D4 | size: 0x0
.sym _savefpr_14, global
/* 800EE1D4 000EAF74  D9 CB FF 70 */	stfd f14, -0x90(r11)
# .text:0x60 | 0x800EE1D8 | size: 0x0
.sym _savefpr_15, global
/* 800EE1D8 000EAF78  D9 EB FF 78 */	stfd f15, -0x88(r11)
# .text:0x64 | 0x800EE1DC | size: 0x0
.sym _savefpr_16, global
/* 800EE1DC 000EAF7C  DA 0B FF 80 */	stfd f16, -0x80(r11)
# .text:0x68 | 0x800EE1E0 | size: 0x0
.sym _savefpr_17, global
/* 800EE1E0 000EAF80  DA 2B FF 88 */	stfd f17, -0x78(r11)
# .text:0x6C | 0x800EE1E4 | size: 0x0
.sym _savefpr_18, global
/* 800EE1E4 000EAF84  DA 4B FF 90 */	stfd f18, -0x70(r11)
# .text:0x70 | 0x800EE1E8 | size: 0x0
.sym _savefpr_19, global
/* 800EE1E8 000EAF88  DA 6B FF 98 */	stfd f19, -0x68(r11)
# .text:0x74 | 0x800EE1EC | size: 0x0
.sym _savefpr_20, global
/* 800EE1EC 000EAF8C  DA 8B FF A0 */	stfd f20, -0x60(r11)
# .text:0x78 | 0x800EE1F0 | size: 0x0
.sym _savefpr_21, global
/* 800EE1F0 000EAF90  DA AB FF A8 */	stfd f21, -0x58(r11)
# .text:0x7C | 0x800EE1F4 | size: 0x0
.sym _savefpr_22, global
/* 800EE1F4 000EAF94  DA CB FF B0 */	stfd f22, -0x50(r11)
# .text:0x80 | 0x800EE1F8 | size: 0x0
.sym _savefpr_23, global
/* 800EE1F8 000EAF98  DA EB FF B8 */	stfd f23, -0x48(r11)
# .text:0x84 | 0x800EE1FC | size: 0x0
.sym _savefpr_24, global
/* 800EE1FC 000EAF9C  DB 0B FF C0 */	stfd f24, -0x40(r11)
# .text:0x88 | 0x800EE200 | size: 0x0
.sym _savefpr_25, global
/* 800EE200 000EAFA0  DB 2B FF C8 */	stfd f25, -0x38(r11)
# .text:0x8C | 0x800EE204 | size: 0x0
.sym _savefpr_26, global
/* 800EE204 000EAFA4  DB 4B FF D0 */	stfd f26, -0x30(r11)
# .text:0x90 | 0x800EE208 | size: 0x0
.sym _savefpr_27, global
/* 800EE208 000EAFA8  DB 6B FF D8 */	stfd f27, -0x28(r11)
# .text:0x94 | 0x800EE20C | size: 0x0
.sym _savefpr_28, global
/* 800EE20C 000EAFAC  DB 8B FF E0 */	stfd f28, -0x20(r11)
# .text:0x98 | 0x800EE210 | size: 0x0
.sym _savefpr_29, global
/* 800EE210 000EAFB0  DB AB FF E8 */	stfd f29, -0x18(r11)
# .text:0x9C | 0x800EE214 | size: 0x0
.sym _savefpr_30, global
/* 800EE214 000EAFB4  DB CB FF F0 */	stfd f30, -0x10(r11)
# .text:0xA0 | 0x800EE218 | size: 0x0
.sym _savefpr_31, global
/* 800EE218 000EAFB8  DB EB FF F8 */	stfd f31, -0x8(r11)
/* 800EE21C 000EAFBC  4E 80 00 20 */	blr
.endfn __save_fpr

# .text:0xA8 | 0x800EE220 | size: 0x4C
.fn __restore_fpr, global
# .text:0xA8 | 0x800EE220 | size: 0x0
.sym _restfpr_14, global
/* 800EE220 000EAFC0  C9 CB FF 70 */	lfd f14, -0x90(r11)
# .text:0xAC | 0x800EE224 | size: 0x0
.sym _restfpr_15, global
/* 800EE224 000EAFC4  C9 EB FF 78 */	lfd f15, -0x88(r11)
# .text:0xB0 | 0x800EE228 | size: 0x0
.sym _restfpr_16, global
/* 800EE228 000EAFC8  CA 0B FF 80 */	lfd f16, -0x80(r11)
# .text:0xB4 | 0x800EE22C | size: 0x0
.sym _restfpr_17, global
/* 800EE22C 000EAFCC  CA 2B FF 88 */	lfd f17, -0x78(r11)
# .text:0xB8 | 0x800EE230 | size: 0x0
.sym _restfpr_18, global
/* 800EE230 000EAFD0  CA 4B FF 90 */	lfd f18, -0x70(r11)
# .text:0xBC | 0x800EE234 | size: 0x0
.sym _restfpr_19, global
/* 800EE234 000EAFD4  CA 6B FF 98 */	lfd f19, -0x68(r11)
# .text:0xC0 | 0x800EE238 | size: 0x0
.sym _restfpr_20, global
/* 800EE238 000EAFD8  CA 8B FF A0 */	lfd f20, -0x60(r11)
# .text:0xC4 | 0x800EE23C | size: 0x0
.sym _restfpr_21, global
/* 800EE23C 000EAFDC  CA AB FF A8 */	lfd f21, -0x58(r11)
# .text:0xC8 | 0x800EE240 | size: 0x0
.sym _restfpr_22, global
/* 800EE240 000EAFE0  CA CB FF B0 */	lfd f22, -0x50(r11)
# .text:0xCC | 0x800EE244 | size: 0x0
.sym _restfpr_23, global
/* 800EE244 000EAFE4  CA EB FF B8 */	lfd f23, -0x48(r11)
# .text:0xD0 | 0x800EE248 | size: 0x0
.sym _restfpr_24, global
/* 800EE248 000EAFE8  CB 0B FF C0 */	lfd f24, -0x40(r11)
# .text:0xD4 | 0x800EE24C | size: 0x0
.sym _restfpr_25, global
/* 800EE24C 000EAFEC  CB 2B FF C8 */	lfd f25, -0x38(r11)
# .text:0xD8 | 0x800EE250 | size: 0x0
.sym _restfpr_26, global
/* 800EE250 000EAFF0  CB 4B FF D0 */	lfd f26, -0x30(r11)
# .text:0xDC | 0x800EE254 | size: 0x0
.sym _restfpr_27, global
/* 800EE254 000EAFF4  CB 6B FF D8 */	lfd f27, -0x28(r11)
# .text:0xE0 | 0x800EE258 | size: 0x0
.sym _restfpr_28, global
/* 800EE258 000EAFF8  CB 8B FF E0 */	lfd f28, -0x20(r11)
# .text:0xE4 | 0x800EE25C | size: 0x0
.sym _restfpr_29, global
/* 800EE25C 000EAFFC  CB AB FF E8 */	lfd f29, -0x18(r11)
# .text:0xE8 | 0x800EE260 | size: 0x0
.sym _restfpr_30, global
/* 800EE260 000EB000  CB CB FF F0 */	lfd f30, -0x10(r11)
# .text:0xEC | 0x800EE264 | size: 0x0
.sym _restfpr_31, global
/* 800EE264 000EB004  CB EB FF F8 */	lfd f31, -0x8(r11)
/* 800EE268 000EB008  4E 80 00 20 */	blr
.endfn __restore_fpr

# .text:0xF4 | 0x800EE26C | size: 0x4C
.fn __save_gpr, global
# .text:0xF4 | 0x800EE26C | size: 0x0
.sym _savegpr_14, global
/* 800EE26C 000EB00C  91 CB FF B8 */	stw r14, -0x48(r11)
# .text:0xF8 | 0x800EE270 | size: 0x0
.sym _savegpr_15, global
/* 800EE270 000EB010  91 EB FF BC */	stw r15, -0x44(r11)
# .text:0xFC | 0x800EE274 | size: 0x0
.sym _savegpr_16, global
/* 800EE274 000EB014  92 0B FF C0 */	stw r16, -0x40(r11)
# .text:0x100 | 0x800EE278 | size: 0x0
.sym _savegpr_17, global
/* 800EE278 000EB018  92 2B FF C4 */	stw r17, -0x3c(r11)
# .text:0x104 | 0x800EE27C | size: 0x0
.sym _savegpr_18, global
/* 800EE27C 000EB01C  92 4B FF C8 */	stw r18, -0x38(r11)
# .text:0x108 | 0x800EE280 | size: 0x0
.sym _savegpr_19, global
/* 800EE280 000EB020  92 6B FF CC */	stw r19, -0x34(r11)
# .text:0x10C | 0x800EE284 | size: 0x0
.sym _savegpr_20, global
/* 800EE284 000EB024  92 8B FF D0 */	stw r20, -0x30(r11)
# .text:0x110 | 0x800EE288 | size: 0x0
.sym _savegpr_21, global
/* 800EE288 000EB028  92 AB FF D4 */	stw r21, -0x2c(r11)
# .text:0x114 | 0x800EE28C | size: 0x0
.sym _savegpr_22, global
/* 800EE28C 000EB02C  92 CB FF D8 */	stw r22, -0x28(r11)
# .text:0x118 | 0x800EE290 | size: 0x0
.sym _savegpr_23, global
/* 800EE290 000EB030  92 EB FF DC */	stw r23, -0x24(r11)
# .text:0x11C | 0x800EE294 | size: 0x0
.sym _savegpr_24, global
/* 800EE294 000EB034  93 0B FF E0 */	stw r24, -0x20(r11)
# .text:0x120 | 0x800EE298 | size: 0x0
.sym _savegpr_25, global
/* 800EE298 000EB038  93 2B FF E4 */	stw r25, -0x1c(r11)
# .text:0x124 | 0x800EE29C | size: 0x0
.sym _savegpr_26, global
/* 800EE29C 000EB03C  93 4B FF E8 */	stw r26, -0x18(r11)
# .text:0x128 | 0x800EE2A0 | size: 0x0
.sym _savegpr_27, global
/* 800EE2A0 000EB040  93 6B FF EC */	stw r27, -0x14(r11)
# .text:0x12C | 0x800EE2A4 | size: 0x0
.sym _savegpr_28, global
/* 800EE2A4 000EB044  93 8B FF F0 */	stw r28, -0x10(r11)
# .text:0x130 | 0x800EE2A8 | size: 0x0
.sym _savegpr_29, global
/* 800EE2A8 000EB048  93 AB FF F4 */	stw r29, -0xc(r11)
# .text:0x134 | 0x800EE2AC | size: 0x0
.sym _savegpr_30, global
/* 800EE2AC 000EB04C  93 CB FF F8 */	stw r30, -0x8(r11)
# .text:0x138 | 0x800EE2B0 | size: 0x0
.sym _savegpr_31, global
/* 800EE2B0 000EB050  93 EB FF FC */	stw r31, -0x4(r11)
/* 800EE2B4 000EB054  4E 80 00 20 */	blr
.endfn __save_gpr

# .text:0x140 | 0x800EE2B8 | size: 0x4C
.fn __restore_gpr, global
# .text:0x140 | 0x800EE2B8 | size: 0x0
.sym _restgpr_14, global
/* 800EE2B8 000EB058  81 CB FF B8 */	lwz r14, -0x48(r11)
# .text:0x144 | 0x800EE2BC | size: 0x0
.sym _restgpr_15, global
/* 800EE2BC 000EB05C  81 EB FF BC */	lwz r15, -0x44(r11)
# .text:0x148 | 0x800EE2C0 | size: 0x0
.sym _restgpr_16, global
/* 800EE2C0 000EB060  82 0B FF C0 */	lwz r16, -0x40(r11)
# .text:0x14C | 0x800EE2C4 | size: 0x0
.sym _restgpr_17, global
/* 800EE2C4 000EB064  82 2B FF C4 */	lwz r17, -0x3c(r11)
# .text:0x150 | 0x800EE2C8 | size: 0x0
.sym _restgpr_18, global
/* 800EE2C8 000EB068  82 4B FF C8 */	lwz r18, -0x38(r11)
# .text:0x154 | 0x800EE2CC | size: 0x0
.sym _restgpr_19, global
/* 800EE2CC 000EB06C  82 6B FF CC */	lwz r19, -0x34(r11)
# .text:0x158 | 0x800EE2D0 | size: 0x0
.sym _restgpr_20, global
/* 800EE2D0 000EB070  82 8B FF D0 */	lwz r20, -0x30(r11)
# .text:0x15C | 0x800EE2D4 | size: 0x0
.sym _restgpr_21, global
/* 800EE2D4 000EB074  82 AB FF D4 */	lwz r21, -0x2c(r11)
# .text:0x160 | 0x800EE2D8 | size: 0x0
.sym _restgpr_22, global
/* 800EE2D8 000EB078  82 CB FF D8 */	lwz r22, -0x28(r11)
# .text:0x164 | 0x800EE2DC | size: 0x0
.sym _restgpr_23, global
/* 800EE2DC 000EB07C  82 EB FF DC */	lwz r23, -0x24(r11)
# .text:0x168 | 0x800EE2E0 | size: 0x0
.sym _restgpr_24, global
/* 800EE2E0 000EB080  83 0B FF E0 */	lwz r24, -0x20(r11)
# .text:0x16C | 0x800EE2E4 | size: 0x0
.sym _restgpr_25, global
/* 800EE2E4 000EB084  83 2B FF E4 */	lwz r25, -0x1c(r11)
# .text:0x170 | 0x800EE2E8 | size: 0x0
.sym _restgpr_26, global
/* 800EE2E8 000EB088  83 4B FF E8 */	lwz r26, -0x18(r11)
# .text:0x174 | 0x800EE2EC | size: 0x0
.sym _restgpr_27, global
/* 800EE2EC 000EB08C  83 6B FF EC */	lwz r27, -0x14(r11)
# .text:0x178 | 0x800EE2F0 | size: 0x0
.sym _restgpr_28, global
/* 800EE2F0 000EB090  83 8B FF F0 */	lwz r28, -0x10(r11)
# .text:0x17C | 0x800EE2F4 | size: 0x0
.sym _restgpr_29, global
/* 800EE2F4 000EB094  83 AB FF F4 */	lwz r29, -0xc(r11)
# .text:0x180 | 0x800EE2F8 | size: 0x0
.sym _restgpr_30, global
/* 800EE2F8 000EB098  83 CB FF F8 */	lwz r30, -0x8(r11)
# .text:0x184 | 0x800EE2FC | size: 0x0
.sym _restgpr_31, global
/* 800EE2FC 000EB09C  83 EB FF FC */	lwz r31, -0x4(r11)
/* 800EE300 000EB0A0  4E 80 00 20 */	blr
.endfn __restore_gpr

# .text:0x18C | 0x800EE304 | size: 0xEC
.fn __div2u, global
/* 800EE304 000EB0A4  2C 03 00 00 */	cmpwi r3, 0x0
/* 800EE308 000EB0A8  7C 60 00 34 */	cntlzw r0, r3
/* 800EE30C 000EB0AC  7C 89 00 34 */	cntlzw r9, r4
/* 800EE310 000EB0B0  40 82 00 08 */	bne .L_800EE318
/* 800EE314 000EB0B4  38 09 00 20 */	addi r0, r9, 0x20
.L_800EE318:
/* 800EE318 000EB0B8  2C 05 00 00 */	cmpwi r5, 0x0
/* 800EE31C 000EB0BC  7C A9 00 34 */	cntlzw r9, r5
/* 800EE320 000EB0C0  7C CA 00 34 */	cntlzw r10, r6
/* 800EE324 000EB0C4  40 82 00 08 */	bne .L_800EE32C
/* 800EE328 000EB0C8  39 2A 00 20 */	addi r9, r10, 0x20
.L_800EE32C:
/* 800EE32C 000EB0CC  7C 00 48 00 */	cmpw r0, r9
/* 800EE330 000EB0D0  21 40 00 40 */	subfic r10, r0, 0x40
/* 800EE334 000EB0D4  41 81 00 B0 */	bgt .L_800EE3E4
/* 800EE338 000EB0D8  39 29 00 01 */	addi r9, r9, 0x1
/* 800EE33C 000EB0DC  21 29 00 40 */	subfic r9, r9, 0x40
/* 800EE340 000EB0E0  7C 00 4A 14 */	add r0, r0, r9
/* 800EE344 000EB0E4  7D 29 50 50 */	subf r9, r9, r10
/* 800EE348 000EB0E8  7D 29 03 A6 */	mtctr r9
/* 800EE34C 000EB0EC  2C 09 00 20 */	cmpwi r9, 0x20
/* 800EE350 000EB0F0  38 E9 FF E0 */	subi r7, r9, 0x20
/* 800EE354 000EB0F4  41 80 00 10 */	blt .L_800EE364
/* 800EE358 000EB0F8  7C 68 3C 30 */	srw r8, r3, r7
/* 800EE35C 000EB0FC  38 E0 00 00 */	li r7, 0x0
/* 800EE360 000EB100  48 00 00 18 */	b .L_800EE378
.L_800EE364:
/* 800EE364 000EB104  7C 88 4C 30 */	srw r8, r4, r9
/* 800EE368 000EB108  20 E9 00 20 */	subfic r7, r9, 0x20
/* 800EE36C 000EB10C  7C 67 38 30 */	slw r7, r3, r7
/* 800EE370 000EB110  7D 08 3B 78 */	or r8, r8, r7
/* 800EE374 000EB114  7C 67 4C 30 */	srw r7, r3, r9
.L_800EE378:
/* 800EE378 000EB118  2C 00 00 20 */	cmpwi r0, 0x20
/* 800EE37C 000EB11C  31 20 FF E0 */	subic r9, r0, 0x20
/* 800EE380 000EB120  41 80 00 10 */	blt .L_800EE390
/* 800EE384 000EB124  7C 83 48 30 */	slw r3, r4, r9
/* 800EE388 000EB128  38 80 00 00 */	li r4, 0x0
/* 800EE38C 000EB12C  48 00 00 18 */	b .L_800EE3A4
.L_800EE390:
/* 800EE390 000EB130  7C 63 00 30 */	slw r3, r3, r0
/* 800EE394 000EB134  21 20 00 20 */	subfic r9, r0, 0x20
/* 800EE398 000EB138  7C 89 4C 30 */	srw r9, r4, r9
/* 800EE39C 000EB13C  7C 63 4B 78 */	or r3, r3, r9
/* 800EE3A0 000EB140  7C 84 00 30 */	slw r4, r4, r0
.L_800EE3A4:
/* 800EE3A4 000EB144  39 40 FF FF */	li r10, -0x1
/* 800EE3A8 000EB148  30 E7 00 00 */	addic r7, r7, 0x0
.L_800EE3AC:
/* 800EE3AC 000EB14C  7C 84 21 14 */	adde r4, r4, r4
/* 800EE3B0 000EB150  7C 63 19 14 */	adde r3, r3, r3
/* 800EE3B4 000EB154  7D 08 41 14 */	adde r8, r8, r8
/* 800EE3B8 000EB158  7C E7 39 14 */	adde r7, r7, r7
/* 800EE3BC 000EB15C  7C 06 40 10 */	subfc r0, r6, r8
/* 800EE3C0 000EB160  7D 25 39 11 */	subfe. r9, r5, r7
/* 800EE3C4 000EB164  41 80 00 10 */	blt .L_800EE3D4
/* 800EE3C8 000EB168  7C 08 03 78 */	mr r8, r0
/* 800EE3CC 000EB16C  7D 27 4B 78 */	mr r7, r9
/* 800EE3D0 000EB170  30 0A 00 01 */	addic r0, r10, 0x1
.L_800EE3D4:
/* 800EE3D4 000EB174  42 00 FF D8 */	bdnz .L_800EE3AC
/* 800EE3D8 000EB178  7C 84 21 14 */	adde r4, r4, r4
/* 800EE3DC 000EB17C  7C 63 19 14 */	adde r3, r3, r3
/* 800EE3E0 000EB180  4E 80 00 20 */	blr
.L_800EE3E4:
/* 800EE3E4 000EB184  38 80 00 00 */	li r4, 0x0
/* 800EE3E8 000EB188  38 60 00 00 */	li r3, 0x0
/* 800EE3EC 000EB18C  4E 80 00 20 */	blr
.endfn __div2u

# .text:0x278 | 0x800EE3F0 | size: 0x138
.fn __div2i, global
/* 800EE3F0 000EB190  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800EE3F4 000EB194  54 69 00 01 */	clrrwi. r9, r3, 31
/* 800EE3F8 000EB198  41 82 00 0C */	beq .L_800EE404
/* 800EE3FC 000EB19C  20 84 00 00 */	subfic r4, r4, 0x0
/* 800EE400 000EB1A0  7C 63 01 90 */	subfze r3, r3
.L_800EE404:
/* 800EE404 000EB1A4  91 21 00 08 */	stw r9, 0x8(r1)
/* 800EE408 000EB1A8  54 AA 00 01 */	clrrwi. r10, r5, 31
/* 800EE40C 000EB1AC  41 82 00 0C */	beq .L_800EE418
/* 800EE410 000EB1B0  20 C6 00 00 */	subfic r6, r6, 0x0
/* 800EE414 000EB1B4  7C A5 01 90 */	subfze r5, r5
.L_800EE418:
/* 800EE418 000EB1B8  91 41 00 0C */	stw r10, 0xc(r1)
/* 800EE41C 000EB1BC  2C 03 00 00 */	cmpwi r3, 0x0
/* 800EE420 000EB1C0  7C 60 00 34 */	cntlzw r0, r3
/* 800EE424 000EB1C4  7C 89 00 34 */	cntlzw r9, r4
/* 800EE428 000EB1C8  40 82 00 08 */	bne .L_800EE430
/* 800EE42C 000EB1CC  38 09 00 20 */	addi r0, r9, 0x20
.L_800EE430:
/* 800EE430 000EB1D0  2C 05 00 00 */	cmpwi r5, 0x0
/* 800EE434 000EB1D4  7C A9 00 34 */	cntlzw r9, r5
/* 800EE438 000EB1D8  7C CA 00 34 */	cntlzw r10, r6
/* 800EE43C 000EB1DC  40 82 00 08 */	bne .L_800EE444
/* 800EE440 000EB1E0  39 2A 00 20 */	addi r9, r10, 0x20
.L_800EE444:
/* 800EE444 000EB1E4  7C 00 48 00 */	cmpw r0, r9
/* 800EE448 000EB1E8  21 40 00 40 */	subfic r10, r0, 0x40
/* 800EE44C 000EB1EC  41 81 00 CC */	bgt .L_800EE518
/* 800EE450 000EB1F0  39 29 00 01 */	addi r9, r9, 0x1
/* 800EE454 000EB1F4  21 29 00 40 */	subfic r9, r9, 0x40
/* 800EE458 000EB1F8  7C 00 4A 14 */	add r0, r0, r9
/* 800EE45C 000EB1FC  7D 29 50 50 */	subf r9, r9, r10
/* 800EE460 000EB200  7D 29 03 A6 */	mtctr r9
/* 800EE464 000EB204  2C 09 00 20 */	cmpwi r9, 0x20
/* 800EE468 000EB208  38 E9 FF E0 */	subi r7, r9, 0x20
/* 800EE46C 000EB20C  41 80 00 10 */	blt .L_800EE47C
/* 800EE470 000EB210  7C 68 3C 30 */	srw r8, r3, r7
/* 800EE474 000EB214  38 E0 00 00 */	li r7, 0x0
/* 800EE478 000EB218  48 00 00 18 */	b .L_800EE490
.L_800EE47C:
/* 800EE47C 000EB21C  7C 88 4C 30 */	srw r8, r4, r9
/* 800EE480 000EB220  20 E9 00 20 */	subfic r7, r9, 0x20
/* 800EE484 000EB224  7C 67 38 30 */	slw r7, r3, r7
/* 800EE488 000EB228  7D 08 3B 78 */	or r8, r8, r7
/* 800EE48C 000EB22C  7C 67 4C 30 */	srw r7, r3, r9
.L_800EE490:
/* 800EE490 000EB230  2C 00 00 20 */	cmpwi r0, 0x20
/* 800EE494 000EB234  31 20 FF E0 */	subic r9, r0, 0x20
/* 800EE498 000EB238  41 80 00 10 */	blt .L_800EE4A8
/* 800EE49C 000EB23C  7C 83 48 30 */	slw r3, r4, r9
/* 800EE4A0 000EB240  38 80 00 00 */	li r4, 0x0
/* 800EE4A4 000EB244  48 00 00 18 */	b .L_800EE4BC
.L_800EE4A8:
/* 800EE4A8 000EB248  7C 63 00 30 */	slw r3, r3, r0
/* 800EE4AC 000EB24C  21 20 00 20 */	subfic r9, r0, 0x20
/* 800EE4B0 000EB250  7C 89 4C 30 */	srw r9, r4, r9
/* 800EE4B4 000EB254  7C 63 4B 78 */	or r3, r3, r9
/* 800EE4B8 000EB258  7C 84 00 30 */	slw r4, r4, r0
.L_800EE4BC:
/* 800EE4BC 000EB25C  39 40 FF FF */	li r10, -0x1
/* 800EE4C0 000EB260  30 E7 00 00 */	addic r7, r7, 0x0
.L_800EE4C4:
/* 800EE4C4 000EB264  7C 84 21 14 */	adde r4, r4, r4
/* 800EE4C8 000EB268  7C 63 19 14 */	adde r3, r3, r3
/* 800EE4CC 000EB26C  7D 08 41 14 */	adde r8, r8, r8
/* 800EE4D0 000EB270  7C E7 39 14 */	adde r7, r7, r7
/* 800EE4D4 000EB274  7C 06 40 10 */	subfc r0, r6, r8
/* 800EE4D8 000EB278  7D 25 39 11 */	subfe. r9, r5, r7
/* 800EE4DC 000EB27C  41 80 00 10 */	blt .L_800EE4EC
/* 800EE4E0 000EB280  7C 08 03 78 */	mr r8, r0
/* 800EE4E4 000EB284  7D 27 4B 78 */	mr r7, r9
/* 800EE4E8 000EB288  30 0A 00 01 */	addic r0, r10, 0x1
.L_800EE4EC:
/* 800EE4EC 000EB28C  42 00 FF D8 */	bdnz .L_800EE4C4
/* 800EE4F0 000EB290  7C 84 21 14 */	adde r4, r4, r4
/* 800EE4F4 000EB294  7C 63 19 14 */	adde r3, r3, r3
/* 800EE4F8 000EB298  81 21 00 08 */	lwz r9, 0x8(r1)
/* 800EE4FC 000EB29C  81 41 00 0C */	lwz r10, 0xc(r1)
/* 800EE500 000EB2A0  7D 27 52 79 */	xor. r7, r9, r10
/* 800EE504 000EB2A4  41 82 00 1C */	beq .L_800EE520
/* 800EE508 000EB2A8  2C 09 00 00 */	cmpwi r9, 0x0
/* 800EE50C 000EB2AC  20 84 00 00 */	subfic r4, r4, 0x0
/* 800EE510 000EB2B0  7C 63 01 90 */	subfze r3, r3
/* 800EE514 000EB2B4  48 00 00 0C */	b .L_800EE520
.L_800EE518:
/* 800EE518 000EB2B8  38 80 00 00 */	li r4, 0x0
/* 800EE51C 000EB2BC  38 60 00 00 */	li r3, 0x0
.L_800EE520:
/* 800EE520 000EB2C0  38 21 00 10 */	addi r1, r1, 0x10
/* 800EE524 000EB2C4  4E 80 00 20 */	blr
.endfn __div2i

# .text:0x3B0 | 0x800EE528 | size: 0xE4
.fn __mod2u, global
/* 800EE528 000EB2C8  2C 03 00 00 */	cmpwi r3, 0x0
/* 800EE52C 000EB2CC  7C 60 00 34 */	cntlzw r0, r3
/* 800EE530 000EB2D0  7C 89 00 34 */	cntlzw r9, r4
/* 800EE534 000EB2D4  40 82 00 08 */	bne .L_800EE53C
/* 800EE538 000EB2D8  38 09 00 20 */	addi r0, r9, 0x20
.L_800EE53C:
/* 800EE53C 000EB2DC  2C 05 00 00 */	cmpwi r5, 0x0
/* 800EE540 000EB2E0  7C A9 00 34 */	cntlzw r9, r5
/* 800EE544 000EB2E4  7C CA 00 34 */	cntlzw r10, r6
/* 800EE548 000EB2E8  40 82 00 08 */	bne .L_800EE550
/* 800EE54C 000EB2EC  39 2A 00 20 */	addi r9, r10, 0x20
.L_800EE550:
/* 800EE550 000EB2F0  7C 00 48 00 */	cmpw r0, r9
/* 800EE554 000EB2F4  21 40 00 40 */	subfic r10, r0, 0x40
/* 800EE558 000EB2F8  4D 81 00 20 */	bgtlr
/* 800EE55C 000EB2FC  39 29 00 01 */	addi r9, r9, 0x1
/* 800EE560 000EB300  21 29 00 40 */	subfic r9, r9, 0x40
/* 800EE564 000EB304  7C 00 4A 14 */	add r0, r0, r9
/* 800EE568 000EB308  7D 29 50 50 */	subf r9, r9, r10
/* 800EE56C 000EB30C  7D 29 03 A6 */	mtctr r9
/* 800EE570 000EB310  2C 09 00 20 */	cmpwi r9, 0x20
/* 800EE574 000EB314  38 E9 FF E0 */	subi r7, r9, 0x20
/* 800EE578 000EB318  41 80 00 10 */	blt .L_800EE588
/* 800EE57C 000EB31C  7C 68 3C 30 */	srw r8, r3, r7
/* 800EE580 000EB320  38 E0 00 00 */	li r7, 0x0
/* 800EE584 000EB324  48 00 00 18 */	b .L_800EE59C
.L_800EE588:
/* 800EE588 000EB328  7C 88 4C 30 */	srw r8, r4, r9
/* 800EE58C 000EB32C  20 E9 00 20 */	subfic r7, r9, 0x20
/* 800EE590 000EB330  7C 67 38 30 */	slw r7, r3, r7
/* 800EE594 000EB334  7D 08 3B 78 */	or r8, r8, r7
/* 800EE598 000EB338  7C 67 4C 30 */	srw r7, r3, r9
.L_800EE59C:
/* 800EE59C 000EB33C  2C 00 00 20 */	cmpwi r0, 0x20
/* 800EE5A0 000EB340  31 20 FF E0 */	subic r9, r0, 0x20
/* 800EE5A4 000EB344  41 80 00 10 */	blt .L_800EE5B4
/* 800EE5A8 000EB348  7C 83 48 30 */	slw r3, r4, r9
/* 800EE5AC 000EB34C  38 80 00 00 */	li r4, 0x0
/* 800EE5B0 000EB350  48 00 00 18 */	b .L_800EE5C8
.L_800EE5B4:
/* 800EE5B4 000EB354  7C 63 00 30 */	slw r3, r3, r0
/* 800EE5B8 000EB358  21 20 00 20 */	subfic r9, r0, 0x20
/* 800EE5BC 000EB35C  7C 89 4C 30 */	srw r9, r4, r9
/* 800EE5C0 000EB360  7C 63 4B 78 */	or r3, r3, r9
/* 800EE5C4 000EB364  7C 84 00 30 */	slw r4, r4, r0
.L_800EE5C8:
/* 800EE5C8 000EB368  39 40 FF FF */	li r10, -0x1
/* 800EE5CC 000EB36C  30 E7 00 00 */	addic r7, r7, 0x0
.L_800EE5D0:
/* 800EE5D0 000EB370  7C 84 21 14 */	adde r4, r4, r4
/* 800EE5D4 000EB374  7C 63 19 14 */	adde r3, r3, r3
/* 800EE5D8 000EB378  7D 08 41 14 */	adde r8, r8, r8
/* 800EE5DC 000EB37C  7C E7 39 14 */	adde r7, r7, r7
/* 800EE5E0 000EB380  7C 06 40 10 */	subfc r0, r6, r8
/* 800EE5E4 000EB384  7D 25 39 11 */	subfe. r9, r5, r7
/* 800EE5E8 000EB388  41 80 00 10 */	blt .L_800EE5F8
/* 800EE5EC 000EB38C  7C 08 03 78 */	mr r8, r0
/* 800EE5F0 000EB390  7D 27 4B 78 */	mr r7, r9
/* 800EE5F4 000EB394  30 0A 00 01 */	addic r0, r10, 0x1
.L_800EE5F8:
/* 800EE5F8 000EB398  42 00 FF D8 */	bdnz .L_800EE5D0
/* 800EE5FC 000EB39C  7D 04 43 78 */	mr r4, r8
/* 800EE600 000EB3A0  7C E3 3B 78 */	mr r3, r7
/* 800EE604 000EB3A4  4E 80 00 20 */	blr
/* 800EE608 000EB3A8  4E 80 00 20 */	blr
.endfn __mod2u

# .text:0x494 | 0x800EE60C | size: 0x10C
.fn __mod2i, global
/* 800EE60C 000EB3AC  2F 83 00 00 */	cmpwi cr7, r3, 0x0
/* 800EE610 000EB3B0  40 9C 00 0C */	bge cr7, .L_800EE61C
/* 800EE614 000EB3B4  20 84 00 00 */	subfic r4, r4, 0x0
/* 800EE618 000EB3B8  7C 63 01 90 */	subfze r3, r3
.L_800EE61C:
/* 800EE61C 000EB3BC  2C 05 00 00 */	cmpwi r5, 0x0
/* 800EE620 000EB3C0  40 80 00 0C */	bge .L_800EE62C
/* 800EE624 000EB3C4  20 C6 00 00 */	subfic r6, r6, 0x0
/* 800EE628 000EB3C8  7C A5 01 90 */	subfze r5, r5
.L_800EE62C:
/* 800EE62C 000EB3CC  2C 03 00 00 */	cmpwi r3, 0x0
/* 800EE630 000EB3D0  7C 60 00 34 */	cntlzw r0, r3
/* 800EE634 000EB3D4  7C 89 00 34 */	cntlzw r9, r4
/* 800EE638 000EB3D8  40 82 00 08 */	bne .L_800EE640
/* 800EE63C 000EB3DC  38 09 00 20 */	addi r0, r9, 0x20
.L_800EE640:
/* 800EE640 000EB3E0  2C 05 00 00 */	cmpwi r5, 0x0
/* 800EE644 000EB3E4  7C A9 00 34 */	cntlzw r9, r5
/* 800EE648 000EB3E8  7C CA 00 34 */	cntlzw r10, r6
/* 800EE64C 000EB3EC  40 82 00 08 */	bne .L_800EE654
/* 800EE650 000EB3F0  39 2A 00 20 */	addi r9, r10, 0x20
.L_800EE654:
/* 800EE654 000EB3F4  7C 00 48 00 */	cmpw r0, r9
/* 800EE658 000EB3F8  21 40 00 40 */	subfic r10, r0, 0x40
/* 800EE65C 000EB3FC  41 81 00 AC */	bgt .L_800EE708
/* 800EE660 000EB400  39 29 00 01 */	addi r9, r9, 0x1
/* 800EE664 000EB404  21 29 00 40 */	subfic r9, r9, 0x40
/* 800EE668 000EB408  7C 00 4A 14 */	add r0, r0, r9
/* 800EE66C 000EB40C  7D 29 50 50 */	subf r9, r9, r10
/* 800EE670 000EB410  7D 29 03 A6 */	mtctr r9
/* 800EE674 000EB414  2C 09 00 20 */	cmpwi r9, 0x20
/* 800EE678 000EB418  38 E9 FF E0 */	subi r7, r9, 0x20
/* 800EE67C 000EB41C  41 80 00 10 */	blt .L_800EE68C
/* 800EE680 000EB420  7C 68 3C 30 */	srw r8, r3, r7
/* 800EE684 000EB424  38 E0 00 00 */	li r7, 0x0
/* 800EE688 000EB428  48 00 00 18 */	b .L_800EE6A0
.L_800EE68C:
/* 800EE68C 000EB42C  7C 88 4C 30 */	srw r8, r4, r9
/* 800EE690 000EB430  20 E9 00 20 */	subfic r7, r9, 0x20
/* 800EE694 000EB434  7C 67 38 30 */	slw r7, r3, r7
/* 800EE698 000EB438  7D 08 3B 78 */	or r8, r8, r7
/* 800EE69C 000EB43C  7C 67 4C 30 */	srw r7, r3, r9
.L_800EE6A0:
/* 800EE6A0 000EB440  2C 00 00 20 */	cmpwi r0, 0x20
/* 800EE6A4 000EB444  31 20 FF E0 */	subic r9, r0, 0x20
/* 800EE6A8 000EB448  41 80 00 10 */	blt .L_800EE6B8
/* 800EE6AC 000EB44C  7C 83 48 30 */	slw r3, r4, r9
/* 800EE6B0 000EB450  38 80 00 00 */	li r4, 0x0
/* 800EE6B4 000EB454  48 00 00 18 */	b .L_800EE6CC
.L_800EE6B8:
/* 800EE6B8 000EB458  7C 63 00 30 */	slw r3, r3, r0
/* 800EE6BC 000EB45C  21 20 00 20 */	subfic r9, r0, 0x20
/* 800EE6C0 000EB460  7C 89 4C 30 */	srw r9, r4, r9
/* 800EE6C4 000EB464  7C 63 4B 78 */	or r3, r3, r9
/* 800EE6C8 000EB468  7C 84 00 30 */	slw r4, r4, r0
.L_800EE6CC:
/* 800EE6CC 000EB46C  39 40 FF FF */	li r10, -0x1
/* 800EE6D0 000EB470  30 E7 00 00 */	addic r7, r7, 0x0
.L_800EE6D4:
/* 800EE6D4 000EB474  7C 84 21 14 */	adde r4, r4, r4
/* 800EE6D8 000EB478  7C 63 19 14 */	adde r3, r3, r3
/* 800EE6DC 000EB47C  7D 08 41 14 */	adde r8, r8, r8
/* 800EE6E0 000EB480  7C E7 39 14 */	adde r7, r7, r7
/* 800EE6E4 000EB484  7C 06 40 10 */	subfc r0, r6, r8
/* 800EE6E8 000EB488  7D 25 39 11 */	subfe. r9, r5, r7
/* 800EE6EC 000EB48C  41 80 00 10 */	blt .L_800EE6FC
/* 800EE6F0 000EB490  7C 08 03 78 */	mr r8, r0
/* 800EE6F4 000EB494  7D 27 4B 78 */	mr r7, r9
/* 800EE6F8 000EB498  30 0A 00 01 */	addic r0, r10, 0x1
.L_800EE6FC:
/* 800EE6FC 000EB49C  42 00 FF D8 */	bdnz .L_800EE6D4
/* 800EE700 000EB4A0  7D 04 43 78 */	mr r4, r8
/* 800EE704 000EB4A4  7C E3 3B 78 */	mr r3, r7
.L_800EE708:
/* 800EE708 000EB4A8  4C 9C 00 20 */	bgelr cr7
/* 800EE70C 000EB4AC  20 84 00 00 */	subfic r4, r4, 0x0
/* 800EE710 000EB4B0  7C 63 01 90 */	subfze r3, r3
/* 800EE714 000EB4B4  4E 80 00 20 */	blr
.endfn __mod2i

# .text:0x5A0 | 0x800EE718 | size: 0x24
.fn __shl2i, global
/* 800EE718 000EB4B8  21 05 00 20 */	subfic r8, r5, 0x20
/* 800EE71C 000EB4BC  31 25 FF E0 */	subic r9, r5, 0x20
/* 800EE720 000EB4C0  7C 63 28 30 */	slw r3, r3, r5
/* 800EE724 000EB4C4  7C 8A 44 30 */	srw r10, r4, r8
/* 800EE728 000EB4C8  7C 63 53 78 */	or r3, r3, r10
/* 800EE72C 000EB4CC  7C 8A 48 30 */	slw r10, r4, r9
/* 800EE730 000EB4D0  7C 63 53 78 */	or r3, r3, r10
/* 800EE734 000EB4D4  7C 84 28 30 */	slw r4, r4, r5
/* 800EE738 000EB4D8  4E 80 00 20 */	blr
.endfn __shl2i

# .text:0x5C4 | 0x800EE73C | size: 0x24
.fn __shr2u, global
/* 800EE73C 000EB4DC  21 05 00 20 */	subfic r8, r5, 0x20
/* 800EE740 000EB4E0  31 25 FF E0 */	subic r9, r5, 0x20
/* 800EE744 000EB4E4  7C 84 2C 30 */	srw r4, r4, r5
/* 800EE748 000EB4E8  7C 6A 40 30 */	slw r10, r3, r8
/* 800EE74C 000EB4EC  7C 84 53 78 */	or r4, r4, r10
/* 800EE750 000EB4F0  7C 6A 4C 30 */	srw r10, r3, r9
/* 800EE754 000EB4F4  7C 84 53 78 */	or r4, r4, r10
/* 800EE758 000EB4F8  7C 63 2C 30 */	srw r3, r3, r5
/* 800EE75C 000EB4FC  4E 80 00 20 */	blr
.endfn __shr2u

# .text:0x5E8 | 0x800EE760 | size: 0x28
.fn __shr2i, global
/* 800EE760 000EB500  21 05 00 20 */	subfic r8, r5, 0x20
/* 800EE764 000EB504  35 25 FF E0 */	subic. r9, r5, 0x20
/* 800EE768 000EB508  7C 84 2C 30 */	srw r4, r4, r5
/* 800EE76C 000EB50C  7C 6A 40 30 */	slw r10, r3, r8
/* 800EE770 000EB510  7C 84 53 78 */	or r4, r4, r10
/* 800EE774 000EB514  7C 6A 4E 30 */	sraw r10, r3, r9
/* 800EE778 000EB518  40 81 00 08 */	ble .L_800EE780
/* 800EE77C 000EB51C  7C 84 53 78 */	or r4, r4, r10
.L_800EE780:
/* 800EE780 000EB520  7C 63 2E 30 */	sraw r3, r3, r5
/* 800EE784 000EB524  4E 80 00 20 */	blr
.endfn __shr2i

# .text:0x610 | 0x800EE788 | size: 0xCC
.fn __cvt_dbl_usll, global
/* 800EE788 000EB528  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800EE78C 000EB52C  D8 21 00 08 */	stfd f1, 0x8(r1)
/* 800EE790 000EB530  80 61 00 08 */	lwz r3, 0x8(r1)
/* 800EE794 000EB534  80 81 00 0C */	lwz r4, 0xc(r1)
/* 800EE798 000EB538  54 65 65 7E */	extrwi r5, r3, 11, 1
/* 800EE79C 000EB53C  28 05 03 FF */	cmplwi r5, 0x3ff
/* 800EE7A0 000EB540  40 80 00 10 */	bge .L_800EE7B0
/* 800EE7A4 000EB544  38 60 00 00 */	li r3, 0x0
/* 800EE7A8 000EB548  38 80 00 00 */	li r4, 0x0
/* 800EE7AC 000EB54C  48 00 00 A0 */	b .L_800EE84C
.L_800EE7B0:
/* 800EE7B0 000EB550  7C 66 1B 78 */	mr r6, r3
/* 800EE7B4 000EB554  54 63 03 3E */	clrlwi r3, r3, 12
/* 800EE7B8 000EB558  64 63 00 10 */	oris r3, r3, 0x10
/* 800EE7BC 000EB55C  38 A5 FB CD */	subi r5, r5, 0x433
/* 800EE7C0 000EB560  2C 05 00 00 */	cmpwi r5, 0x0
/* 800EE7C4 000EB564  40 80 00 2C */	bge .L_800EE7F0
/* 800EE7C8 000EB568  7C A5 00 D0 */	neg r5, r5
/* 800EE7CC 000EB56C  21 05 00 20 */	subfic r8, r5, 0x20
/* 800EE7D0 000EB570  31 25 FF E0 */	subic r9, r5, 0x20
/* 800EE7D4 000EB574  7C 84 2C 30 */	srw r4, r4, r5
/* 800EE7D8 000EB578  7C 6A 40 30 */	slw r10, r3, r8
/* 800EE7DC 000EB57C  7C 84 53 78 */	or r4, r4, r10
/* 800EE7E0 000EB580  7C 6A 4C 30 */	srw r10, r3, r9
/* 800EE7E4 000EB584  7C 84 53 78 */	or r4, r4, r10
/* 800EE7E8 000EB588  7C 63 2C 30 */	srw r3, r3, r5
/* 800EE7EC 000EB58C  48 00 00 50 */	b .L_800EE83C
.L_800EE7F0:
/* 800EE7F0 000EB590  2C 05 00 0A */	cmpwi r5, 0xa
/* 800EE7F4 000EB594  40 A1 00 28 */	ble+ .L_800EE81C
/* 800EE7F8 000EB598  54 C6 00 01 */	clrrwi. r6, r6, 31
/* 800EE7FC 000EB59C  41 82 00 10 */	beq .L_800EE80C
/* 800EE800 000EB5A0  3C 60 80 00 */	lis r3, 0x8000
/* 800EE804 000EB5A4  38 80 00 00 */	li r4, 0x0
/* 800EE808 000EB5A8  48 00 00 44 */	b .L_800EE84C
.L_800EE80C:
/* 800EE80C 000EB5AC  3C 60 7F FF */	lis r3, 0x7fff
/* 800EE810 000EB5B0  60 63 FF FF */	ori r3, r3, 0xffff
/* 800EE814 000EB5B4  38 80 FF FF */	li r4, -0x1
/* 800EE818 000EB5B8  48 00 00 34 */	b .L_800EE84C
.L_800EE81C:
/* 800EE81C 000EB5BC  21 05 00 20 */	subfic r8, r5, 0x20
/* 800EE820 000EB5C0  31 25 FF E0 */	subic r9, r5, 0x20
/* 800EE824 000EB5C4  7C 63 28 30 */	slw r3, r3, r5
/* 800EE828 000EB5C8  7C 8A 44 30 */	srw r10, r4, r8
/* 800EE82C 000EB5CC  7C 63 53 78 */	or r3, r3, r10
/* 800EE830 000EB5D0  7C 8A 48 30 */	slw r10, r4, r9
/* 800EE834 000EB5D4  7C 63 53 78 */	or r3, r3, r10
/* 800EE838 000EB5D8  7C 84 28 30 */	slw r4, r4, r5
.L_800EE83C:
/* 800EE83C 000EB5DC  54 C6 00 01 */	clrrwi. r6, r6, 31
/* 800EE840 000EB5E0  41 82 00 0C */	beq .L_800EE84C
/* 800EE844 000EB5E4  20 84 00 00 */	subfic r4, r4, 0x0
/* 800EE848 000EB5E8  7C 63 01 90 */	subfze r3, r3
.L_800EE84C:
/* 800EE84C 000EB5EC  38 21 00 10 */	addi r1, r1, 0x10
/* 800EE850 000EB5F0  4E 80 00 20 */	blr
.endfn __cvt_dbl_usll

# 0x802166E8..0x80216700 | size: 0x18
.rodata
.balign 8

# .rodata:0x0 | 0x802166E8 | size: 0x18
.obj __constants, local
	.double 0
	.double 4294967296
	.double 2147483648
.endobj __constants
