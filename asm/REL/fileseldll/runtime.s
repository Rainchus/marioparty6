.include "macros.inc"
.file "runtime.c"

# 0x0000E404..0x0000EE48 | size: 0xA44
.text
.balign 4

# .text:0x0 | 0xE404 | size: 0x5C
.fn fn_1_E404, global
/* 0000E404 0000E4E8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000E408 0000E4EC  3C 80 00 00 */	lis r4, lbl_1_rodata_2D8@h
/* 0000E40C 0000E4F0  60 84 00 00 */	ori r4, r4, lbl_1_rodata_2D8@l
/* 0000E410 0000E4F4  38 60 00 00 */	li r3, 0x0
/* 0000E414 0000E4F8  C8 04 00 00 */	lfd f0, 0x0(r4)
/* 0000E418 0000E4FC  C8 64 00 08 */	lfd f3, 0x8(r4)
/* 0000E41C 0000E500  C8 84 00 10 */	lfd f4, 0x10(r4)
/* 0000E420 0000E504  FC 01 00 00 */	fcmpu cr0, f1, f0
/* 0000E424 0000E508  FF 01 18 00 */	fcmpu cr6, f1, f3
/* 0000E428 0000E50C  41 80 00 30 */	blt .L_0000E458
/* 0000E42C 0000E510  38 63 FF FF */	subi r3, r3, 0x1
/* 0000E430 0000E514  40 98 00 28 */	bge cr6, .L_0000E458
/* 0000E434 0000E518  FF 81 20 00 */	fcmpu cr7, f1, f4
/* 0000E438 0000E51C  FC 40 08 90 */	fmr f2, f1
/* 0000E43C 0000E520  41 9C 00 08 */	blt cr7, .L_0000E444
/* 0000E440 0000E524  FC 41 20 28 */	fsub f2, f1, f4
.L_0000E444:
/* 0000E444 0000E528  FC 40 10 1E */	fctiwz f2, f2
/* 0000E448 0000E52C  D8 41 00 08 */	stfd f2, 0x8(r1)
/* 0000E44C 0000E530  80 61 00 0C */	lwz r3, 0xc(r1)
/* 0000E450 0000E534  41 9C 00 08 */	blt cr7, .L_0000E458
/* 0000E454 0000E538  3C 63 80 00 */	addis r3, r3, 0x8000
.L_0000E458:
/* 0000E458 0000E53C  38 21 00 10 */	addi r1, r1, 0x10
/* 0000E45C 0000E540  4E 80 00 20 */	blr
.endfn fn_1_E404

# .text:0x5C | 0xE460 | size: 0x4C
.fn __save_fpr, global
# .text:0x5C | 0xE460 | size: 0x0
.sym _savefpr_14, global
/* 0000E460 0000E544  D9 CB FF 70 */	stfd f14, -0x90(r11)
# .text:0x60 | 0xE464 | size: 0x0
.sym _savefpr_15, global
/* 0000E464 0000E548  D9 EB FF 78 */	stfd f15, -0x88(r11)
# .text:0x64 | 0xE468 | size: 0x0
.sym _savefpr_16, global
/* 0000E468 0000E54C  DA 0B FF 80 */	stfd f16, -0x80(r11)
# .text:0x68 | 0xE46C | size: 0x0
.sym _savefpr_17, global
/* 0000E46C 0000E550  DA 2B FF 88 */	stfd f17, -0x78(r11)
# .text:0x6C | 0xE470 | size: 0x0
.sym _savefpr_18, global
/* 0000E470 0000E554  DA 4B FF 90 */	stfd f18, -0x70(r11)
# .text:0x70 | 0xE474 | size: 0x0
.sym _savefpr_19, global
/* 0000E474 0000E558  DA 6B FF 98 */	stfd f19, -0x68(r11)
# .text:0x74 | 0xE478 | size: 0x0
.sym _savefpr_20, global
/* 0000E478 0000E55C  DA 8B FF A0 */	stfd f20, -0x60(r11)
# .text:0x78 | 0xE47C | size: 0x0
.sym _savefpr_21, global
/* 0000E47C 0000E560  DA AB FF A8 */	stfd f21, -0x58(r11)
# .text:0x7C | 0xE480 | size: 0x0
.sym _savefpr_22, global
/* 0000E480 0000E564  DA CB FF B0 */	stfd f22, -0x50(r11)
# .text:0x80 | 0xE484 | size: 0x0
.sym _savefpr_23, global
/* 0000E484 0000E568  DA EB FF B8 */	stfd f23, -0x48(r11)
# .text:0x84 | 0xE488 | size: 0x0
.sym _savefpr_24, global
/* 0000E488 0000E56C  DB 0B FF C0 */	stfd f24, -0x40(r11)
# .text:0x88 | 0xE48C | size: 0x0
.sym _savefpr_25, global
/* 0000E48C 0000E570  DB 2B FF C8 */	stfd f25, -0x38(r11)
# .text:0x8C | 0xE490 | size: 0x0
.sym _savefpr_26, global
/* 0000E490 0000E574  DB 4B FF D0 */	stfd f26, -0x30(r11)
# .text:0x90 | 0xE494 | size: 0x0
.sym _savefpr_27, global
/* 0000E494 0000E578  DB 6B FF D8 */	stfd f27, -0x28(r11)
# .text:0x94 | 0xE498 | size: 0x0
.sym _savefpr_28, global
/* 0000E498 0000E57C  DB 8B FF E0 */	stfd f28, -0x20(r11)
# .text:0x98 | 0xE49C | size: 0x0
.sym _savefpr_29, global
/* 0000E49C 0000E580  DB AB FF E8 */	stfd f29, -0x18(r11)
# .text:0x9C | 0xE4A0 | size: 0x0
.sym _savefpr_30, global
/* 0000E4A0 0000E584  DB CB FF F0 */	stfd f30, -0x10(r11)
# .text:0xA0 | 0xE4A4 | size: 0x0
.sym _savefpr_31, global
/* 0000E4A4 0000E588  DB EB FF F8 */	stfd f31, -0x8(r11)
/* 0000E4A8 0000E58C  4E 80 00 20 */	blr
.endfn __save_fpr

# .text:0xA8 | 0xE4AC | size: 0x64
.fn _savevr, global
# .text:0xA8 | 0xE4AC | size: 0x0
.sym _savev20, global
/* 0000E4AC 0000E590  39 80 FF 40 */	li r12, -0xc0
/* 0000E4B0 0000E594  7E 8C 01 CE */	.4byte 0x7E8C01CE /* invalid */
# .text:0xB0 | 0xE4B4 | size: 0x0
.sym _savev21, global
/* 0000E4B4 0000E598  39 80 FF 50 */	li r12, -0xb0
/* 0000E4B8 0000E59C  7E AC 01 CE */	.4byte 0x7EAC01CE /* invalid */
# .text:0xB8 | 0xE4BC | size: 0x0
.sym _savev22, global
/* 0000E4BC 0000E5A0  39 80 FF 60 */	li r12, -0xa0
/* 0000E4C0 0000E5A4  7E CC 01 CE */	.4byte 0x7ECC01CE /* invalid */
# .text:0xC0 | 0xE4C4 | size: 0x0
.sym _savev23, global
/* 0000E4C4 0000E5A8  39 80 FF 70 */	li r12, -0x90
/* 0000E4C8 0000E5AC  7E EC 01 CE */	.4byte 0x7EEC01CE /* invalid */
# .text:0xC8 | 0xE4CC | size: 0x0
.sym _savev24, global
/* 0000E4CC 0000E5B0  39 80 FF 80 */	li r12, -0x80
/* 0000E4D0 0000E5B4  7F 0C 01 CE */	.4byte 0x7F0C01CE /* invalid */
# .text:0xD0 | 0xE4D4 | size: 0x0
.sym _savev25, global
/* 0000E4D4 0000E5B8  39 80 FF 90 */	li r12, -0x70
/* 0000E4D8 0000E5BC  7F 2C 01 CE */	.4byte 0x7F2C01CE /* invalid */
# .text:0xD8 | 0xE4DC | size: 0x0
.sym _savev26, global
/* 0000E4DC 0000E5C0  39 80 FF A0 */	li r12, -0x60
/* 0000E4E0 0000E5C4  7F 4C 01 CE */	.4byte 0x7F4C01CE /* invalid */
# .text:0xE0 | 0xE4E4 | size: 0x0
.sym _savev27, global
/* 0000E4E4 0000E5C8  39 80 FF B0 */	li r12, -0x50
/* 0000E4E8 0000E5CC  7F 6C 01 CE */	.4byte 0x7F6C01CE /* invalid */
# .text:0xE8 | 0xE4EC | size: 0x0
.sym _savev28, global
/* 0000E4EC 0000E5D0  39 80 FF C0 */	li r12, -0x40
/* 0000E4F0 0000E5D4  7F 8C 01 CE */	.4byte 0x7F8C01CE /* invalid */
# .text:0xF0 | 0xE4F4 | size: 0x0
.sym _savev29, global
/* 0000E4F4 0000E5D8  39 80 FF D0 */	li r12, -0x30
/* 0000E4F8 0000E5DC  7F AC 01 CE */	.4byte 0x7FAC01CE /* invalid */
# .text:0xF8 | 0xE4FC | size: 0x0
.sym _savev30, global
/* 0000E4FC 0000E5E0  39 80 FF E0 */	li r12, -0x20
/* 0000E500 0000E5E4  7F CC 01 CE */	.4byte 0x7FCC01CE /* invalid */
# .text:0x100 | 0xE504 | size: 0x0
.sym _savev31, global
/* 0000E504 0000E5E8  39 80 FF F0 */	li r12, -0x10
/* 0000E508 0000E5EC  7F EC 01 CE */	.4byte 0x7FEC01CE /* invalid */
/* 0000E50C 0000E5F0  4E 80 00 20 */	blr
.endfn _savevr

# .text:0x10C | 0xE510 | size: 0x4C
.fn __restore_fpr, global
# .text:0x10C | 0xE510 | size: 0x0
.sym _restfpr_14, global
/* 0000E510 0000E5F4  C9 CB FF 70 */	lfd f14, -0x90(r11)
# .text:0x110 | 0xE514 | size: 0x0
.sym _restfpr_15, global
/* 0000E514 0000E5F8  C9 EB FF 78 */	lfd f15, -0x88(r11)
# .text:0x114 | 0xE518 | size: 0x0
.sym _restfpr_16, global
/* 0000E518 0000E5FC  CA 0B FF 80 */	lfd f16, -0x80(r11)
# .text:0x118 | 0xE51C | size: 0x0
.sym _restfpr_17, global
/* 0000E51C 0000E600  CA 2B FF 88 */	lfd f17, -0x78(r11)
# .text:0x11C | 0xE520 | size: 0x0
.sym _restfpr_18, global
/* 0000E520 0000E604  CA 4B FF 90 */	lfd f18, -0x70(r11)
# .text:0x120 | 0xE524 | size: 0x0
.sym _restfpr_19, global
/* 0000E524 0000E608  CA 6B FF 98 */	lfd f19, -0x68(r11)
# .text:0x124 | 0xE528 | size: 0x0
.sym _restfpr_20, global
/* 0000E528 0000E60C  CA 8B FF A0 */	lfd f20, -0x60(r11)
# .text:0x128 | 0xE52C | size: 0x0
.sym _restfpr_21, global
/* 0000E52C 0000E610  CA AB FF A8 */	lfd f21, -0x58(r11)
# .text:0x12C | 0xE530 | size: 0x0
.sym _restfpr_22, global
/* 0000E530 0000E614  CA CB FF B0 */	lfd f22, -0x50(r11)
# .text:0x130 | 0xE534 | size: 0x0
.sym _restfpr_23, global
/* 0000E534 0000E618  CA EB FF B8 */	lfd f23, -0x48(r11)
# .text:0x134 | 0xE538 | size: 0x0
.sym _restfpr_24, global
/* 0000E538 0000E61C  CB 0B FF C0 */	lfd f24, -0x40(r11)
# .text:0x138 | 0xE53C | size: 0x0
.sym _restfpr_25, global
/* 0000E53C 0000E620  CB 2B FF C8 */	lfd f25, -0x38(r11)
# .text:0x13C | 0xE540 | size: 0x0
.sym _restfpr_26, global
/* 0000E540 0000E624  CB 4B FF D0 */	lfd f26, -0x30(r11)
# .text:0x140 | 0xE544 | size: 0x0
.sym _restfpr_27, global
/* 0000E544 0000E628  CB 6B FF D8 */	lfd f27, -0x28(r11)
# .text:0x144 | 0xE548 | size: 0x0
.sym _restfpr_28, global
/* 0000E548 0000E62C  CB 8B FF E0 */	lfd f28, -0x20(r11)
# .text:0x148 | 0xE54C | size: 0x0
.sym _restfpr_29, global
/* 0000E54C 0000E630  CB AB FF E8 */	lfd f29, -0x18(r11)
# .text:0x14C | 0xE550 | size: 0x0
.sym _restfpr_30, global
/* 0000E550 0000E634  CB CB FF F0 */	lfd f30, -0x10(r11)
# .text:0x150 | 0xE554 | size: 0x0
.sym _restfpr_31, global
/* 0000E554 0000E638  CB EB FF F8 */	lfd f31, -0x8(r11)
/* 0000E558 0000E63C  4E 80 00 20 */	blr
.endfn __restore_fpr

# .text:0x158 | 0xE55C | size: 0x4C
.fn __save_gpr, global
# .text:0x158 | 0xE55C | size: 0x0
.sym _savegpr_14, global
/* 0000E55C 0000E640  91 CB FF B8 */	stw r14, -0x48(r11)
# .text:0x15C | 0xE560 | size: 0x0
.sym _savegpr_15, global
/* 0000E560 0000E644  91 EB FF BC */	stw r15, -0x44(r11)
# .text:0x160 | 0xE564 | size: 0x0
.sym _savegpr_16, global
/* 0000E564 0000E648  92 0B FF C0 */	stw r16, -0x40(r11)
# .text:0x164 | 0xE568 | size: 0x0
.sym _savegpr_17, global
/* 0000E568 0000E64C  92 2B FF C4 */	stw r17, -0x3c(r11)
# .text:0x168 | 0xE56C | size: 0x0
.sym _savegpr_18, global
/* 0000E56C 0000E650  92 4B FF C8 */	stw r18, -0x38(r11)
# .text:0x16C | 0xE570 | size: 0x0
.sym _savegpr_19, global
/* 0000E570 0000E654  92 6B FF CC */	stw r19, -0x34(r11)
# .text:0x170 | 0xE574 | size: 0x0
.sym _savegpr_20, global
/* 0000E574 0000E658  92 8B FF D0 */	stw r20, -0x30(r11)
# .text:0x174 | 0xE578 | size: 0x0
.sym _savegpr_21, global
/* 0000E578 0000E65C  92 AB FF D4 */	stw r21, -0x2c(r11)
# .text:0x178 | 0xE57C | size: 0x0
.sym _savegpr_22, global
/* 0000E57C 0000E660  92 CB FF D8 */	stw r22, -0x28(r11)
# .text:0x17C | 0xE580 | size: 0x0
.sym _savegpr_23, global
/* 0000E580 0000E664  92 EB FF DC */	stw r23, -0x24(r11)
# .text:0x180 | 0xE584 | size: 0x0
.sym _savegpr_24, global
/* 0000E584 0000E668  93 0B FF E0 */	stw r24, -0x20(r11)
# .text:0x184 | 0xE588 | size: 0x0
.sym _savegpr_25, global
/* 0000E588 0000E66C  93 2B FF E4 */	stw r25, -0x1c(r11)
# .text:0x188 | 0xE58C | size: 0x0
.sym _savegpr_26, global
/* 0000E58C 0000E670  93 4B FF E8 */	stw r26, -0x18(r11)
# .text:0x18C | 0xE590 | size: 0x0
.sym _savegpr_27, global
/* 0000E590 0000E674  93 6B FF EC */	stw r27, -0x14(r11)
# .text:0x190 | 0xE594 | size: 0x0
.sym _savegpr_28, global
/* 0000E594 0000E678  93 8B FF F0 */	stw r28, -0x10(r11)
# .text:0x194 | 0xE598 | size: 0x0
.sym _savegpr_29, global
/* 0000E598 0000E67C  93 AB FF F4 */	stw r29, -0xc(r11)
# .text:0x198 | 0xE59C | size: 0x0
.sym _savegpr_30, global
/* 0000E59C 0000E680  93 CB FF F8 */	stw r30, -0x8(r11)
# .text:0x19C | 0xE5A0 | size: 0x0
.sym _savegpr_31, global
/* 0000E5A0 0000E684  93 EB FF FC */	stw r31, -0x4(r11)
/* 0000E5A4 0000E688  4E 80 00 20 */	blr
.endfn __save_gpr

# .text:0x1A4 | 0xE5A8 | size: 0x4C
.fn __restore_gpr, global
# .text:0x1A4 | 0xE5A8 | size: 0x0
.sym _restgpr_14, global
/* 0000E5A8 0000E68C  81 CB FF B8 */	lwz r14, -0x48(r11)
# .text:0x1A8 | 0xE5AC | size: 0x0
.sym _restgpr_15, global
/* 0000E5AC 0000E690  81 EB FF BC */	lwz r15, -0x44(r11)
# .text:0x1AC | 0xE5B0 | size: 0x0
.sym _restgpr_16, global
/* 0000E5B0 0000E694  82 0B FF C0 */	lwz r16, -0x40(r11)
# .text:0x1B0 | 0xE5B4 | size: 0x0
.sym _restgpr_17, global
/* 0000E5B4 0000E698  82 2B FF C4 */	lwz r17, -0x3c(r11)
# .text:0x1B4 | 0xE5B8 | size: 0x0
.sym _restgpr_18, global
/* 0000E5B8 0000E69C  82 4B FF C8 */	lwz r18, -0x38(r11)
# .text:0x1B8 | 0xE5BC | size: 0x0
.sym _restgpr_19, global
/* 0000E5BC 0000E6A0  82 6B FF CC */	lwz r19, -0x34(r11)
# .text:0x1BC | 0xE5C0 | size: 0x0
.sym _restgpr_20, global
/* 0000E5C0 0000E6A4  82 8B FF D0 */	lwz r20, -0x30(r11)
# .text:0x1C0 | 0xE5C4 | size: 0x0
.sym _restgpr_21, global
/* 0000E5C4 0000E6A8  82 AB FF D4 */	lwz r21, -0x2c(r11)
# .text:0x1C4 | 0xE5C8 | size: 0x0
.sym _restgpr_22, global
/* 0000E5C8 0000E6AC  82 CB FF D8 */	lwz r22, -0x28(r11)
# .text:0x1C8 | 0xE5CC | size: 0x0
.sym _restgpr_23, global
/* 0000E5CC 0000E6B0  82 EB FF DC */	lwz r23, -0x24(r11)
# .text:0x1CC | 0xE5D0 | size: 0x0
.sym _restgpr_24, global
/* 0000E5D0 0000E6B4  83 0B FF E0 */	lwz r24, -0x20(r11)
# .text:0x1D0 | 0xE5D4 | size: 0x0
.sym _restgpr_25, global
/* 0000E5D4 0000E6B8  83 2B FF E4 */	lwz r25, -0x1c(r11)
# .text:0x1D4 | 0xE5D8 | size: 0x0
.sym _restgpr_26, global
/* 0000E5D8 0000E6BC  83 4B FF E8 */	lwz r26, -0x18(r11)
# .text:0x1D8 | 0xE5DC | size: 0x0
.sym _restgpr_27, global
/* 0000E5DC 0000E6C0  83 6B FF EC */	lwz r27, -0x14(r11)
# .text:0x1DC | 0xE5E0 | size: 0x0
.sym _restgpr_28, global
/* 0000E5E0 0000E6C4  83 8B FF F0 */	lwz r28, -0x10(r11)
# .text:0x1E0 | 0xE5E4 | size: 0x0
.sym _restgpr_29, global
/* 0000E5E4 0000E6C8  83 AB FF F4 */	lwz r29, -0xc(r11)
# .text:0x1E4 | 0xE5E8 | size: 0x0
.sym _restgpr_30, global
/* 0000E5E8 0000E6CC  83 CB FF F8 */	lwz r30, -0x8(r11)
# .text:0x1E8 | 0xE5EC | size: 0x0
.sym _restgpr_31, global
/* 0000E5EC 0000E6D0  83 EB FF FC */	lwz r31, -0x4(r11)
/* 0000E5F0 0000E6D4  4E 80 00 20 */	blr
.endfn __restore_gpr

# .text:0x1F0 | 0xE5F4 | size: 0x64
.fn _restorevr, global
# .text:0x1F0 | 0xE5F4 | size: 0x0
.sym _restv20, global
/* 0000E5F4 0000E6D8  39 80 FF 40 */	li r12, -0xc0
/* 0000E5F8 0000E6DC  7E 8C 00 CE */	.4byte 0x7E8C00CE /* invalid */
# .text:0x1F8 | 0xE5FC | size: 0x0
.sym _restv21, global
/* 0000E5FC 0000E6E0  39 80 FF 50 */	li r12, -0xb0
/* 0000E600 0000E6E4  7E AC 00 CE */	.4byte 0x7EAC00CE /* invalid */
# .text:0x200 | 0xE604 | size: 0x0
.sym _restv22, global
/* 0000E604 0000E6E8  39 80 FF 60 */	li r12, -0xa0
/* 0000E608 0000E6EC  7E CC 00 CE */	.4byte 0x7ECC00CE /* invalid */
# .text:0x208 | 0xE60C | size: 0x0
.sym _restv23, global
/* 0000E60C 0000E6F0  39 80 FF 70 */	li r12, -0x90
/* 0000E610 0000E6F4  7E EC 00 CE */	.4byte 0x7EEC00CE /* invalid */
# .text:0x210 | 0xE614 | size: 0x0
.sym _restv24, global
/* 0000E614 0000E6F8  39 80 FF 80 */	li r12, -0x80
/* 0000E618 0000E6FC  7F 0C 00 CE */	.4byte 0x7F0C00CE /* invalid */
# .text:0x218 | 0xE61C | size: 0x0
.sym _restv25, global
/* 0000E61C 0000E700  39 80 FF 90 */	li r12, -0x70
/* 0000E620 0000E704  7F 2C 00 CE */	.4byte 0x7F2C00CE /* invalid */
# .text:0x220 | 0xE624 | size: 0x0
.sym _restv26, global
/* 0000E624 0000E708  39 80 FF A0 */	li r12, -0x60
/* 0000E628 0000E70C  7F 4C 00 CE */	.4byte 0x7F4C00CE /* invalid */
# .text:0x228 | 0xE62C | size: 0x0
.sym _restv27, global
/* 0000E62C 0000E710  39 80 FF B0 */	li r12, -0x50
/* 0000E630 0000E714  7F 6C 00 CE */	.4byte 0x7F6C00CE /* invalid */
# .text:0x230 | 0xE634 | size: 0x0
.sym _restv28, global
/* 0000E634 0000E718  39 80 FF C0 */	li r12, -0x40
/* 0000E638 0000E71C  7F 8C 00 CE */	.4byte 0x7F8C00CE /* invalid */
# .text:0x238 | 0xE63C | size: 0x0
.sym _restv29, global
/* 0000E63C 0000E720  39 80 FF D0 */	li r12, -0x30
/* 0000E640 0000E724  7F AC 00 CE */	.4byte 0x7FAC00CE /* invalid */
# .text:0x240 | 0xE644 | size: 0x0
.sym _restv30, global
/* 0000E644 0000E728  39 80 FF E0 */	li r12, -0x20
/* 0000E648 0000E72C  7F CC 00 CE */	.4byte 0x7FCC00CE /* invalid */
# .text:0x248 | 0xE64C | size: 0x0
.sym _restv31, global
/* 0000E64C 0000E730  39 80 FF F0 */	li r12, -0x10
/* 0000E650 0000E734  7F EC 00 CE */	.4byte 0x7FEC00CE /* invalid */
/* 0000E654 0000E738  4E 80 00 20 */	blr
.endfn _restorevr

# .text:0x254 | 0xE658 | size: 0xEC
.fn fn_1_E658, global
/* 0000E658 0000E73C  2C 03 00 00 */	cmpwi r3, 0x0
/* 0000E65C 0000E740  7C 60 00 34 */	cntlzw r0, r3
/* 0000E660 0000E744  7C 89 00 34 */	cntlzw r9, r4
/* 0000E664 0000E748  40 82 00 08 */	bne .L_0000E66C
/* 0000E668 0000E74C  38 09 00 20 */	addi r0, r9, 0x20
.L_0000E66C:
/* 0000E66C 0000E750  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000E670 0000E754  7C A9 00 34 */	cntlzw r9, r5
/* 0000E674 0000E758  7C CA 00 34 */	cntlzw r10, r6
/* 0000E678 0000E75C  40 82 00 08 */	bne .L_0000E680
/* 0000E67C 0000E760  39 2A 00 20 */	addi r9, r10, 0x20
.L_0000E680:
/* 0000E680 0000E764  7C 00 48 00 */	cmpw r0, r9
/* 0000E684 0000E768  21 40 00 40 */	subfic r10, r0, 0x40
/* 0000E688 0000E76C  41 81 00 B0 */	bgt .L_0000E738
/* 0000E68C 0000E770  39 29 00 01 */	addi r9, r9, 0x1
/* 0000E690 0000E774  21 29 00 40 */	subfic r9, r9, 0x40
/* 0000E694 0000E778  7C 00 4A 14 */	add r0, r0, r9
/* 0000E698 0000E77C  7D 29 50 50 */	subf r9, r9, r10
/* 0000E69C 0000E780  7D 29 03 A6 */	mtctr r9
/* 0000E6A0 0000E784  2C 09 00 20 */	cmpwi r9, 0x20
/* 0000E6A4 0000E788  38 E9 FF E0 */	subi r7, r9, 0x20
/* 0000E6A8 0000E78C  41 80 00 10 */	blt .L_0000E6B8
/* 0000E6AC 0000E790  7C 68 3C 30 */	srw r8, r3, r7
/* 0000E6B0 0000E794  38 E0 00 00 */	li r7, 0x0
/* 0000E6B4 0000E798  48 00 00 18 */	b .L_0000E6CC
.L_0000E6B8:
/* 0000E6B8 0000E79C  7C 88 4C 30 */	srw r8, r4, r9
/* 0000E6BC 0000E7A0  20 E9 00 20 */	subfic r7, r9, 0x20
/* 0000E6C0 0000E7A4  7C 67 38 30 */	slw r7, r3, r7
/* 0000E6C4 0000E7A8  7D 08 3B 78 */	or r8, r8, r7
/* 0000E6C8 0000E7AC  7C 67 4C 30 */	srw r7, r3, r9
.L_0000E6CC:
/* 0000E6CC 0000E7B0  2C 00 00 20 */	cmpwi r0, 0x20
/* 0000E6D0 0000E7B4  31 20 FF E0 */	subic r9, r0, 0x20
/* 0000E6D4 0000E7B8  41 80 00 10 */	blt .L_0000E6E4
/* 0000E6D8 0000E7BC  7C 83 48 30 */	slw r3, r4, r9
/* 0000E6DC 0000E7C0  38 80 00 00 */	li r4, 0x0
/* 0000E6E0 0000E7C4  48 00 00 18 */	b .L_0000E6F8
.L_0000E6E4:
/* 0000E6E4 0000E7C8  7C 63 00 30 */	slw r3, r3, r0
/* 0000E6E8 0000E7CC  21 20 00 20 */	subfic r9, r0, 0x20
/* 0000E6EC 0000E7D0  7C 89 4C 30 */	srw r9, r4, r9
/* 0000E6F0 0000E7D4  7C 63 4B 78 */	or r3, r3, r9
/* 0000E6F4 0000E7D8  7C 84 00 30 */	slw r4, r4, r0
.L_0000E6F8:
/* 0000E6F8 0000E7DC  39 40 FF FF */	li r10, -0x1
/* 0000E6FC 0000E7E0  30 E7 00 00 */	addic r7, r7, 0x0
.L_0000E700:
/* 0000E700 0000E7E4  7C 84 21 14 */	adde r4, r4, r4
/* 0000E704 0000E7E8  7C 63 19 14 */	adde r3, r3, r3
/* 0000E708 0000E7EC  7D 08 41 14 */	adde r8, r8, r8
/* 0000E70C 0000E7F0  7C E7 39 14 */	adde r7, r7, r7
/* 0000E710 0000E7F4  7C 06 40 10 */	subfc r0, r6, r8
/* 0000E714 0000E7F8  7D 25 39 11 */	subfe. r9, r5, r7
/* 0000E718 0000E7FC  41 80 00 10 */	blt .L_0000E728
/* 0000E71C 0000E800  7C 08 03 78 */	mr r8, r0
/* 0000E720 0000E804  7D 27 4B 78 */	mr r7, r9
/* 0000E724 0000E808  30 0A 00 01 */	addic r0, r10, 0x1
.L_0000E728:
/* 0000E728 0000E80C  42 00 FF D8 */	bdnz .L_0000E700
/* 0000E72C 0000E810  7C 84 21 14 */	adde r4, r4, r4
/* 0000E730 0000E814  7C 63 19 14 */	adde r3, r3, r3
/* 0000E734 0000E818  4E 80 00 20 */	blr
.L_0000E738:
/* 0000E738 0000E81C  38 80 00 00 */	li r4, 0x0
/* 0000E73C 0000E820  38 60 00 00 */	li r3, 0x0
/* 0000E740 0000E824  4E 80 00 20 */	blr
.endfn fn_1_E658

# .text:0x340 | 0xE744 | size: 0x138
.fn fn_1_E744, global
/* 0000E744 0000E828  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000E748 0000E82C  54 69 00 01 */	clrrwi. r9, r3, 31
/* 0000E74C 0000E830  41 82 00 0C */	beq .L_0000E758
/* 0000E750 0000E834  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000E754 0000E838  7C 63 01 90 */	subfze r3, r3
.L_0000E758:
/* 0000E758 0000E83C  91 21 00 08 */	stw r9, 0x8(r1)
/* 0000E75C 0000E840  54 AA 00 01 */	clrrwi. r10, r5, 31
/* 0000E760 0000E844  41 82 00 0C */	beq .L_0000E76C
/* 0000E764 0000E848  20 C6 00 00 */	subfic r6, r6, 0x0
/* 0000E768 0000E84C  7C A5 01 90 */	subfze r5, r5
.L_0000E76C:
/* 0000E76C 0000E850  91 41 00 0C */	stw r10, 0xc(r1)
/* 0000E770 0000E854  2C 03 00 00 */	cmpwi r3, 0x0
/* 0000E774 0000E858  7C 60 00 34 */	cntlzw r0, r3
/* 0000E778 0000E85C  7C 89 00 34 */	cntlzw r9, r4
/* 0000E77C 0000E860  40 82 00 08 */	bne .L_0000E784
/* 0000E780 0000E864  38 09 00 20 */	addi r0, r9, 0x20
.L_0000E784:
/* 0000E784 0000E868  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000E788 0000E86C  7C A9 00 34 */	cntlzw r9, r5
/* 0000E78C 0000E870  7C CA 00 34 */	cntlzw r10, r6
/* 0000E790 0000E874  40 82 00 08 */	bne .L_0000E798
/* 0000E794 0000E878  39 2A 00 20 */	addi r9, r10, 0x20
.L_0000E798:
/* 0000E798 0000E87C  7C 00 48 00 */	cmpw r0, r9
/* 0000E79C 0000E880  21 40 00 40 */	subfic r10, r0, 0x40
/* 0000E7A0 0000E884  41 81 00 CC */	bgt .L_0000E86C
/* 0000E7A4 0000E888  39 29 00 01 */	addi r9, r9, 0x1
/* 0000E7A8 0000E88C  21 29 00 40 */	subfic r9, r9, 0x40
/* 0000E7AC 0000E890  7C 00 4A 14 */	add r0, r0, r9
/* 0000E7B0 0000E894  7D 29 50 50 */	subf r9, r9, r10
/* 0000E7B4 0000E898  7D 29 03 A6 */	mtctr r9
/* 0000E7B8 0000E89C  2C 09 00 20 */	cmpwi r9, 0x20
/* 0000E7BC 0000E8A0  38 E9 FF E0 */	subi r7, r9, 0x20
/* 0000E7C0 0000E8A4  41 80 00 10 */	blt .L_0000E7D0
/* 0000E7C4 0000E8A8  7C 68 3C 30 */	srw r8, r3, r7
/* 0000E7C8 0000E8AC  38 E0 00 00 */	li r7, 0x0
/* 0000E7CC 0000E8B0  48 00 00 18 */	b .L_0000E7E4
.L_0000E7D0:
/* 0000E7D0 0000E8B4  7C 88 4C 30 */	srw r8, r4, r9
/* 0000E7D4 0000E8B8  20 E9 00 20 */	subfic r7, r9, 0x20
/* 0000E7D8 0000E8BC  7C 67 38 30 */	slw r7, r3, r7
/* 0000E7DC 0000E8C0  7D 08 3B 78 */	or r8, r8, r7
/* 0000E7E0 0000E8C4  7C 67 4C 30 */	srw r7, r3, r9
.L_0000E7E4:
/* 0000E7E4 0000E8C8  2C 00 00 20 */	cmpwi r0, 0x20
/* 0000E7E8 0000E8CC  31 20 FF E0 */	subic r9, r0, 0x20
/* 0000E7EC 0000E8D0  41 80 00 10 */	blt .L_0000E7FC
/* 0000E7F0 0000E8D4  7C 83 48 30 */	slw r3, r4, r9
/* 0000E7F4 0000E8D8  38 80 00 00 */	li r4, 0x0
/* 0000E7F8 0000E8DC  48 00 00 18 */	b .L_0000E810
.L_0000E7FC:
/* 0000E7FC 0000E8E0  7C 63 00 30 */	slw r3, r3, r0
/* 0000E800 0000E8E4  21 20 00 20 */	subfic r9, r0, 0x20
/* 0000E804 0000E8E8  7C 89 4C 30 */	srw r9, r4, r9
/* 0000E808 0000E8EC  7C 63 4B 78 */	or r3, r3, r9
/* 0000E80C 0000E8F0  7C 84 00 30 */	slw r4, r4, r0
.L_0000E810:
/* 0000E810 0000E8F4  39 40 FF FF */	li r10, -0x1
/* 0000E814 0000E8F8  30 E7 00 00 */	addic r7, r7, 0x0
.L_0000E818:
/* 0000E818 0000E8FC  7C 84 21 14 */	adde r4, r4, r4
/* 0000E81C 0000E900  7C 63 19 14 */	adde r3, r3, r3
/* 0000E820 0000E904  7D 08 41 14 */	adde r8, r8, r8
/* 0000E824 0000E908  7C E7 39 14 */	adde r7, r7, r7
/* 0000E828 0000E90C  7C 06 40 10 */	subfc r0, r6, r8
/* 0000E82C 0000E910  7D 25 39 11 */	subfe. r9, r5, r7
/* 0000E830 0000E914  41 80 00 10 */	blt .L_0000E840
/* 0000E834 0000E918  7C 08 03 78 */	mr r8, r0
/* 0000E838 0000E91C  7D 27 4B 78 */	mr r7, r9
/* 0000E83C 0000E920  30 0A 00 01 */	addic r0, r10, 0x1
.L_0000E840:
/* 0000E840 0000E924  42 00 FF D8 */	bdnz .L_0000E818
/* 0000E844 0000E928  7C 84 21 14 */	adde r4, r4, r4
/* 0000E848 0000E92C  7C 63 19 14 */	adde r3, r3, r3
/* 0000E84C 0000E930  81 21 00 08 */	lwz r9, 0x8(r1)
/* 0000E850 0000E934  81 41 00 0C */	lwz r10, 0xc(r1)
/* 0000E854 0000E938  7D 27 52 79 */	xor. r7, r9, r10
/* 0000E858 0000E93C  41 82 00 10 */	beq .L_0000E868
/* 0000E85C 0000E940  2C 09 00 00 */	cmpwi r9, 0x0
/* 0000E860 0000E944  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000E864 0000E948  7C 63 01 90 */	subfze r3, r3
.L_0000E868:
/* 0000E868 0000E94C  48 00 00 0C */	b .L_0000E874
.L_0000E86C:
/* 0000E86C 0000E950  38 80 00 00 */	li r4, 0x0
/* 0000E870 0000E954  38 60 00 00 */	li r3, 0x0
.L_0000E874:
/* 0000E874 0000E958  38 21 00 10 */	addi r1, r1, 0x10
/* 0000E878 0000E95C  4E 80 00 20 */	blr
.endfn fn_1_E744

# .text:0x478 | 0xE87C | size: 0xE4
.fn fn_1_E87C, global
/* 0000E87C 0000E960  2C 03 00 00 */	cmpwi r3, 0x0
/* 0000E880 0000E964  7C 60 00 34 */	cntlzw r0, r3
/* 0000E884 0000E968  7C 89 00 34 */	cntlzw r9, r4
/* 0000E888 0000E96C  40 82 00 08 */	bne .L_0000E890
/* 0000E88C 0000E970  38 09 00 20 */	addi r0, r9, 0x20
.L_0000E890:
/* 0000E890 0000E974  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000E894 0000E978  7C A9 00 34 */	cntlzw r9, r5
/* 0000E898 0000E97C  7C CA 00 34 */	cntlzw r10, r6
/* 0000E89C 0000E980  40 82 00 08 */	bne .L_0000E8A4
/* 0000E8A0 0000E984  39 2A 00 20 */	addi r9, r10, 0x20
.L_0000E8A4:
/* 0000E8A4 0000E988  7C 00 48 00 */	cmpw r0, r9
/* 0000E8A8 0000E98C  21 40 00 40 */	subfic r10, r0, 0x40
/* 0000E8AC 0000E990  41 81 00 B0 */	bgt .L_0000E95C
/* 0000E8B0 0000E994  39 29 00 01 */	addi r9, r9, 0x1
/* 0000E8B4 0000E998  21 29 00 40 */	subfic r9, r9, 0x40
/* 0000E8B8 0000E99C  7C 00 4A 14 */	add r0, r0, r9
/* 0000E8BC 0000E9A0  7D 29 50 50 */	subf r9, r9, r10
/* 0000E8C0 0000E9A4  7D 29 03 A6 */	mtctr r9
/* 0000E8C4 0000E9A8  2C 09 00 20 */	cmpwi r9, 0x20
/* 0000E8C8 0000E9AC  38 E9 FF E0 */	subi r7, r9, 0x20
/* 0000E8CC 0000E9B0  41 80 00 10 */	blt .L_0000E8DC
/* 0000E8D0 0000E9B4  7C 68 3C 30 */	srw r8, r3, r7
/* 0000E8D4 0000E9B8  38 E0 00 00 */	li r7, 0x0
/* 0000E8D8 0000E9BC  48 00 00 18 */	b .L_0000E8F0
.L_0000E8DC:
/* 0000E8DC 0000E9C0  7C 88 4C 30 */	srw r8, r4, r9
/* 0000E8E0 0000E9C4  20 E9 00 20 */	subfic r7, r9, 0x20
/* 0000E8E4 0000E9C8  7C 67 38 30 */	slw r7, r3, r7
/* 0000E8E8 0000E9CC  7D 08 3B 78 */	or r8, r8, r7
/* 0000E8EC 0000E9D0  7C 67 4C 30 */	srw r7, r3, r9
.L_0000E8F0:
/* 0000E8F0 0000E9D4  2C 00 00 20 */	cmpwi r0, 0x20
/* 0000E8F4 0000E9D8  31 20 FF E0 */	subic r9, r0, 0x20
/* 0000E8F8 0000E9DC  41 80 00 10 */	blt .L_0000E908
/* 0000E8FC 0000E9E0  7C 83 48 30 */	slw r3, r4, r9
/* 0000E900 0000E9E4  38 80 00 00 */	li r4, 0x0
/* 0000E904 0000E9E8  48 00 00 18 */	b .L_0000E91C
.L_0000E908:
/* 0000E908 0000E9EC  7C 63 00 30 */	slw r3, r3, r0
/* 0000E90C 0000E9F0  21 20 00 20 */	subfic r9, r0, 0x20
/* 0000E910 0000E9F4  7C 89 4C 30 */	srw r9, r4, r9
/* 0000E914 0000E9F8  7C 63 4B 78 */	or r3, r3, r9
/* 0000E918 0000E9FC  7C 84 00 30 */	slw r4, r4, r0
.L_0000E91C:
/* 0000E91C 0000EA00  39 40 FF FF */	li r10, -0x1
/* 0000E920 0000EA04  30 E7 00 00 */	addic r7, r7, 0x0
.L_0000E924:
/* 0000E924 0000EA08  7C 84 21 14 */	adde r4, r4, r4
/* 0000E928 0000EA0C  7C 63 19 14 */	adde r3, r3, r3
/* 0000E92C 0000EA10  7D 08 41 14 */	adde r8, r8, r8
/* 0000E930 0000EA14  7C E7 39 14 */	adde r7, r7, r7
/* 0000E934 0000EA18  7C 06 40 10 */	subfc r0, r6, r8
/* 0000E938 0000EA1C  7D 25 39 11 */	subfe. r9, r5, r7
/* 0000E93C 0000EA20  41 80 00 10 */	blt .L_0000E94C
/* 0000E940 0000EA24  7C 08 03 78 */	mr r8, r0
/* 0000E944 0000EA28  7D 27 4B 78 */	mr r7, r9
/* 0000E948 0000EA2C  30 0A 00 01 */	addic r0, r10, 0x1
.L_0000E94C:
/* 0000E94C 0000EA30  42 00 FF D8 */	bdnz .L_0000E924
/* 0000E950 0000EA34  7D 04 43 78 */	mr r4, r8
/* 0000E954 0000EA38  7C E3 3B 78 */	mr r3, r7
/* 0000E958 0000EA3C  4E 80 00 20 */	blr
.L_0000E95C:
/* 0000E95C 0000EA40  4E 80 00 20 */	blr
.endfn fn_1_E87C

# .text:0x55C | 0xE960 | size: 0x10C
.fn fn_1_E960, global
/* 0000E960 0000EA44  2F 83 00 00 */	cmpwi cr7, r3, 0x0
/* 0000E964 0000EA48  40 9C 00 0C */	bge cr7, .L_0000E970
/* 0000E968 0000EA4C  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000E96C 0000EA50  7C 63 01 90 */	subfze r3, r3
.L_0000E970:
/* 0000E970 0000EA54  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000E974 0000EA58  40 80 00 0C */	bge .L_0000E980
/* 0000E978 0000EA5C  20 C6 00 00 */	subfic r6, r6, 0x0
/* 0000E97C 0000EA60  7C A5 01 90 */	subfze r5, r5
.L_0000E980:
/* 0000E980 0000EA64  2C 03 00 00 */	cmpwi r3, 0x0
/* 0000E984 0000EA68  7C 60 00 34 */	cntlzw r0, r3
/* 0000E988 0000EA6C  7C 89 00 34 */	cntlzw r9, r4
/* 0000E98C 0000EA70  40 82 00 08 */	bne .L_0000E994
/* 0000E990 0000EA74  38 09 00 20 */	addi r0, r9, 0x20
.L_0000E994:
/* 0000E994 0000EA78  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000E998 0000EA7C  7C A9 00 34 */	cntlzw r9, r5
/* 0000E99C 0000EA80  7C CA 00 34 */	cntlzw r10, r6
/* 0000E9A0 0000EA84  40 82 00 08 */	bne .L_0000E9A8
/* 0000E9A4 0000EA88  39 2A 00 20 */	addi r9, r10, 0x20
.L_0000E9A8:
/* 0000E9A8 0000EA8C  7C 00 48 00 */	cmpw r0, r9
/* 0000E9AC 0000EA90  21 40 00 40 */	subfic r10, r0, 0x40
/* 0000E9B0 0000EA94  41 81 00 AC */	bgt .L_0000EA5C
/* 0000E9B4 0000EA98  39 29 00 01 */	addi r9, r9, 0x1
/* 0000E9B8 0000EA9C  21 29 00 40 */	subfic r9, r9, 0x40
/* 0000E9BC 0000EAA0  7C 00 4A 14 */	add r0, r0, r9
/* 0000E9C0 0000EAA4  7D 29 50 50 */	subf r9, r9, r10
/* 0000E9C4 0000EAA8  7D 29 03 A6 */	mtctr r9
/* 0000E9C8 0000EAAC  2C 09 00 20 */	cmpwi r9, 0x20
/* 0000E9CC 0000EAB0  38 E9 FF E0 */	subi r7, r9, 0x20
/* 0000E9D0 0000EAB4  41 80 00 10 */	blt .L_0000E9E0
/* 0000E9D4 0000EAB8  7C 68 3C 30 */	srw r8, r3, r7
/* 0000E9D8 0000EABC  38 E0 00 00 */	li r7, 0x0
/* 0000E9DC 0000EAC0  48 00 00 18 */	b .L_0000E9F4
.L_0000E9E0:
/* 0000E9E0 0000EAC4  7C 88 4C 30 */	srw r8, r4, r9
/* 0000E9E4 0000EAC8  20 E9 00 20 */	subfic r7, r9, 0x20
/* 0000E9E8 0000EACC  7C 67 38 30 */	slw r7, r3, r7
/* 0000E9EC 0000EAD0  7D 08 3B 78 */	or r8, r8, r7
/* 0000E9F0 0000EAD4  7C 67 4C 30 */	srw r7, r3, r9
.L_0000E9F4:
/* 0000E9F4 0000EAD8  2C 00 00 20 */	cmpwi r0, 0x20
/* 0000E9F8 0000EADC  31 20 FF E0 */	subic r9, r0, 0x20
/* 0000E9FC 0000EAE0  41 80 00 10 */	blt .L_0000EA0C
/* 0000EA00 0000EAE4  7C 83 48 30 */	slw r3, r4, r9
/* 0000EA04 0000EAE8  38 80 00 00 */	li r4, 0x0
/* 0000EA08 0000EAEC  48 00 00 18 */	b .L_0000EA20
.L_0000EA0C:
/* 0000EA0C 0000EAF0  7C 63 00 30 */	slw r3, r3, r0
/* 0000EA10 0000EAF4  21 20 00 20 */	subfic r9, r0, 0x20
/* 0000EA14 0000EAF8  7C 89 4C 30 */	srw r9, r4, r9
/* 0000EA18 0000EAFC  7C 63 4B 78 */	or r3, r3, r9
/* 0000EA1C 0000EB00  7C 84 00 30 */	slw r4, r4, r0
.L_0000EA20:
/* 0000EA20 0000EB04  39 40 FF FF */	li r10, -0x1
/* 0000EA24 0000EB08  30 E7 00 00 */	addic r7, r7, 0x0
.L_0000EA28:
/* 0000EA28 0000EB0C  7C 84 21 14 */	adde r4, r4, r4
/* 0000EA2C 0000EB10  7C 63 19 14 */	adde r3, r3, r3
/* 0000EA30 0000EB14  7D 08 41 14 */	adde r8, r8, r8
/* 0000EA34 0000EB18  7C E7 39 14 */	adde r7, r7, r7
/* 0000EA38 0000EB1C  7C 06 40 10 */	subfc r0, r6, r8
/* 0000EA3C 0000EB20  7D 25 39 11 */	subfe. r9, r5, r7
/* 0000EA40 0000EB24  41 80 00 10 */	blt .L_0000EA50
/* 0000EA44 0000EB28  7C 08 03 78 */	mr r8, r0
/* 0000EA48 0000EB2C  7D 27 4B 78 */	mr r7, r9
/* 0000EA4C 0000EB30  30 0A 00 01 */	addic r0, r10, 0x1
.L_0000EA50:
/* 0000EA50 0000EB34  42 00 FF D8 */	bdnz .L_0000EA28
/* 0000EA54 0000EB38  7D 04 43 78 */	mr r4, r8
/* 0000EA58 0000EB3C  7C E3 3B 78 */	mr r3, r7
.L_0000EA5C:
/* 0000EA5C 0000EB40  40 9C 00 0C */	bge cr7, .L_0000EA68
/* 0000EA60 0000EB44  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000EA64 0000EB48  7C 63 01 90 */	subfze r3, r3
.L_0000EA68:
/* 0000EA68 0000EB4C  4E 80 00 20 */	blr
.endfn fn_1_E960

# .text:0x668 | 0xEA6C | size: 0x24
.fn fn_1_EA6C, global
/* 0000EA6C 0000EB50  21 05 00 20 */	subfic r8, r5, 0x20
/* 0000EA70 0000EB54  31 25 FF E0 */	subic r9, r5, 0x20
/* 0000EA74 0000EB58  7C 63 28 30 */	slw r3, r3, r5
/* 0000EA78 0000EB5C  7C 8A 44 30 */	srw r10, r4, r8
/* 0000EA7C 0000EB60  7C 63 53 78 */	or r3, r3, r10
/* 0000EA80 0000EB64  7C 8A 48 30 */	slw r10, r4, r9
/* 0000EA84 0000EB68  7C 63 53 78 */	or r3, r3, r10
/* 0000EA88 0000EB6C  7C 84 28 30 */	slw r4, r4, r5
/* 0000EA8C 0000EB70  4E 80 00 20 */	blr
.endfn fn_1_EA6C

# .text:0x68C | 0xEA90 | size: 0x24
.fn fn_1_EA90, global
/* 0000EA90 0000EB74  21 05 00 20 */	subfic r8, r5, 0x20
/* 0000EA94 0000EB78  31 25 FF E0 */	subic r9, r5, 0x20
/* 0000EA98 0000EB7C  7C 84 2C 30 */	srw r4, r4, r5
/* 0000EA9C 0000EB80  7C 6A 40 30 */	slw r10, r3, r8
/* 0000EAA0 0000EB84  7C 84 53 78 */	or r4, r4, r10
/* 0000EAA4 0000EB88  7C 6A 4C 30 */	srw r10, r3, r9
/* 0000EAA8 0000EB8C  7C 84 53 78 */	or r4, r4, r10
/* 0000EAAC 0000EB90  7C 63 2C 30 */	srw r3, r3, r5
/* 0000EAB0 0000EB94  4E 80 00 20 */	blr
.endfn fn_1_EA90

# .text:0x6B0 | 0xEAB4 | size: 0x28
.fn fn_1_EAB4, global
/* 0000EAB4 0000EB98  21 05 00 20 */	subfic r8, r5, 0x20
/* 0000EAB8 0000EB9C  35 25 FF E0 */	subic. r9, r5, 0x20
/* 0000EABC 0000EBA0  7C 84 2C 30 */	srw r4, r4, r5
/* 0000EAC0 0000EBA4  7C 6A 40 30 */	slw r10, r3, r8
/* 0000EAC4 0000EBA8  7C 84 53 78 */	or r4, r4, r10
/* 0000EAC8 0000EBAC  7C 6A 4E 30 */	sraw r10, r3, r9
/* 0000EACC 0000EBB0  40 81 00 08 */	ble .L_0000EAD4
/* 0000EAD0 0000EBB4  7C 84 53 78 */	or r4, r4, r10
.L_0000EAD4:
/* 0000EAD4 0000EBB8  7C 63 2E 30 */	sraw r3, r3, r5
/* 0000EAD8 0000EBBC  4E 80 00 20 */	blr
.endfn fn_1_EAB4

# .text:0x6D8 | 0xEADC | size: 0xB0
.fn fn_1_EADC, global
/* 0000EADC 0000EBC0  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000EAE0 0000EBC4  54 65 00 01 */	clrrwi. r5, r3, 31
/* 0000EAE4 0000EBC8  41 82 00 0C */	beq .L_0000EAF0
/* 0000EAE8 0000EBCC  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000EAEC 0000EBD0  7C 63 01 90 */	subfze r3, r3
.L_0000EAF0:
/* 0000EAF0 0000EBD4  7C 67 23 79 */	or. r7, r3, r4
/* 0000EAF4 0000EBD8  38 C0 00 00 */	li r6, 0x0
/* 0000EAF8 0000EBDC  41 82 00 80 */	beq .L_0000EB78
/* 0000EAFC 0000EBE0  7C 67 00 34 */	cntlzw r7, r3
/* 0000EB00 0000EBE4  7C 88 00 34 */	cntlzw r8, r4
/* 0000EB04 0000EBE8  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 0000EB08 0000EBEC  7D 29 FE 70 */	srawi r9, r9, 31
/* 0000EB0C 0000EBF0  7D 29 40 38 */	and r9, r9, r8
/* 0000EB10 0000EBF4  7C E7 4A 14 */	add r7, r7, r9
/* 0000EB14 0000EBF8  21 07 00 20 */	subfic r8, r7, 0x20
/* 0000EB18 0000EBFC  31 27 FF E0 */	subic r9, r7, 0x20
/* 0000EB1C 0000EC00  7C 63 38 30 */	slw r3, r3, r7
/* 0000EB20 0000EC04  7C 8A 44 30 */	srw r10, r4, r8
/* 0000EB24 0000EC08  7C 63 53 78 */	or r3, r3, r10
/* 0000EB28 0000EC0C  7C 8A 48 30 */	slw r10, r4, r9
/* 0000EB2C 0000EC10  7C 63 53 78 */	or r3, r3, r10
/* 0000EB30 0000EC14  7C 84 38 30 */	slw r4, r4, r7
/* 0000EB34 0000EC18  7C C7 30 50 */	subf r6, r7, r6
/* 0000EB38 0000EC1C  54 87 05 7E */	clrlwi r7, r4, 21
/* 0000EB3C 0000EC20  2C 07 04 00 */	cmpwi r7, 0x400
/* 0000EB40 0000EC24  38 C6 04 3E */	addi r6, r6, 0x43e
/* 0000EB44 0000EC28  41 80 00 1C */	blt .L_0000EB60
/* 0000EB48 0000EC2C  41 81 00 0C */	bgt .L_0000EB54
/* 0000EB4C 0000EC30  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 0000EB50 0000EC34  41 82 00 10 */	beq .L_0000EB60
.L_0000EB54:
/* 0000EB54 0000EC38  30 84 08 00 */	addic r4, r4, 0x800
/* 0000EB58 0000EC3C  7C 63 01 94 */	addze r3, r3
/* 0000EB5C 0000EC40  7C C6 01 94 */	addze r6, r6
.L_0000EB60:
/* 0000EB60 0000EC44  54 84 A8 3E */	rotrwi r4, r4, 11
/* 0000EB64 0000EC48  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 0000EB68 0000EC4C  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 0000EB6C 0000EC50  54 C6 A0 16 */	slwi r6, r6, 20
/* 0000EB70 0000EC54  7C C3 1B 78 */	or r3, r6, r3
/* 0000EB74 0000EC58  7C A3 1B 78 */	or r3, r5, r3
.L_0000EB78:
/* 0000EB78 0000EC5C  90 61 00 08 */	stw r3, 0x8(r1)
/* 0000EB7C 0000EC60  90 81 00 0C */	stw r4, 0xc(r1)
/* 0000EB80 0000EC64  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 0000EB84 0000EC68  38 21 00 10 */	addi r1, r1, 0x10
/* 0000EB88 0000EC6C  4E 80 00 20 */	blr
.endfn fn_1_EADC

# .text:0x788 | 0xEB8C | size: 0x9C
.fn fn_1_EB8C, global
/* 0000EB8C 0000EC70  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000EB90 0000EC74  7C 67 23 79 */	or. r7, r3, r4
/* 0000EB94 0000EC78  38 C0 00 00 */	li r6, 0x0
/* 0000EB98 0000EC7C  41 82 00 7C */	beq .L_0000EC14
/* 0000EB9C 0000EC80  7C 67 00 34 */	cntlzw r7, r3
/* 0000EBA0 0000EC84  7C 88 00 34 */	cntlzw r8, r4
/* 0000EBA4 0000EC88  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 0000EBA8 0000EC8C  7D 29 FE 70 */	srawi r9, r9, 31
/* 0000EBAC 0000EC90  7D 29 40 38 */	and r9, r9, r8
/* 0000EBB0 0000EC94  7C E7 4A 14 */	add r7, r7, r9
/* 0000EBB4 0000EC98  21 07 00 20 */	subfic r8, r7, 0x20
/* 0000EBB8 0000EC9C  31 27 FF E0 */	subic r9, r7, 0x20
/* 0000EBBC 0000ECA0  7C 63 38 30 */	slw r3, r3, r7
/* 0000EBC0 0000ECA4  7C 8A 44 30 */	srw r10, r4, r8
/* 0000EBC4 0000ECA8  7C 63 53 78 */	or r3, r3, r10
/* 0000EBC8 0000ECAC  7C 8A 48 30 */	slw r10, r4, r9
/* 0000EBCC 0000ECB0  7C 63 53 78 */	or r3, r3, r10
/* 0000EBD0 0000ECB4  7C 84 38 30 */	slw r4, r4, r7
/* 0000EBD4 0000ECB8  7C C7 30 50 */	subf r6, r7, r6
/* 0000EBD8 0000ECBC  54 87 05 7E */	clrlwi r7, r4, 21
/* 0000EBDC 0000ECC0  2C 07 04 00 */	cmpwi r7, 0x400
/* 0000EBE0 0000ECC4  38 C6 04 3E */	addi r6, r6, 0x43e
/* 0000EBE4 0000ECC8  41 80 00 1C */	blt .L_0000EC00
/* 0000EBE8 0000ECCC  41 81 00 0C */	bgt .L_0000EBF4
/* 0000EBEC 0000ECD0  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 0000EBF0 0000ECD4  41 82 00 10 */	beq .L_0000EC00
.L_0000EBF4:
/* 0000EBF4 0000ECD8  30 84 08 00 */	addic r4, r4, 0x800
/* 0000EBF8 0000ECDC  7C 63 01 94 */	addze r3, r3
/* 0000EBFC 0000ECE0  7C C6 01 94 */	addze r6, r6
.L_0000EC00:
/* 0000EC00 0000ECE4  54 84 A8 3E */	rotrwi r4, r4, 11
/* 0000EC04 0000ECE8  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 0000EC08 0000ECEC  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 0000EC0C 0000ECF0  54 C6 A0 16 */	slwi r6, r6, 20
/* 0000EC10 0000ECF4  7C C3 1B 78 */	or r3, r6, r3
.L_0000EC14:
/* 0000EC14 0000ECF8  90 61 00 08 */	stw r3, 0x8(r1)
/* 0000EC18 0000ECFC  90 81 00 0C */	stw r4, 0xc(r1)
/* 0000EC1C 0000ED00  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 0000EC20 0000ED04  38 21 00 10 */	addi r1, r1, 0x10
/* 0000EC24 0000ED08  4E 80 00 20 */	blr
.endfn fn_1_EB8C

# .text:0x824 | 0xEC28 | size: 0xB4
.fn fn_1_EC28, global
/* 0000EC28 0000ED0C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000EC2C 0000ED10  54 65 00 01 */	clrrwi. r5, r3, 31
/* 0000EC30 0000ED14  41 82 00 0C */	beq .L_0000EC3C
/* 0000EC34 0000ED18  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000EC38 0000ED1C  7C 63 01 90 */	subfze r3, r3
.L_0000EC3C:
/* 0000EC3C 0000ED20  7C 67 23 79 */	or. r7, r3, r4
/* 0000EC40 0000ED24  38 C0 00 00 */	li r6, 0x0
/* 0000EC44 0000ED28  41 82 00 80 */	beq .L_0000ECC4
/* 0000EC48 0000ED2C  7C 67 00 34 */	cntlzw r7, r3
/* 0000EC4C 0000ED30  7C 88 00 34 */	cntlzw r8, r4
/* 0000EC50 0000ED34  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 0000EC54 0000ED38  7D 29 FE 70 */	srawi r9, r9, 31
/* 0000EC58 0000ED3C  7D 29 40 38 */	and r9, r9, r8
/* 0000EC5C 0000ED40  7C E7 4A 14 */	add r7, r7, r9
/* 0000EC60 0000ED44  21 07 00 20 */	subfic r8, r7, 0x20
/* 0000EC64 0000ED48  31 27 FF E0 */	subic r9, r7, 0x20
/* 0000EC68 0000ED4C  7C 63 38 30 */	slw r3, r3, r7
/* 0000EC6C 0000ED50  7C 8A 44 30 */	srw r10, r4, r8
/* 0000EC70 0000ED54  7C 63 53 78 */	or r3, r3, r10
/* 0000EC74 0000ED58  7C 8A 48 30 */	slw r10, r4, r9
/* 0000EC78 0000ED5C  7C 63 53 78 */	or r3, r3, r10
/* 0000EC7C 0000ED60  7C 84 38 30 */	slw r4, r4, r7
/* 0000EC80 0000ED64  7C C7 30 50 */	subf r6, r7, r6
/* 0000EC84 0000ED68  54 87 05 7E */	clrlwi r7, r4, 21
/* 0000EC88 0000ED6C  2C 07 04 00 */	cmpwi r7, 0x400
/* 0000EC8C 0000ED70  38 C6 04 3E */	addi r6, r6, 0x43e
/* 0000EC90 0000ED74  41 80 00 1C */	blt .L_0000ECAC
/* 0000EC94 0000ED78  41 81 00 0C */	bgt .L_0000ECA0
/* 0000EC98 0000ED7C  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 0000EC9C 0000ED80  41 82 00 10 */	beq .L_0000ECAC
.L_0000ECA0:
/* 0000ECA0 0000ED84  30 84 08 00 */	addic r4, r4, 0x800
/* 0000ECA4 0000ED88  7C 63 01 94 */	addze r3, r3
/* 0000ECA8 0000ED8C  7C C6 01 94 */	addze r6, r6
.L_0000ECAC:
/* 0000ECAC 0000ED90  54 84 A8 3E */	rotrwi r4, r4, 11
/* 0000ECB0 0000ED94  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 0000ECB4 0000ED98  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 0000ECB8 0000ED9C  54 C6 A0 16 */	slwi r6, r6, 20
/* 0000ECBC 0000EDA0  7C C3 1B 78 */	or r3, r6, r3
/* 0000ECC0 0000EDA4  7C A3 1B 78 */	or r3, r5, r3
.L_0000ECC4:
/* 0000ECC4 0000EDA8  90 61 00 08 */	stw r3, 0x8(r1)
/* 0000ECC8 0000EDAC  90 81 00 0C */	stw r4, 0xc(r1)
/* 0000ECCC 0000EDB0  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 0000ECD0 0000EDB4  FC 20 08 18 */	frsp f1, f1
/* 0000ECD4 0000EDB8  38 21 00 10 */	addi r1, r1, 0x10
/* 0000ECD8 0000EDBC  4E 80 00 20 */	blr
.endfn fn_1_EC28

# .text:0x8D8 | 0xECDC | size: 0xA0
.fn fn_1_ECDC, global
/* 0000ECDC 0000EDC0  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000ECE0 0000EDC4  7C 67 23 79 */	or. r7, r3, r4
/* 0000ECE4 0000EDC8  38 C0 00 00 */	li r6, 0x0
/* 0000ECE8 0000EDCC  41 82 00 7C */	beq .L_0000ED64
/* 0000ECEC 0000EDD0  7C 67 00 34 */	cntlzw r7, r3
/* 0000ECF0 0000EDD4  7C 88 00 34 */	cntlzw r8, r4
/* 0000ECF4 0000EDD8  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 0000ECF8 0000EDDC  7D 29 FE 70 */	srawi r9, r9, 31
/* 0000ECFC 0000EDE0  7D 29 40 38 */	and r9, r9, r8
/* 0000ED00 0000EDE4  7C E7 4A 14 */	add r7, r7, r9
/* 0000ED04 0000EDE8  21 07 00 20 */	subfic r8, r7, 0x20
/* 0000ED08 0000EDEC  31 27 FF E0 */	subic r9, r7, 0x20
/* 0000ED0C 0000EDF0  7C 63 38 30 */	slw r3, r3, r7
/* 0000ED10 0000EDF4  7C 8A 44 30 */	srw r10, r4, r8
/* 0000ED14 0000EDF8  7C 63 53 78 */	or r3, r3, r10
/* 0000ED18 0000EDFC  7C 8A 48 30 */	slw r10, r4, r9
/* 0000ED1C 0000EE00  7C 63 53 78 */	or r3, r3, r10
/* 0000ED20 0000EE04  7C 84 38 30 */	slw r4, r4, r7
/* 0000ED24 0000EE08  7C C7 30 50 */	subf r6, r7, r6
/* 0000ED28 0000EE0C  54 87 05 7E */	clrlwi r7, r4, 21
/* 0000ED2C 0000EE10  2C 07 04 00 */	cmpwi r7, 0x400
/* 0000ED30 0000EE14  38 C6 04 3E */	addi r6, r6, 0x43e
/* 0000ED34 0000EE18  41 80 00 1C */	blt .L_0000ED50
/* 0000ED38 0000EE1C  41 81 00 0C */	bgt .L_0000ED44
/* 0000ED3C 0000EE20  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 0000ED40 0000EE24  41 82 00 10 */	beq .L_0000ED50
.L_0000ED44:
/* 0000ED44 0000EE28  30 84 08 00 */	addic r4, r4, 0x800
/* 0000ED48 0000EE2C  7C 63 01 94 */	addze r3, r3
/* 0000ED4C 0000EE30  7C C6 01 94 */	addze r6, r6
.L_0000ED50:
/* 0000ED50 0000EE34  54 84 A8 3E */	rotrwi r4, r4, 11
/* 0000ED54 0000EE38  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 0000ED58 0000EE3C  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 0000ED5C 0000EE40  54 C6 A0 16 */	slwi r6, r6, 20
/* 0000ED60 0000EE44  7C C3 1B 78 */	or r3, r6, r3
.L_0000ED64:
/* 0000ED64 0000EE48  90 61 00 08 */	stw r3, 0x8(r1)
/* 0000ED68 0000EE4C  90 81 00 0C */	stw r4, 0xc(r1)
/* 0000ED6C 0000EE50  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 0000ED70 0000EE54  FC 20 08 18 */	frsp f1, f1
/* 0000ED74 0000EE58  38 21 00 10 */	addi r1, r1, 0x10
/* 0000ED78 0000EE5C  4E 80 00 20 */	blr
.endfn fn_1_ECDC

# .text:0x978 | 0xED7C | size: 0xCC
.fn fn_1_ED7C, global
/* 0000ED7C 0000EE60  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 0000ED80 0000EE64  D8 21 00 08 */	stfd f1, 0x8(r1)
/* 0000ED84 0000EE68  80 61 00 08 */	lwz r3, 0x8(r1)
/* 0000ED88 0000EE6C  80 81 00 0C */	lwz r4, 0xc(r1)
/* 0000ED8C 0000EE70  54 65 65 7E */	extrwi r5, r3, 11, 1
/* 0000ED90 0000EE74  28 05 03 FF */	cmplwi r5, 0x3ff
/* 0000ED94 0000EE78  40 80 00 10 */	bge .L_0000EDA4
/* 0000ED98 0000EE7C  38 60 00 00 */	li r3, 0x0
/* 0000ED9C 0000EE80  38 80 00 00 */	li r4, 0x0
/* 0000EDA0 0000EE84  48 00 00 A0 */	b .L_0000EE40
.L_0000EDA4:
/* 0000EDA4 0000EE88  7C 66 1B 78 */	mr r6, r3
/* 0000EDA8 0000EE8C  54 63 03 3E */	clrlwi r3, r3, 12
/* 0000EDAC 0000EE90  64 63 00 10 */	oris r3, r3, 0x10
/* 0000EDB0 0000EE94  38 A5 FB CD */	subi r5, r5, 0x433
/* 0000EDB4 0000EE98  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000EDB8 0000EE9C  40 80 00 2C */	bge .L_0000EDE4
/* 0000EDBC 0000EEA0  7C A5 00 D0 */	neg r5, r5
/* 0000EDC0 0000EEA4  21 05 00 20 */	subfic r8, r5, 0x20
/* 0000EDC4 0000EEA8  31 25 FF E0 */	subic r9, r5, 0x20
/* 0000EDC8 0000EEAC  7C 84 2C 30 */	srw r4, r4, r5
/* 0000EDCC 0000EEB0  7C 6A 40 30 */	slw r10, r3, r8
/* 0000EDD0 0000EEB4  7C 84 53 78 */	or r4, r4, r10
/* 0000EDD4 0000EEB8  7C 6A 4C 30 */	srw r10, r3, r9
/* 0000EDD8 0000EEBC  7C 84 53 78 */	or r4, r4, r10
/* 0000EDDC 0000EEC0  7C 63 2C 30 */	srw r3, r3, r5
/* 0000EDE0 0000EEC4  48 00 00 50 */	b .L_0000EE30
.L_0000EDE4:
/* 0000EDE4 0000EEC8  2C 05 00 0A */	cmpwi r5, 0xa
/* 0000EDE8 0000EECC  40 A1 00 28 */	ble+ .L_0000EE10
/* 0000EDEC 0000EED0  54 C6 00 01 */	clrrwi. r6, r6, 31
/* 0000EDF0 0000EED4  41 82 00 10 */	beq .L_0000EE00
/* 0000EDF4 0000EED8  3C 60 80 00 */	lis r3, 0x8000
/* 0000EDF8 0000EEDC  38 80 00 00 */	li r4, 0x0
/* 0000EDFC 0000EEE0  48 00 00 44 */	b .L_0000EE40
.L_0000EE00:
/* 0000EE00 0000EEE4  3C 60 7F FF */	lis r3, 0x7fff
/* 0000EE04 0000EEE8  60 63 FF FF */	ori r3, r3, 0xffff
/* 0000EE08 0000EEEC  38 80 FF FF */	li r4, -0x1
/* 0000EE0C 0000EEF0  48 00 00 34 */	b .L_0000EE40
.L_0000EE10:
/* 0000EE10 0000EEF4  21 05 00 20 */	subfic r8, r5, 0x20
/* 0000EE14 0000EEF8  31 25 FF E0 */	subic r9, r5, 0x20
/* 0000EE18 0000EEFC  7C 63 28 30 */	slw r3, r3, r5
/* 0000EE1C 0000EF00  7C 8A 44 30 */	srw r10, r4, r8
/* 0000EE20 0000EF04  7C 63 53 78 */	or r3, r3, r10
/* 0000EE24 0000EF08  7C 8A 48 30 */	slw r10, r4, r9
/* 0000EE28 0000EF0C  7C 63 53 78 */	or r3, r3, r10
/* 0000EE2C 0000EF10  7C 84 28 30 */	slw r4, r4, r5
.L_0000EE30:
/* 0000EE30 0000EF14  54 C6 00 01 */	clrrwi. r6, r6, 31
/* 0000EE34 0000EF18  41 82 00 0C */	beq .L_0000EE40
/* 0000EE38 0000EF1C  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000EE3C 0000EF20  7C 63 01 90 */	subfze r3, r3
.L_0000EE40:
/* 0000EE40 0000EF24  38 21 00 10 */	addi r1, r1, 0x10
/* 0000EE44 0000EF28  4E 80 00 20 */	blr
.endfn fn_1_ED7C

# 0x000002D8..0x000002F0 | size: 0x18
.rodata
.balign 8

# .rodata:0x0 | 0x2D8 | size: 0x18
.obj lbl_1_rodata_2D8, global
	.double 0
	.double 4294967296
	.double 2147483648
.endobj lbl_1_rodata_2D8
