.include "macros.inc"
.file "runtime.c"

# 0x000080BC..0x00008B00 | size: 0xA44
.text
.balign 4

# .text:0x0 | 0x80BC | size: 0x5C
.fn __cvt_fp2unsigned, global
/* 000080BC 000081A0  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 000080C0 000081A4  3C 80 00 00 */	lis r4, __constants@h
/* 000080C4 000081A8  60 84 00 00 */	ori r4, r4, __constants@l
/* 000080C8 000081AC  38 60 00 00 */	li r3, 0x0
/* 000080CC 000081B0  C8 04 00 00 */	lfd f0, 0x0(r4)
/* 000080D0 000081B4  C8 64 00 08 */	lfd f3, 0x8(r4)
/* 000080D4 000081B8  C8 84 00 10 */	lfd f4, 0x10(r4)
/* 000080D8 000081BC  FC 01 00 00 */	fcmpu cr0, f1, f0
/* 000080DC 000081C0  FF 01 18 00 */	fcmpu cr6, f1, f3
/* 000080E0 000081C4  41 80 00 30 */	blt .L_00008110
/* 000080E4 000081C8  38 63 FF FF */	subi r3, r3, 0x1
/* 000080E8 000081CC  40 98 00 28 */	bge cr6, .L_00008110
/* 000080EC 000081D0  FF 81 20 00 */	fcmpu cr7, f1, f4
/* 000080F0 000081D4  FC 40 08 90 */	fmr f2, f1
/* 000080F4 000081D8  41 9C 00 08 */	blt cr7, .L_000080FC
/* 000080F8 000081DC  FC 41 20 28 */	fsub f2, f1, f4
.L_000080FC:
/* 000080FC 000081E0  FC 40 10 1E */	fctiwz f2, f2
/* 00008100 000081E4  D8 41 00 08 */	stfd f2, 0x8(r1)
/* 00008104 000081E8  80 61 00 0C */	lwz r3, 0xc(r1)
/* 00008108 000081EC  41 9C 00 08 */	blt cr7, .L_00008110
/* 0000810C 000081F0  3C 63 80 00 */	addis r3, r3, 0x8000
.L_00008110:
/* 00008110 000081F4  38 21 00 10 */	addi r1, r1, 0x10
/* 00008114 000081F8  4E 80 00 20 */	blr
.endfn __cvt_fp2unsigned

# .text:0x5C | 0x8118 | size: 0x4C
.fn __save_fpr, global
# .text:0x5C | 0x8118 | size: 0x0
.sym _savefpr_14, global
/* 00008118 000081FC  D9 CB FF 70 */	stfd f14, -0x90(r11)
# .text:0x60 | 0x811C | size: 0x0
.sym _savefpr_15, global
/* 0000811C 00008200  D9 EB FF 78 */	stfd f15, -0x88(r11)
# .text:0x64 | 0x8120 | size: 0x0
.sym _savefpr_16, global
/* 00008120 00008204  DA 0B FF 80 */	stfd f16, -0x80(r11)
# .text:0x68 | 0x8124 | size: 0x0
.sym _savefpr_17, global
/* 00008124 00008208  DA 2B FF 88 */	stfd f17, -0x78(r11)
# .text:0x6C | 0x8128 | size: 0x0
.sym _savefpr_18, global
/* 00008128 0000820C  DA 4B FF 90 */	stfd f18, -0x70(r11)
# .text:0x70 | 0x812C | size: 0x0
.sym _savefpr_19, global
/* 0000812C 00008210  DA 6B FF 98 */	stfd f19, -0x68(r11)
# .text:0x74 | 0x8130 | size: 0x0
.sym _savefpr_20, global
/* 00008130 00008214  DA 8B FF A0 */	stfd f20, -0x60(r11)
# .text:0x78 | 0x8134 | size: 0x0
.sym _savefpr_21, global
/* 00008134 00008218  DA AB FF A8 */	stfd f21, -0x58(r11)
# .text:0x7C | 0x8138 | size: 0x0
.sym _savefpr_22, global
/* 00008138 0000821C  DA CB FF B0 */	stfd f22, -0x50(r11)
# .text:0x80 | 0x813C | size: 0x0
.sym _savefpr_23, global
/* 0000813C 00008220  DA EB FF B8 */	stfd f23, -0x48(r11)
# .text:0x84 | 0x8140 | size: 0x0
.sym _savefpr_24, global
/* 00008140 00008224  DB 0B FF C0 */	stfd f24, -0x40(r11)
# .text:0x88 | 0x8144 | size: 0x0
.sym _savefpr_25, global
/* 00008144 00008228  DB 2B FF C8 */	stfd f25, -0x38(r11)
# .text:0x8C | 0x8148 | size: 0x0
.sym _savefpr_26, global
/* 00008148 0000822C  DB 4B FF D0 */	stfd f26, -0x30(r11)
# .text:0x90 | 0x814C | size: 0x0
.sym _savefpr_27, global
/* 0000814C 00008230  DB 6B FF D8 */	stfd f27, -0x28(r11)
# .text:0x94 | 0x8150 | size: 0x0
.sym _savefpr_28, global
/* 00008150 00008234  DB 8B FF E0 */	stfd f28, -0x20(r11)
# .text:0x98 | 0x8154 | size: 0x0
.sym _savefpr_29, global
/* 00008154 00008238  DB AB FF E8 */	stfd f29, -0x18(r11)
# .text:0x9C | 0x8158 | size: 0x0
.sym _savefpr_30, global
/* 00008158 0000823C  DB CB FF F0 */	stfd f30, -0x10(r11)
# .text:0xA0 | 0x815C | size: 0x0
.sym _savefpr_31, global
/* 0000815C 00008240  DB EB FF F8 */	stfd f31, -0x8(r11)
/* 00008160 00008244  4E 80 00 20 */	blr
.endfn __save_fpr

# .text:0xA8 | 0x8164 | size: 0x64
.fn _savevr, global
# .text:0xA8 | 0x8164 | size: 0x0
.sym _savev20, global
/* 00008164 00008248  39 80 FF 40 */	li r12, -0xc0
/* 00008168 0000824C  7E 8C 01 CE */	.4byte 0x7E8C01CE /* invalid */
# .text:0xB0 | 0x816C | size: 0x0
.sym _savev21, global
/* 0000816C 00008250  39 80 FF 50 */	li r12, -0xb0
/* 00008170 00008254  7E AC 01 CE */	.4byte 0x7EAC01CE /* invalid */
# .text:0xB8 | 0x8174 | size: 0x0
.sym _savev22, global
/* 00008174 00008258  39 80 FF 60 */	li r12, -0xa0
/* 00008178 0000825C  7E CC 01 CE */	.4byte 0x7ECC01CE /* invalid */
# .text:0xC0 | 0x817C | size: 0x0
.sym _savev23, global
/* 0000817C 00008260  39 80 FF 70 */	li r12, -0x90
/* 00008180 00008264  7E EC 01 CE */	.4byte 0x7EEC01CE /* invalid */
# .text:0xC8 | 0x8184 | size: 0x0
.sym _savev24, global
/* 00008184 00008268  39 80 FF 80 */	li r12, -0x80
/* 00008188 0000826C  7F 0C 01 CE */	.4byte 0x7F0C01CE /* invalid */
# .text:0xD0 | 0x818C | size: 0x0
.sym _savev25, global
/* 0000818C 00008270  39 80 FF 90 */	li r12, -0x70
/* 00008190 00008274  7F 2C 01 CE */	.4byte 0x7F2C01CE /* invalid */
# .text:0xD8 | 0x8194 | size: 0x0
.sym _savev26, global
/* 00008194 00008278  39 80 FF A0 */	li r12, -0x60
/* 00008198 0000827C  7F 4C 01 CE */	.4byte 0x7F4C01CE /* invalid */
# .text:0xE0 | 0x819C | size: 0x0
.sym _savev27, global
/* 0000819C 00008280  39 80 FF B0 */	li r12, -0x50
/* 000081A0 00008284  7F 6C 01 CE */	.4byte 0x7F6C01CE /* invalid */
# .text:0xE8 | 0x81A4 | size: 0x0
.sym _savev28, global
/* 000081A4 00008288  39 80 FF C0 */	li r12, -0x40
/* 000081A8 0000828C  7F 8C 01 CE */	.4byte 0x7F8C01CE /* invalid */
# .text:0xF0 | 0x81AC | size: 0x0
.sym _savev29, global
/* 000081AC 00008290  39 80 FF D0 */	li r12, -0x30
/* 000081B0 00008294  7F AC 01 CE */	.4byte 0x7FAC01CE /* invalid */
# .text:0xF8 | 0x81B4 | size: 0x0
.sym _savev30, global
/* 000081B4 00008298  39 80 FF E0 */	li r12, -0x20
/* 000081B8 0000829C  7F CC 01 CE */	.4byte 0x7FCC01CE /* invalid */
# .text:0x100 | 0x81BC | size: 0x0
.sym _savev31, global
/* 000081BC 000082A0  39 80 FF F0 */	li r12, -0x10
/* 000081C0 000082A4  7F EC 01 CE */	.4byte 0x7FEC01CE /* invalid */
/* 000081C4 000082A8  4E 80 00 20 */	blr
.endfn _savevr

# .text:0x10C | 0x81C8 | size: 0x4C
.fn __restore_fpr, global
# .text:0x10C | 0x81C8 | size: 0x0
.sym _restfpr_14, global
/* 000081C8 000082AC  C9 CB FF 70 */	lfd f14, -0x90(r11)
# .text:0x110 | 0x81CC | size: 0x0
.sym _restfpr_15, global
/* 000081CC 000082B0  C9 EB FF 78 */	lfd f15, -0x88(r11)
# .text:0x114 | 0x81D0 | size: 0x0
.sym _restfpr_16, global
/* 000081D0 000082B4  CA 0B FF 80 */	lfd f16, -0x80(r11)
# .text:0x118 | 0x81D4 | size: 0x0
.sym _restfpr_17, global
/* 000081D4 000082B8  CA 2B FF 88 */	lfd f17, -0x78(r11)
# .text:0x11C | 0x81D8 | size: 0x0
.sym _restfpr_18, global
/* 000081D8 000082BC  CA 4B FF 90 */	lfd f18, -0x70(r11)
# .text:0x120 | 0x81DC | size: 0x0
.sym _restfpr_19, global
/* 000081DC 000082C0  CA 6B FF 98 */	lfd f19, -0x68(r11)
# .text:0x124 | 0x81E0 | size: 0x0
.sym _restfpr_20, global
/* 000081E0 000082C4  CA 8B FF A0 */	lfd f20, -0x60(r11)
# .text:0x128 | 0x81E4 | size: 0x0
.sym _restfpr_21, global
/* 000081E4 000082C8  CA AB FF A8 */	lfd f21, -0x58(r11)
# .text:0x12C | 0x81E8 | size: 0x0
.sym _restfpr_22, global
/* 000081E8 000082CC  CA CB FF B0 */	lfd f22, -0x50(r11)
# .text:0x130 | 0x81EC | size: 0x0
.sym _restfpr_23, global
/* 000081EC 000082D0  CA EB FF B8 */	lfd f23, -0x48(r11)
# .text:0x134 | 0x81F0 | size: 0x0
.sym _restfpr_24, global
/* 000081F0 000082D4  CB 0B FF C0 */	lfd f24, -0x40(r11)
# .text:0x138 | 0x81F4 | size: 0x0
.sym _restfpr_25, global
/* 000081F4 000082D8  CB 2B FF C8 */	lfd f25, -0x38(r11)
# .text:0x13C | 0x81F8 | size: 0x0
.sym _restfpr_26, global
/* 000081F8 000082DC  CB 4B FF D0 */	lfd f26, -0x30(r11)
# .text:0x140 | 0x81FC | size: 0x0
.sym _restfpr_27, global
/* 000081FC 000082E0  CB 6B FF D8 */	lfd f27, -0x28(r11)
# .text:0x144 | 0x8200 | size: 0x0
.sym _restfpr_28, global
/* 00008200 000082E4  CB 8B FF E0 */	lfd f28, -0x20(r11)
# .text:0x148 | 0x8204 | size: 0x0
.sym _restfpr_29, global
/* 00008204 000082E8  CB AB FF E8 */	lfd f29, -0x18(r11)
# .text:0x14C | 0x8208 | size: 0x0
.sym _restfpr_30, global
/* 00008208 000082EC  CB CB FF F0 */	lfd f30, -0x10(r11)
# .text:0x150 | 0x820C | size: 0x0
.sym _restfpr_31, global
/* 0000820C 000082F0  CB EB FF F8 */	lfd f31, -0x8(r11)
/* 00008210 000082F4  4E 80 00 20 */	blr
.endfn __restore_fpr

# .text:0x158 | 0x8214 | size: 0x4C
.fn __save_gpr, global
# .text:0x158 | 0x8214 | size: 0x0
.sym _savegpr_14, global
/* 00008214 000082F8  91 CB FF B8 */	stw r14, -0x48(r11)
# .text:0x15C | 0x8218 | size: 0x0
.sym _savegpr_15, global
/* 00008218 000082FC  91 EB FF BC */	stw r15, -0x44(r11)
# .text:0x160 | 0x821C | size: 0x0
.sym _savegpr_16, global
/* 0000821C 00008300  92 0B FF C0 */	stw r16, -0x40(r11)
# .text:0x164 | 0x8220 | size: 0x0
.sym _savegpr_17, global
/* 00008220 00008304  92 2B FF C4 */	stw r17, -0x3c(r11)
# .text:0x168 | 0x8224 | size: 0x0
.sym _savegpr_18, global
/* 00008224 00008308  92 4B FF C8 */	stw r18, -0x38(r11)
# .text:0x16C | 0x8228 | size: 0x0
.sym _savegpr_19, global
/* 00008228 0000830C  92 6B FF CC */	stw r19, -0x34(r11)
# .text:0x170 | 0x822C | size: 0x0
.sym _savegpr_20, global
/* 0000822C 00008310  92 8B FF D0 */	stw r20, -0x30(r11)
# .text:0x174 | 0x8230 | size: 0x0
.sym _savegpr_21, global
/* 00008230 00008314  92 AB FF D4 */	stw r21, -0x2c(r11)
# .text:0x178 | 0x8234 | size: 0x0
.sym _savegpr_22, global
/* 00008234 00008318  92 CB FF D8 */	stw r22, -0x28(r11)
# .text:0x17C | 0x8238 | size: 0x0
.sym _savegpr_23, global
/* 00008238 0000831C  92 EB FF DC */	stw r23, -0x24(r11)
# .text:0x180 | 0x823C | size: 0x0
.sym _savegpr_24, global
/* 0000823C 00008320  93 0B FF E0 */	stw r24, -0x20(r11)
# .text:0x184 | 0x8240 | size: 0x0
.sym _savegpr_25, global
/* 00008240 00008324  93 2B FF E4 */	stw r25, -0x1c(r11)
# .text:0x188 | 0x8244 | size: 0x0
.sym _savegpr_26, global
/* 00008244 00008328  93 4B FF E8 */	stw r26, -0x18(r11)
# .text:0x18C | 0x8248 | size: 0x0
.sym _savegpr_27, global
/* 00008248 0000832C  93 6B FF EC */	stw r27, -0x14(r11)
# .text:0x190 | 0x824C | size: 0x0
.sym _savegpr_28, global
/* 0000824C 00008330  93 8B FF F0 */	stw r28, -0x10(r11)
# .text:0x194 | 0x8250 | size: 0x0
.sym _savegpr_29, global
/* 00008250 00008334  93 AB FF F4 */	stw r29, -0xc(r11)
# .text:0x198 | 0x8254 | size: 0x0
.sym _savegpr_30, global
/* 00008254 00008338  93 CB FF F8 */	stw r30, -0x8(r11)
# .text:0x19C | 0x8258 | size: 0x0
.sym _savegpr_31, global
/* 00008258 0000833C  93 EB FF FC */	stw r31, -0x4(r11)
/* 0000825C 00008340  4E 80 00 20 */	blr
.endfn __save_gpr

# .text:0x1A4 | 0x8260 | size: 0x4C
.fn __restore_gpr, global
# .text:0x1A4 | 0x8260 | size: 0x0
.sym _restgpr_14, global
/* 00008260 00008344  81 CB FF B8 */	lwz r14, -0x48(r11)
# .text:0x1A8 | 0x8264 | size: 0x0
.sym _restgpr_15, global
/* 00008264 00008348  81 EB FF BC */	lwz r15, -0x44(r11)
# .text:0x1AC | 0x8268 | size: 0x0
.sym _restgpr_16, global
/* 00008268 0000834C  82 0B FF C0 */	lwz r16, -0x40(r11)
# .text:0x1B0 | 0x826C | size: 0x0
.sym _restgpr_17, global
/* 0000826C 00008350  82 2B FF C4 */	lwz r17, -0x3c(r11)
# .text:0x1B4 | 0x8270 | size: 0x0
.sym _restgpr_18, global
/* 00008270 00008354  82 4B FF C8 */	lwz r18, -0x38(r11)
# .text:0x1B8 | 0x8274 | size: 0x0
.sym _restgpr_19, global
/* 00008274 00008358  82 6B FF CC */	lwz r19, -0x34(r11)
# .text:0x1BC | 0x8278 | size: 0x0
.sym _restgpr_20, global
/* 00008278 0000835C  82 8B FF D0 */	lwz r20, -0x30(r11)
# .text:0x1C0 | 0x827C | size: 0x0
.sym _restgpr_21, global
/* 0000827C 00008360  82 AB FF D4 */	lwz r21, -0x2c(r11)
# .text:0x1C4 | 0x8280 | size: 0x0
.sym _restgpr_22, global
/* 00008280 00008364  82 CB FF D8 */	lwz r22, -0x28(r11)
# .text:0x1C8 | 0x8284 | size: 0x0
.sym _restgpr_23, global
/* 00008284 00008368  82 EB FF DC */	lwz r23, -0x24(r11)
# .text:0x1CC | 0x8288 | size: 0x0
.sym _restgpr_24, global
/* 00008288 0000836C  83 0B FF E0 */	lwz r24, -0x20(r11)
# .text:0x1D0 | 0x828C | size: 0x0
.sym _restgpr_25, global
/* 0000828C 00008370  83 2B FF E4 */	lwz r25, -0x1c(r11)
# .text:0x1D4 | 0x8290 | size: 0x0
.sym _restgpr_26, global
/* 00008290 00008374  83 4B FF E8 */	lwz r26, -0x18(r11)
# .text:0x1D8 | 0x8294 | size: 0x0
.sym _restgpr_27, global
/* 00008294 00008378  83 6B FF EC */	lwz r27, -0x14(r11)
# .text:0x1DC | 0x8298 | size: 0x0
.sym _restgpr_28, global
/* 00008298 0000837C  83 8B FF F0 */	lwz r28, -0x10(r11)
# .text:0x1E0 | 0x829C | size: 0x0
.sym _restgpr_29, global
/* 0000829C 00008380  83 AB FF F4 */	lwz r29, -0xc(r11)
# .text:0x1E4 | 0x82A0 | size: 0x0
.sym _restgpr_30, global
/* 000082A0 00008384  83 CB FF F8 */	lwz r30, -0x8(r11)
# .text:0x1E8 | 0x82A4 | size: 0x0
.sym _restgpr_31, global
/* 000082A4 00008388  83 EB FF FC */	lwz r31, -0x4(r11)
/* 000082A8 0000838C  4E 80 00 20 */	blr
.endfn __restore_gpr

# .text:0x1F0 | 0x82AC | size: 0x64
.fn _restorevr, global
# .text:0x1F0 | 0x82AC | size: 0x0
.sym _restv20, global
/* 000082AC 00008390  39 80 FF 40 */	li r12, -0xc0
/* 000082B0 00008394  7E 8C 00 CE */	.4byte 0x7E8C00CE /* invalid */
# .text:0x1F8 | 0x82B4 | size: 0x0
.sym _restv21, global
/* 000082B4 00008398  39 80 FF 50 */	li r12, -0xb0
/* 000082B8 0000839C  7E AC 00 CE */	.4byte 0x7EAC00CE /* invalid */
# .text:0x200 | 0x82BC | size: 0x0
.sym _restv22, global
/* 000082BC 000083A0  39 80 FF 60 */	li r12, -0xa0
/* 000082C0 000083A4  7E CC 00 CE */	.4byte 0x7ECC00CE /* invalid */
# .text:0x208 | 0x82C4 | size: 0x0
.sym _restv23, global
/* 000082C4 000083A8  39 80 FF 70 */	li r12, -0x90
/* 000082C8 000083AC  7E EC 00 CE */	.4byte 0x7EEC00CE /* invalid */
# .text:0x210 | 0x82CC | size: 0x0
.sym _restv24, global
/* 000082CC 000083B0  39 80 FF 80 */	li r12, -0x80
/* 000082D0 000083B4  7F 0C 00 CE */	.4byte 0x7F0C00CE /* invalid */
# .text:0x218 | 0x82D4 | size: 0x0
.sym _restv25, global
/* 000082D4 000083B8  39 80 FF 90 */	li r12, -0x70
/* 000082D8 000083BC  7F 2C 00 CE */	.4byte 0x7F2C00CE /* invalid */
# .text:0x220 | 0x82DC | size: 0x0
.sym _restv26, global
/* 000082DC 000083C0  39 80 FF A0 */	li r12, -0x60
/* 000082E0 000083C4  7F 4C 00 CE */	.4byte 0x7F4C00CE /* invalid */
# .text:0x228 | 0x82E4 | size: 0x0
.sym _restv27, global
/* 000082E4 000083C8  39 80 FF B0 */	li r12, -0x50
/* 000082E8 000083CC  7F 6C 00 CE */	.4byte 0x7F6C00CE /* invalid */
# .text:0x230 | 0x82EC | size: 0x0
.sym _restv28, global
/* 000082EC 000083D0  39 80 FF C0 */	li r12, -0x40
/* 000082F0 000083D4  7F 8C 00 CE */	.4byte 0x7F8C00CE /* invalid */
# .text:0x238 | 0x82F4 | size: 0x0
.sym _restv29, global
/* 000082F4 000083D8  39 80 FF D0 */	li r12, -0x30
/* 000082F8 000083DC  7F AC 00 CE */	.4byte 0x7FAC00CE /* invalid */
# .text:0x240 | 0x82FC | size: 0x0
.sym _restv30, global
/* 000082FC 000083E0  39 80 FF E0 */	li r12, -0x20
/* 00008300 000083E4  7F CC 00 CE */	.4byte 0x7FCC00CE /* invalid */
# .text:0x248 | 0x8304 | size: 0x0
.sym _restv31, global
/* 00008304 000083E8  39 80 FF F0 */	li r12, -0x10
/* 00008308 000083EC  7F EC 00 CE */	.4byte 0x7FEC00CE /* invalid */
/* 0000830C 000083F0  4E 80 00 20 */	blr
.endfn _restorevr

# .text:0x254 | 0x8310 | size: 0xEC
.fn __div2u, global
/* 00008310 000083F4  2C 03 00 00 */	cmpwi r3, 0x0
/* 00008314 000083F8  7C 60 00 34 */	cntlzw r0, r3
/* 00008318 000083FC  7C 89 00 34 */	cntlzw r9, r4
/* 0000831C 00008400  40 82 00 08 */	bne .L_00008324
/* 00008320 00008404  38 09 00 20 */	addi r0, r9, 0x20
.L_00008324:
/* 00008324 00008408  2C 05 00 00 */	cmpwi r5, 0x0
/* 00008328 0000840C  7C A9 00 34 */	cntlzw r9, r5
/* 0000832C 00008410  7C CA 00 34 */	cntlzw r10, r6
/* 00008330 00008414  40 82 00 08 */	bne .L_00008338
/* 00008334 00008418  39 2A 00 20 */	addi r9, r10, 0x20
.L_00008338:
/* 00008338 0000841C  7C 00 48 00 */	cmpw r0, r9
/* 0000833C 00008420  21 40 00 40 */	subfic r10, r0, 0x40
/* 00008340 00008424  41 81 00 B0 */	bgt .L_000083F0
/* 00008344 00008428  39 29 00 01 */	addi r9, r9, 0x1
/* 00008348 0000842C  21 29 00 40 */	subfic r9, r9, 0x40
/* 0000834C 00008430  7C 00 4A 14 */	add r0, r0, r9
/* 00008350 00008434  7D 29 50 50 */	subf r9, r9, r10
/* 00008354 00008438  7D 29 03 A6 */	mtctr r9
/* 00008358 0000843C  2C 09 00 20 */	cmpwi r9, 0x20
/* 0000835C 00008440  38 E9 FF E0 */	subi r7, r9, 0x20
/* 00008360 00008444  41 80 00 10 */	blt .L_00008370
/* 00008364 00008448  7C 68 3C 30 */	srw r8, r3, r7
/* 00008368 0000844C  38 E0 00 00 */	li r7, 0x0
/* 0000836C 00008450  48 00 00 18 */	b .L_00008384
.L_00008370:
/* 00008370 00008454  7C 88 4C 30 */	srw r8, r4, r9
/* 00008374 00008458  20 E9 00 20 */	subfic r7, r9, 0x20
/* 00008378 0000845C  7C 67 38 30 */	slw r7, r3, r7
/* 0000837C 00008460  7D 08 3B 78 */	or r8, r8, r7
/* 00008380 00008464  7C 67 4C 30 */	srw r7, r3, r9
.L_00008384:
/* 00008384 00008468  2C 00 00 20 */	cmpwi r0, 0x20
/* 00008388 0000846C  31 20 FF E0 */	subic r9, r0, 0x20
/* 0000838C 00008470  41 80 00 10 */	blt .L_0000839C
/* 00008390 00008474  7C 83 48 30 */	slw r3, r4, r9
/* 00008394 00008478  38 80 00 00 */	li r4, 0x0
/* 00008398 0000847C  48 00 00 18 */	b .L_000083B0
.L_0000839C:
/* 0000839C 00008480  7C 63 00 30 */	slw r3, r3, r0
/* 000083A0 00008484  21 20 00 20 */	subfic r9, r0, 0x20
/* 000083A4 00008488  7C 89 4C 30 */	srw r9, r4, r9
/* 000083A8 0000848C  7C 63 4B 78 */	or r3, r3, r9
/* 000083AC 00008490  7C 84 00 30 */	slw r4, r4, r0
.L_000083B0:
/* 000083B0 00008494  39 40 FF FF */	li r10, -0x1
/* 000083B4 00008498  30 E7 00 00 */	addic r7, r7, 0x0
.L_000083B8:
/* 000083B8 0000849C  7C 84 21 14 */	adde r4, r4, r4
/* 000083BC 000084A0  7C 63 19 14 */	adde r3, r3, r3
/* 000083C0 000084A4  7D 08 41 14 */	adde r8, r8, r8
/* 000083C4 000084A8  7C E7 39 14 */	adde r7, r7, r7
/* 000083C8 000084AC  7C 06 40 10 */	subfc r0, r6, r8
/* 000083CC 000084B0  7D 25 39 11 */	subfe. r9, r5, r7
/* 000083D0 000084B4  41 80 00 10 */	blt .L_000083E0
/* 000083D4 000084B8  7C 08 03 78 */	mr r8, r0
/* 000083D8 000084BC  7D 27 4B 78 */	mr r7, r9
/* 000083DC 000084C0  30 0A 00 01 */	addic r0, r10, 0x1
.L_000083E0:
/* 000083E0 000084C4  42 00 FF D8 */	bdnz .L_000083B8
/* 000083E4 000084C8  7C 84 21 14 */	adde r4, r4, r4
/* 000083E8 000084CC  7C 63 19 14 */	adde r3, r3, r3
/* 000083EC 000084D0  4E 80 00 20 */	blr
.L_000083F0:
/* 000083F0 000084D4  38 80 00 00 */	li r4, 0x0
/* 000083F4 000084D8  38 60 00 00 */	li r3, 0x0
/* 000083F8 000084DC  4E 80 00 20 */	blr
.endfn __div2u

# .text:0x340 | 0x83FC | size: 0x138
.fn __div2i, global
/* 000083FC 000084E0  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 00008400 000084E4  54 69 00 01 */	clrrwi. r9, r3, 31
/* 00008404 000084E8  41 82 00 0C */	beq .L_00008410
/* 00008408 000084EC  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000840C 000084F0  7C 63 01 90 */	subfze r3, r3
.L_00008410:
/* 00008410 000084F4  91 21 00 08 */	stw r9, 0x8(r1)
/* 00008414 000084F8  54 AA 00 01 */	clrrwi. r10, r5, 31
/* 00008418 000084FC  41 82 00 0C */	beq .L_00008424
/* 0000841C 00008500  20 C6 00 00 */	subfic r6, r6, 0x0
/* 00008420 00008504  7C A5 01 90 */	subfze r5, r5
.L_00008424:
/* 00008424 00008508  91 41 00 0C */	stw r10, 0xc(r1)
/* 00008428 0000850C  2C 03 00 00 */	cmpwi r3, 0x0
/* 0000842C 00008510  7C 60 00 34 */	cntlzw r0, r3
/* 00008430 00008514  7C 89 00 34 */	cntlzw r9, r4
/* 00008434 00008518  40 82 00 08 */	bne .L_0000843C
/* 00008438 0000851C  38 09 00 20 */	addi r0, r9, 0x20
.L_0000843C:
/* 0000843C 00008520  2C 05 00 00 */	cmpwi r5, 0x0
/* 00008440 00008524  7C A9 00 34 */	cntlzw r9, r5
/* 00008444 00008528  7C CA 00 34 */	cntlzw r10, r6
/* 00008448 0000852C  40 82 00 08 */	bne .L_00008450
/* 0000844C 00008530  39 2A 00 20 */	addi r9, r10, 0x20
.L_00008450:
/* 00008450 00008534  7C 00 48 00 */	cmpw r0, r9
/* 00008454 00008538  21 40 00 40 */	subfic r10, r0, 0x40
/* 00008458 0000853C  41 81 00 CC */	bgt .L_00008524
/* 0000845C 00008540  39 29 00 01 */	addi r9, r9, 0x1
/* 00008460 00008544  21 29 00 40 */	subfic r9, r9, 0x40
/* 00008464 00008548  7C 00 4A 14 */	add r0, r0, r9
/* 00008468 0000854C  7D 29 50 50 */	subf r9, r9, r10
/* 0000846C 00008550  7D 29 03 A6 */	mtctr r9
/* 00008470 00008554  2C 09 00 20 */	cmpwi r9, 0x20
/* 00008474 00008558  38 E9 FF E0 */	subi r7, r9, 0x20
/* 00008478 0000855C  41 80 00 10 */	blt .L_00008488
/* 0000847C 00008560  7C 68 3C 30 */	srw r8, r3, r7
/* 00008480 00008564  38 E0 00 00 */	li r7, 0x0
/* 00008484 00008568  48 00 00 18 */	b .L_0000849C
.L_00008488:
/* 00008488 0000856C  7C 88 4C 30 */	srw r8, r4, r9
/* 0000848C 00008570  20 E9 00 20 */	subfic r7, r9, 0x20
/* 00008490 00008574  7C 67 38 30 */	slw r7, r3, r7
/* 00008494 00008578  7D 08 3B 78 */	or r8, r8, r7
/* 00008498 0000857C  7C 67 4C 30 */	srw r7, r3, r9
.L_0000849C:
/* 0000849C 00008580  2C 00 00 20 */	cmpwi r0, 0x20
/* 000084A0 00008584  31 20 FF E0 */	subic r9, r0, 0x20
/* 000084A4 00008588  41 80 00 10 */	blt .L_000084B4
/* 000084A8 0000858C  7C 83 48 30 */	slw r3, r4, r9
/* 000084AC 00008590  38 80 00 00 */	li r4, 0x0
/* 000084B0 00008594  48 00 00 18 */	b .L_000084C8
.L_000084B4:
/* 000084B4 00008598  7C 63 00 30 */	slw r3, r3, r0
/* 000084B8 0000859C  21 20 00 20 */	subfic r9, r0, 0x20
/* 000084BC 000085A0  7C 89 4C 30 */	srw r9, r4, r9
/* 000084C0 000085A4  7C 63 4B 78 */	or r3, r3, r9
/* 000084C4 000085A8  7C 84 00 30 */	slw r4, r4, r0
.L_000084C8:
/* 000084C8 000085AC  39 40 FF FF */	li r10, -0x1
/* 000084CC 000085B0  30 E7 00 00 */	addic r7, r7, 0x0
.L_000084D0:
/* 000084D0 000085B4  7C 84 21 14 */	adde r4, r4, r4
/* 000084D4 000085B8  7C 63 19 14 */	adde r3, r3, r3
/* 000084D8 000085BC  7D 08 41 14 */	adde r8, r8, r8
/* 000084DC 000085C0  7C E7 39 14 */	adde r7, r7, r7
/* 000084E0 000085C4  7C 06 40 10 */	subfc r0, r6, r8
/* 000084E4 000085C8  7D 25 39 11 */	subfe. r9, r5, r7
/* 000084E8 000085CC  41 80 00 10 */	blt .L_000084F8
/* 000084EC 000085D0  7C 08 03 78 */	mr r8, r0
/* 000084F0 000085D4  7D 27 4B 78 */	mr r7, r9
/* 000084F4 000085D8  30 0A 00 01 */	addic r0, r10, 0x1
.L_000084F8:
/* 000084F8 000085DC  42 00 FF D8 */	bdnz .L_000084D0
/* 000084FC 000085E0  7C 84 21 14 */	adde r4, r4, r4
/* 00008500 000085E4  7C 63 19 14 */	adde r3, r3, r3
/* 00008504 000085E8  81 21 00 08 */	lwz r9, 0x8(r1)
/* 00008508 000085EC  81 41 00 0C */	lwz r10, 0xc(r1)
/* 0000850C 000085F0  7D 27 52 79 */	xor. r7, r9, r10
/* 00008510 000085F4  41 82 00 10 */	beq .L_00008520
/* 00008514 000085F8  2C 09 00 00 */	cmpwi r9, 0x0
/* 00008518 000085FC  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000851C 00008600  7C 63 01 90 */	subfze r3, r3
.L_00008520:
/* 00008520 00008604  48 00 00 0C */	b .L_0000852C
.L_00008524:
/* 00008524 00008608  38 80 00 00 */	li r4, 0x0
/* 00008528 0000860C  38 60 00 00 */	li r3, 0x0
.L_0000852C:
/* 0000852C 00008610  38 21 00 10 */	addi r1, r1, 0x10
/* 00008530 00008614  4E 80 00 20 */	blr
.endfn __div2i

# .text:0x478 | 0x8534 | size: 0xE4
.fn __mod2u, global
/* 00008534 00008618  2C 03 00 00 */	cmpwi r3, 0x0
/* 00008538 0000861C  7C 60 00 34 */	cntlzw r0, r3
/* 0000853C 00008620  7C 89 00 34 */	cntlzw r9, r4
/* 00008540 00008624  40 82 00 08 */	bne .L_00008548
/* 00008544 00008628  38 09 00 20 */	addi r0, r9, 0x20
.L_00008548:
/* 00008548 0000862C  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000854C 00008630  7C A9 00 34 */	cntlzw r9, r5
/* 00008550 00008634  7C CA 00 34 */	cntlzw r10, r6
/* 00008554 00008638  40 82 00 08 */	bne .L_0000855C
/* 00008558 0000863C  39 2A 00 20 */	addi r9, r10, 0x20
.L_0000855C:
/* 0000855C 00008640  7C 00 48 00 */	cmpw r0, r9
/* 00008560 00008644  21 40 00 40 */	subfic r10, r0, 0x40
/* 00008564 00008648  41 81 00 B0 */	bgt .L_00008614
/* 00008568 0000864C  39 29 00 01 */	addi r9, r9, 0x1
/* 0000856C 00008650  21 29 00 40 */	subfic r9, r9, 0x40
/* 00008570 00008654  7C 00 4A 14 */	add r0, r0, r9
/* 00008574 00008658  7D 29 50 50 */	subf r9, r9, r10
/* 00008578 0000865C  7D 29 03 A6 */	mtctr r9
/* 0000857C 00008660  2C 09 00 20 */	cmpwi r9, 0x20
/* 00008580 00008664  38 E9 FF E0 */	subi r7, r9, 0x20
/* 00008584 00008668  41 80 00 10 */	blt .L_00008594
/* 00008588 0000866C  7C 68 3C 30 */	srw r8, r3, r7
/* 0000858C 00008670  38 E0 00 00 */	li r7, 0x0
/* 00008590 00008674  48 00 00 18 */	b .L_000085A8
.L_00008594:
/* 00008594 00008678  7C 88 4C 30 */	srw r8, r4, r9
/* 00008598 0000867C  20 E9 00 20 */	subfic r7, r9, 0x20
/* 0000859C 00008680  7C 67 38 30 */	slw r7, r3, r7
/* 000085A0 00008684  7D 08 3B 78 */	or r8, r8, r7
/* 000085A4 00008688  7C 67 4C 30 */	srw r7, r3, r9
.L_000085A8:
/* 000085A8 0000868C  2C 00 00 20 */	cmpwi r0, 0x20
/* 000085AC 00008690  31 20 FF E0 */	subic r9, r0, 0x20
/* 000085B0 00008694  41 80 00 10 */	blt .L_000085C0
/* 000085B4 00008698  7C 83 48 30 */	slw r3, r4, r9
/* 000085B8 0000869C  38 80 00 00 */	li r4, 0x0
/* 000085BC 000086A0  48 00 00 18 */	b .L_000085D4
.L_000085C0:
/* 000085C0 000086A4  7C 63 00 30 */	slw r3, r3, r0
/* 000085C4 000086A8  21 20 00 20 */	subfic r9, r0, 0x20
/* 000085C8 000086AC  7C 89 4C 30 */	srw r9, r4, r9
/* 000085CC 000086B0  7C 63 4B 78 */	or r3, r3, r9
/* 000085D0 000086B4  7C 84 00 30 */	slw r4, r4, r0
.L_000085D4:
/* 000085D4 000086B8  39 40 FF FF */	li r10, -0x1
/* 000085D8 000086BC  30 E7 00 00 */	addic r7, r7, 0x0
.L_000085DC:
/* 000085DC 000086C0  7C 84 21 14 */	adde r4, r4, r4
/* 000085E0 000086C4  7C 63 19 14 */	adde r3, r3, r3
/* 000085E4 000086C8  7D 08 41 14 */	adde r8, r8, r8
/* 000085E8 000086CC  7C E7 39 14 */	adde r7, r7, r7
/* 000085EC 000086D0  7C 06 40 10 */	subfc r0, r6, r8
/* 000085F0 000086D4  7D 25 39 11 */	subfe. r9, r5, r7
/* 000085F4 000086D8  41 80 00 10 */	blt .L_00008604
/* 000085F8 000086DC  7C 08 03 78 */	mr r8, r0
/* 000085FC 000086E0  7D 27 4B 78 */	mr r7, r9
/* 00008600 000086E4  30 0A 00 01 */	addic r0, r10, 0x1
.L_00008604:
/* 00008604 000086E8  42 00 FF D8 */	bdnz .L_000085DC
/* 00008608 000086EC  7D 04 43 78 */	mr r4, r8
/* 0000860C 000086F0  7C E3 3B 78 */	mr r3, r7
/* 00008610 000086F4  4E 80 00 20 */	blr
.L_00008614:
/* 00008614 000086F8  4E 80 00 20 */	blr
.endfn __mod2u

# .text:0x55C | 0x8618 | size: 0x10C
.fn __mod2i, global
/* 00008618 000086FC  2F 83 00 00 */	cmpwi cr7, r3, 0x0
/* 0000861C 00008700  40 9C 00 0C */	bge cr7, .L_00008628
/* 00008620 00008704  20 84 00 00 */	subfic r4, r4, 0x0
/* 00008624 00008708  7C 63 01 90 */	subfze r3, r3
.L_00008628:
/* 00008628 0000870C  2C 05 00 00 */	cmpwi r5, 0x0
/* 0000862C 00008710  40 80 00 0C */	bge .L_00008638
/* 00008630 00008714  20 C6 00 00 */	subfic r6, r6, 0x0
/* 00008634 00008718  7C A5 01 90 */	subfze r5, r5
.L_00008638:
/* 00008638 0000871C  2C 03 00 00 */	cmpwi r3, 0x0
/* 0000863C 00008720  7C 60 00 34 */	cntlzw r0, r3
/* 00008640 00008724  7C 89 00 34 */	cntlzw r9, r4
/* 00008644 00008728  40 82 00 08 */	bne .L_0000864C
/* 00008648 0000872C  38 09 00 20 */	addi r0, r9, 0x20
.L_0000864C:
/* 0000864C 00008730  2C 05 00 00 */	cmpwi r5, 0x0
/* 00008650 00008734  7C A9 00 34 */	cntlzw r9, r5
/* 00008654 00008738  7C CA 00 34 */	cntlzw r10, r6
/* 00008658 0000873C  40 82 00 08 */	bne .L_00008660
/* 0000865C 00008740  39 2A 00 20 */	addi r9, r10, 0x20
.L_00008660:
/* 00008660 00008744  7C 00 48 00 */	cmpw r0, r9
/* 00008664 00008748  21 40 00 40 */	subfic r10, r0, 0x40
/* 00008668 0000874C  41 81 00 AC */	bgt .L_00008714
/* 0000866C 00008750  39 29 00 01 */	addi r9, r9, 0x1
/* 00008670 00008754  21 29 00 40 */	subfic r9, r9, 0x40
/* 00008674 00008758  7C 00 4A 14 */	add r0, r0, r9
/* 00008678 0000875C  7D 29 50 50 */	subf r9, r9, r10
/* 0000867C 00008760  7D 29 03 A6 */	mtctr r9
/* 00008680 00008764  2C 09 00 20 */	cmpwi r9, 0x20
/* 00008684 00008768  38 E9 FF E0 */	subi r7, r9, 0x20
/* 00008688 0000876C  41 80 00 10 */	blt .L_00008698
/* 0000868C 00008770  7C 68 3C 30 */	srw r8, r3, r7
/* 00008690 00008774  38 E0 00 00 */	li r7, 0x0
/* 00008694 00008778  48 00 00 18 */	b .L_000086AC
.L_00008698:
/* 00008698 0000877C  7C 88 4C 30 */	srw r8, r4, r9
/* 0000869C 00008780  20 E9 00 20 */	subfic r7, r9, 0x20
/* 000086A0 00008784  7C 67 38 30 */	slw r7, r3, r7
/* 000086A4 00008788  7D 08 3B 78 */	or r8, r8, r7
/* 000086A8 0000878C  7C 67 4C 30 */	srw r7, r3, r9
.L_000086AC:
/* 000086AC 00008790  2C 00 00 20 */	cmpwi r0, 0x20
/* 000086B0 00008794  31 20 FF E0 */	subic r9, r0, 0x20
/* 000086B4 00008798  41 80 00 10 */	blt .L_000086C4
/* 000086B8 0000879C  7C 83 48 30 */	slw r3, r4, r9
/* 000086BC 000087A0  38 80 00 00 */	li r4, 0x0
/* 000086C0 000087A4  48 00 00 18 */	b .L_000086D8
.L_000086C4:
/* 000086C4 000087A8  7C 63 00 30 */	slw r3, r3, r0
/* 000086C8 000087AC  21 20 00 20 */	subfic r9, r0, 0x20
/* 000086CC 000087B0  7C 89 4C 30 */	srw r9, r4, r9
/* 000086D0 000087B4  7C 63 4B 78 */	or r3, r3, r9
/* 000086D4 000087B8  7C 84 00 30 */	slw r4, r4, r0
.L_000086D8:
/* 000086D8 000087BC  39 40 FF FF */	li r10, -0x1
/* 000086DC 000087C0  30 E7 00 00 */	addic r7, r7, 0x0
.L_000086E0:
/* 000086E0 000087C4  7C 84 21 14 */	adde r4, r4, r4
/* 000086E4 000087C8  7C 63 19 14 */	adde r3, r3, r3
/* 000086E8 000087CC  7D 08 41 14 */	adde r8, r8, r8
/* 000086EC 000087D0  7C E7 39 14 */	adde r7, r7, r7
/* 000086F0 000087D4  7C 06 40 10 */	subfc r0, r6, r8
/* 000086F4 000087D8  7D 25 39 11 */	subfe. r9, r5, r7
/* 000086F8 000087DC  41 80 00 10 */	blt .L_00008708
/* 000086FC 000087E0  7C 08 03 78 */	mr r8, r0
/* 00008700 000087E4  7D 27 4B 78 */	mr r7, r9
/* 00008704 000087E8  30 0A 00 01 */	addic r0, r10, 0x1
.L_00008708:
/* 00008708 000087EC  42 00 FF D8 */	bdnz .L_000086E0
/* 0000870C 000087F0  7D 04 43 78 */	mr r4, r8
/* 00008710 000087F4  7C E3 3B 78 */	mr r3, r7
.L_00008714:
/* 00008714 000087F8  40 9C 00 0C */	bge cr7, .L_00008720
/* 00008718 000087FC  20 84 00 00 */	subfic r4, r4, 0x0
/* 0000871C 00008800  7C 63 01 90 */	subfze r3, r3
.L_00008720:
/* 00008720 00008804  4E 80 00 20 */	blr
.endfn __mod2i

# .text:0x668 | 0x8724 | size: 0x24
.fn __shl2i, global
/* 00008724 00008808  21 05 00 20 */	subfic r8, r5, 0x20
/* 00008728 0000880C  31 25 FF E0 */	subic r9, r5, 0x20
/* 0000872C 00008810  7C 63 28 30 */	slw r3, r3, r5
/* 00008730 00008814  7C 8A 44 30 */	srw r10, r4, r8
/* 00008734 00008818  7C 63 53 78 */	or r3, r3, r10
/* 00008738 0000881C  7C 8A 48 30 */	slw r10, r4, r9
/* 0000873C 00008820  7C 63 53 78 */	or r3, r3, r10
/* 00008740 00008824  7C 84 28 30 */	slw r4, r4, r5
/* 00008744 00008828  4E 80 00 20 */	blr
.endfn __shl2i

# .text:0x68C | 0x8748 | size: 0x24
.fn __shr2u, global
/* 00008748 0000882C  21 05 00 20 */	subfic r8, r5, 0x20
/* 0000874C 00008830  31 25 FF E0 */	subic r9, r5, 0x20
/* 00008750 00008834  7C 84 2C 30 */	srw r4, r4, r5
/* 00008754 00008838  7C 6A 40 30 */	slw r10, r3, r8
/* 00008758 0000883C  7C 84 53 78 */	or r4, r4, r10
/* 0000875C 00008840  7C 6A 4C 30 */	srw r10, r3, r9
/* 00008760 00008844  7C 84 53 78 */	or r4, r4, r10
/* 00008764 00008848  7C 63 2C 30 */	srw r3, r3, r5
/* 00008768 0000884C  4E 80 00 20 */	blr
.endfn __shr2u

# .text:0x6B0 | 0x876C | size: 0x28
.fn __shr2i, global
/* 0000876C 00008850  21 05 00 20 */	subfic r8, r5, 0x20
/* 00008770 00008854  35 25 FF E0 */	subic. r9, r5, 0x20
/* 00008774 00008858  7C 84 2C 30 */	srw r4, r4, r5
/* 00008778 0000885C  7C 6A 40 30 */	slw r10, r3, r8
/* 0000877C 00008860  7C 84 53 78 */	or r4, r4, r10
/* 00008780 00008864  7C 6A 4E 30 */	sraw r10, r3, r9
/* 00008784 00008868  40 81 00 08 */	ble .L_0000878C
/* 00008788 0000886C  7C 84 53 78 */	or r4, r4, r10
.L_0000878C:
/* 0000878C 00008870  7C 63 2E 30 */	sraw r3, r3, r5
/* 00008790 00008874  4E 80 00 20 */	blr
.endfn __shr2i

# .text:0x6D8 | 0x8794 | size: 0xB0
.fn __cvt_sll_dbl, global
/* 00008794 00008878  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 00008798 0000887C  54 65 00 01 */	clrrwi. r5, r3, 31
/* 0000879C 00008880  41 82 00 0C */	beq .L_000087A8
/* 000087A0 00008884  20 84 00 00 */	subfic r4, r4, 0x0
/* 000087A4 00008888  7C 63 01 90 */	subfze r3, r3
.L_000087A8:
/* 000087A8 0000888C  7C 67 23 79 */	or. r7, r3, r4
/* 000087AC 00008890  38 C0 00 00 */	li r6, 0x0
/* 000087B0 00008894  41 82 00 80 */	beq .L_00008830
/* 000087B4 00008898  7C 67 00 34 */	cntlzw r7, r3
/* 000087B8 0000889C  7C 88 00 34 */	cntlzw r8, r4
/* 000087BC 000088A0  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 000087C0 000088A4  7D 29 FE 70 */	srawi r9, r9, 31
/* 000087C4 000088A8  7D 29 40 38 */	and r9, r9, r8
/* 000087C8 000088AC  7C E7 4A 14 */	add r7, r7, r9
/* 000087CC 000088B0  21 07 00 20 */	subfic r8, r7, 0x20
/* 000087D0 000088B4  31 27 FF E0 */	subic r9, r7, 0x20
/* 000087D4 000088B8  7C 63 38 30 */	slw r3, r3, r7
/* 000087D8 000088BC  7C 8A 44 30 */	srw r10, r4, r8
/* 000087DC 000088C0  7C 63 53 78 */	or r3, r3, r10
/* 000087E0 000088C4  7C 8A 48 30 */	slw r10, r4, r9
/* 000087E4 000088C8  7C 63 53 78 */	or r3, r3, r10
/* 000087E8 000088CC  7C 84 38 30 */	slw r4, r4, r7
/* 000087EC 000088D0  7C C7 30 50 */	subf r6, r7, r6
/* 000087F0 000088D4  54 87 05 7E */	clrlwi r7, r4, 21
/* 000087F4 000088D8  2C 07 04 00 */	cmpwi r7, 0x400
/* 000087F8 000088DC  38 C6 04 3E */	addi r6, r6, 0x43e
/* 000087FC 000088E0  41 80 00 1C */	blt .L_00008818
/* 00008800 000088E4  41 81 00 0C */	bgt .L_0000880C
/* 00008804 000088E8  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 00008808 000088EC  41 82 00 10 */	beq .L_00008818
.L_0000880C:
/* 0000880C 000088F0  30 84 08 00 */	addic r4, r4, 0x800
/* 00008810 000088F4  7C 63 01 94 */	addze r3, r3
/* 00008814 000088F8  7C C6 01 94 */	addze r6, r6
.L_00008818:
/* 00008818 000088FC  54 84 A8 3E */	rotrwi r4, r4, 11
/* 0000881C 00008900  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 00008820 00008904  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 00008824 00008908  54 C6 A0 16 */	slwi r6, r6, 20
/* 00008828 0000890C  7C C3 1B 78 */	or r3, r6, r3
/* 0000882C 00008910  7C A3 1B 78 */	or r3, r5, r3
.L_00008830:
/* 00008830 00008914  90 61 00 08 */	stw r3, 0x8(r1)
/* 00008834 00008918  90 81 00 0C */	stw r4, 0xc(r1)
/* 00008838 0000891C  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 0000883C 00008920  38 21 00 10 */	addi r1, r1, 0x10
/* 00008840 00008924  4E 80 00 20 */	blr
.endfn __cvt_sll_dbl

# .text:0x788 | 0x8844 | size: 0x9C
.fn __cvt_ull_dbl, global
/* 00008844 00008928  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 00008848 0000892C  7C 67 23 79 */	or. r7, r3, r4
/* 0000884C 00008930  38 C0 00 00 */	li r6, 0x0
/* 00008850 00008934  41 82 00 7C */	beq .L_000088CC
/* 00008854 00008938  7C 67 00 34 */	cntlzw r7, r3
/* 00008858 0000893C  7C 88 00 34 */	cntlzw r8, r4
/* 0000885C 00008940  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 00008860 00008944  7D 29 FE 70 */	srawi r9, r9, 31
/* 00008864 00008948  7D 29 40 38 */	and r9, r9, r8
/* 00008868 0000894C  7C E7 4A 14 */	add r7, r7, r9
/* 0000886C 00008950  21 07 00 20 */	subfic r8, r7, 0x20
/* 00008870 00008954  31 27 FF E0 */	subic r9, r7, 0x20
/* 00008874 00008958  7C 63 38 30 */	slw r3, r3, r7
/* 00008878 0000895C  7C 8A 44 30 */	srw r10, r4, r8
/* 0000887C 00008960  7C 63 53 78 */	or r3, r3, r10
/* 00008880 00008964  7C 8A 48 30 */	slw r10, r4, r9
/* 00008884 00008968  7C 63 53 78 */	or r3, r3, r10
/* 00008888 0000896C  7C 84 38 30 */	slw r4, r4, r7
/* 0000888C 00008970  7C C7 30 50 */	subf r6, r7, r6
/* 00008890 00008974  54 87 05 7E */	clrlwi r7, r4, 21
/* 00008894 00008978  2C 07 04 00 */	cmpwi r7, 0x400
/* 00008898 0000897C  38 C6 04 3E */	addi r6, r6, 0x43e
/* 0000889C 00008980  41 80 00 1C */	blt .L_000088B8
/* 000088A0 00008984  41 81 00 0C */	bgt .L_000088AC
/* 000088A4 00008988  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 000088A8 0000898C  41 82 00 10 */	beq .L_000088B8
.L_000088AC:
/* 000088AC 00008990  30 84 08 00 */	addic r4, r4, 0x800
/* 000088B0 00008994  7C 63 01 94 */	addze r3, r3
/* 000088B4 00008998  7C C6 01 94 */	addze r6, r6
.L_000088B8:
/* 000088B8 0000899C  54 84 A8 3E */	rotrwi r4, r4, 11
/* 000088BC 000089A0  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 000088C0 000089A4  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 000088C4 000089A8  54 C6 A0 16 */	slwi r6, r6, 20
/* 000088C8 000089AC  7C C3 1B 78 */	or r3, r6, r3
.L_000088CC:
/* 000088CC 000089B0  90 61 00 08 */	stw r3, 0x8(r1)
/* 000088D0 000089B4  90 81 00 0C */	stw r4, 0xc(r1)
/* 000088D4 000089B8  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 000088D8 000089BC  38 21 00 10 */	addi r1, r1, 0x10
/* 000088DC 000089C0  4E 80 00 20 */	blr
.endfn __cvt_ull_dbl

# .text:0x824 | 0x88E0 | size: 0xB4
.fn __cvt_sll_flt, global
/* 000088E0 000089C4  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 000088E4 000089C8  54 65 00 01 */	clrrwi. r5, r3, 31
/* 000088E8 000089CC  41 82 00 0C */	beq .L_000088F4
/* 000088EC 000089D0  20 84 00 00 */	subfic r4, r4, 0x0
/* 000088F0 000089D4  7C 63 01 90 */	subfze r3, r3
.L_000088F4:
/* 000088F4 000089D8  7C 67 23 79 */	or. r7, r3, r4
/* 000088F8 000089DC  38 C0 00 00 */	li r6, 0x0
/* 000088FC 000089E0  41 82 00 80 */	beq .L_0000897C
/* 00008900 000089E4  7C 67 00 34 */	cntlzw r7, r3
/* 00008904 000089E8  7C 88 00 34 */	cntlzw r8, r4
/* 00008908 000089EC  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 0000890C 000089F0  7D 29 FE 70 */	srawi r9, r9, 31
/* 00008910 000089F4  7D 29 40 38 */	and r9, r9, r8
/* 00008914 000089F8  7C E7 4A 14 */	add r7, r7, r9
/* 00008918 000089FC  21 07 00 20 */	subfic r8, r7, 0x20
/* 0000891C 00008A00  31 27 FF E0 */	subic r9, r7, 0x20
/* 00008920 00008A04  7C 63 38 30 */	slw r3, r3, r7
/* 00008924 00008A08  7C 8A 44 30 */	srw r10, r4, r8
/* 00008928 00008A0C  7C 63 53 78 */	or r3, r3, r10
/* 0000892C 00008A10  7C 8A 48 30 */	slw r10, r4, r9
/* 00008930 00008A14  7C 63 53 78 */	or r3, r3, r10
/* 00008934 00008A18  7C 84 38 30 */	slw r4, r4, r7
/* 00008938 00008A1C  7C C7 30 50 */	subf r6, r7, r6
/* 0000893C 00008A20  54 87 05 7E */	clrlwi r7, r4, 21
/* 00008940 00008A24  2C 07 04 00 */	cmpwi r7, 0x400
/* 00008944 00008A28  38 C6 04 3E */	addi r6, r6, 0x43e
/* 00008948 00008A2C  41 80 00 1C */	blt .L_00008964
/* 0000894C 00008A30  41 81 00 0C */	bgt .L_00008958
/* 00008950 00008A34  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 00008954 00008A38  41 82 00 10 */	beq .L_00008964
.L_00008958:
/* 00008958 00008A3C  30 84 08 00 */	addic r4, r4, 0x800
/* 0000895C 00008A40  7C 63 01 94 */	addze r3, r3
/* 00008960 00008A44  7C C6 01 94 */	addze r6, r6
.L_00008964:
/* 00008964 00008A48  54 84 A8 3E */	rotrwi r4, r4, 11
/* 00008968 00008A4C  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 0000896C 00008A50  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 00008970 00008A54  54 C6 A0 16 */	slwi r6, r6, 20
/* 00008974 00008A58  7C C3 1B 78 */	or r3, r6, r3
/* 00008978 00008A5C  7C A3 1B 78 */	or r3, r5, r3
.L_0000897C:
/* 0000897C 00008A60  90 61 00 08 */	stw r3, 0x8(r1)
/* 00008980 00008A64  90 81 00 0C */	stw r4, 0xc(r1)
/* 00008984 00008A68  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 00008988 00008A6C  FC 20 08 18 */	frsp f1, f1
/* 0000898C 00008A70  38 21 00 10 */	addi r1, r1, 0x10
/* 00008990 00008A74  4E 80 00 20 */	blr
.endfn __cvt_sll_flt

# .text:0x8D8 | 0x8994 | size: 0xA0
.fn __cvt_ull_flt, global
/* 00008994 00008A78  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 00008998 00008A7C  7C 67 23 79 */	or. r7, r3, r4
/* 0000899C 00008A80  38 C0 00 00 */	li r6, 0x0
/* 000089A0 00008A84  41 82 00 7C */	beq .L_00008A1C
/* 000089A4 00008A88  7C 67 00 34 */	cntlzw r7, r3
/* 000089A8 00008A8C  7C 88 00 34 */	cntlzw r8, r4
/* 000089AC 00008A90  54 E9 D0 08 */	extlwi r9, r7, 5, 26
/* 000089B0 00008A94  7D 29 FE 70 */	srawi r9, r9, 31
/* 000089B4 00008A98  7D 29 40 38 */	and r9, r9, r8
/* 000089B8 00008A9C  7C E7 4A 14 */	add r7, r7, r9
/* 000089BC 00008AA0  21 07 00 20 */	subfic r8, r7, 0x20
/* 000089C0 00008AA4  31 27 FF E0 */	subic r9, r7, 0x20
/* 000089C4 00008AA8  7C 63 38 30 */	slw r3, r3, r7
/* 000089C8 00008AAC  7C 8A 44 30 */	srw r10, r4, r8
/* 000089CC 00008AB0  7C 63 53 78 */	or r3, r3, r10
/* 000089D0 00008AB4  7C 8A 48 30 */	slw r10, r4, r9
/* 000089D4 00008AB8  7C 63 53 78 */	or r3, r3, r10
/* 000089D8 00008ABC  7C 84 38 30 */	slw r4, r4, r7
/* 000089DC 00008AC0  7C C7 30 50 */	subf r6, r7, r6
/* 000089E0 00008AC4  54 87 05 7E */	clrlwi r7, r4, 21
/* 000089E4 00008AC8  2C 07 04 00 */	cmpwi r7, 0x400
/* 000089E8 00008ACC  38 C6 04 3E */	addi r6, r6, 0x43e
/* 000089EC 00008AD0  41 80 00 1C */	blt .L_00008A08
/* 000089F0 00008AD4  41 81 00 0C */	bgt .L_000089FC
/* 000089F4 00008AD8  54 87 05 29 */	rlwinm. r7, r4, 0, 20, 20
/* 000089F8 00008ADC  41 82 00 10 */	beq .L_00008A08
.L_000089FC:
/* 000089FC 00008AE0  30 84 08 00 */	addic r4, r4, 0x800
/* 00008A00 00008AE4  7C 63 01 94 */	addze r3, r3
/* 00008A04 00008AE8  7C C6 01 94 */	addze r6, r6
.L_00008A08:
/* 00008A08 00008AEC  54 84 A8 3E */	rotrwi r4, r4, 11
/* 00008A0C 00008AF0  50 64 A8 14 */	rlwimi r4, r3, 21, 0, 10
/* 00008A10 00008AF4  54 63 AB 3E */	extrwi r3, r3, 20, 1
/* 00008A14 00008AF8  54 C6 A0 16 */	slwi r6, r6, 20
/* 00008A18 00008AFC  7C C3 1B 78 */	or r3, r6, r3
.L_00008A1C:
/* 00008A1C 00008B00  90 61 00 08 */	stw r3, 0x8(r1)
/* 00008A20 00008B04  90 81 00 0C */	stw r4, 0xc(r1)
/* 00008A24 00008B08  C8 21 00 08 */	lfd f1, 0x8(r1)
/* 00008A28 00008B0C  FC 20 08 18 */	frsp f1, f1
/* 00008A2C 00008B10  38 21 00 10 */	addi r1, r1, 0x10
/* 00008A30 00008B14  4E 80 00 20 */	blr
.endfn __cvt_ull_flt

# .text:0x978 | 0x8A34 | size: 0xCC
.fn __cvt_dbl_usll, global
/* 00008A34 00008B18  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 00008A38 00008B1C  D8 21 00 08 */	stfd f1, 0x8(r1)
/* 00008A3C 00008B20  80 61 00 08 */	lwz r3, 0x8(r1)
/* 00008A40 00008B24  80 81 00 0C */	lwz r4, 0xc(r1)
/* 00008A44 00008B28  54 65 65 7E */	extrwi r5, r3, 11, 1
/* 00008A48 00008B2C  28 05 03 FF */	cmplwi r5, 0x3ff
/* 00008A4C 00008B30  40 80 00 10 */	bge .L_00008A5C
/* 00008A50 00008B34  38 60 00 00 */	li r3, 0x0
/* 00008A54 00008B38  38 80 00 00 */	li r4, 0x0
/* 00008A58 00008B3C  48 00 00 A0 */	b .L_00008AF8
.L_00008A5C:
/* 00008A5C 00008B40  7C 66 1B 78 */	mr r6, r3
/* 00008A60 00008B44  54 63 03 3E */	clrlwi r3, r3, 12
/* 00008A64 00008B48  64 63 00 10 */	oris r3, r3, 0x10
/* 00008A68 00008B4C  38 A5 FB CD */	subi r5, r5, 0x433
/* 00008A6C 00008B50  2C 05 00 00 */	cmpwi r5, 0x0
/* 00008A70 00008B54  40 80 00 2C */	bge .L_00008A9C
/* 00008A74 00008B58  7C A5 00 D0 */	neg r5, r5
/* 00008A78 00008B5C  21 05 00 20 */	subfic r8, r5, 0x20
/* 00008A7C 00008B60  31 25 FF E0 */	subic r9, r5, 0x20
/* 00008A80 00008B64  7C 84 2C 30 */	srw r4, r4, r5
/* 00008A84 00008B68  7C 6A 40 30 */	slw r10, r3, r8
/* 00008A88 00008B6C  7C 84 53 78 */	or r4, r4, r10
/* 00008A8C 00008B70  7C 6A 4C 30 */	srw r10, r3, r9
/* 00008A90 00008B74  7C 84 53 78 */	or r4, r4, r10
/* 00008A94 00008B78  7C 63 2C 30 */	srw r3, r3, r5
/* 00008A98 00008B7C  48 00 00 50 */	b .L_00008AE8
.L_00008A9C:
/* 00008A9C 00008B80  2C 05 00 0A */	cmpwi r5, 0xa
/* 00008AA0 00008B84  40 A1 00 28 */	ble+ .L_00008AC8
/* 00008AA4 00008B88  54 C6 00 01 */	clrrwi. r6, r6, 31
/* 00008AA8 00008B8C  41 82 00 10 */	beq .L_00008AB8
/* 00008AAC 00008B90  3C 60 80 00 */	lis r3, 0x8000
/* 00008AB0 00008B94  38 80 00 00 */	li r4, 0x0
/* 00008AB4 00008B98  48 00 00 44 */	b .L_00008AF8
.L_00008AB8:
/* 00008AB8 00008B9C  3C 60 7F FF */	lis r3, 0x7fff
/* 00008ABC 00008BA0  60 63 FF FF */	ori r3, r3, 0xffff
/* 00008AC0 00008BA4  38 80 FF FF */	li r4, -0x1
/* 00008AC4 00008BA8  48 00 00 34 */	b .L_00008AF8
.L_00008AC8:
/* 00008AC8 00008BAC  21 05 00 20 */	subfic r8, r5, 0x20
/* 00008ACC 00008BB0  31 25 FF E0 */	subic r9, r5, 0x20
/* 00008AD0 00008BB4  7C 63 28 30 */	slw r3, r3, r5
/* 00008AD4 00008BB8  7C 8A 44 30 */	srw r10, r4, r8
/* 00008AD8 00008BBC  7C 63 53 78 */	or r3, r3, r10
/* 00008ADC 00008BC0  7C 8A 48 30 */	slw r10, r4, r9
/* 00008AE0 00008BC4  7C 63 53 78 */	or r3, r3, r10
/* 00008AE4 00008BC8  7C 84 28 30 */	slw r4, r4, r5
.L_00008AE8:
/* 00008AE8 00008BCC  54 C6 00 01 */	clrrwi. r6, r6, 31
/* 00008AEC 00008BD0  41 82 00 0C */	beq .L_00008AF8
/* 00008AF0 00008BD4  20 84 00 00 */	subfic r4, r4, 0x0
/* 00008AF4 00008BD8  7C 63 01 90 */	subfze r3, r3
.L_00008AF8:
/* 00008AF8 00008BDC  38 21 00 10 */	addi r1, r1, 0x10
/* 00008AFC 00008BE0  4E 80 00 20 */	blr
.endfn __cvt_dbl_usll

# 0x00000290..0x000002A8 | size: 0x18
.rodata
.balign 8

# .rodata:0x0 | 0x290 | size: 0x18
.obj __constants, local
	.double 0
	.double 4294967296
	.double 2147483648
.endobj __constants
