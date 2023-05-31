/*
Sections:
	.init       0x00000100  0x80003100  0x800056C0  0x000025C0
	extab_      0x00212DC0  0x800056C0  0x80005800  0x00000140
	extabindex_ 0x00212F00  0x80005800  0x80005920  0x00000120
	.text       0x000026C0  0x80005920  0x80216020  0x00210700
	.ctors      0x00213020  0x80216020  0x80216040  0x00000020
	.dtors      0x00213040  0x80216040  0x80216060  0x00000020
	.rodata     0x00213060  0x80216060  0x8021B040  0x00004FE0
	.data       0x00218040  0x8021B040  0x8024C7A0  0x00031760
	.bss        0x00000000  0x8024C7A0  0x802BF6E0  0x00072F40
	.sdata      0x002497A0  0x802BF6E0  0x802BFF60  0x00000880
	.sbss       0x00000000  0x802BFF60  0x802C10C0  0x00001160
	.sdata2     0x0024A020  0x802C10C0  0x802C50A0  0x00003FE0
	.sbss2      0x00000000  0x802C50A0  0x802C50AE  0x0000000E
Entry Point: 0x80003154
*/
# PowerPC Register Constants
.set r0, 0
.set r1, 1
.set r2, 2
.set r3, 3
.set r4, 4
.set r5, 5
.set r6, 6
.set r7, 7
.set r8, 8
.set r9, 9
.set r10, 10
.set r11, 11
.set r12, 12
.set r13, 13
.set r14, 14
.set r15, 15
.set r16, 16
.set r17, 17
.set r18, 18
.set r19, 19
.set r20, 20
.set r21, 21
.set r22, 22
.set r23, 23
.set r24, 24
.set r25, 25
.set r26, 26
.set r27, 27
.set r28, 28
.set r29, 29
.set r30, 30
.set r31, 31
.set f0, 0
.set f1, 1
.set f2, 2
.set f3, 3
.set f4, 4
.set f5, 5
.set f6, 6
.set f7, 7
.set f8, 8
.set f9, 9
.set f10, 10
.set f11, 11
.set f12, 12
.set f13, 13
.set f14, 14
.set f15, 15
.set f16, 16
.set f17, 17
.set f18, 18
.set f19, 19
.set f20, 20
.set f21, 21
.set f22, 22
.set f23, 23
.set f24, 24
.set f25, 25
.set f26, 26
.set f27, 27
.set f28, 28
.set f29, 29
.set f30, 30
.set f31, 31
.set qr0, 0
.set qr1, 1
.set qr2, 2
.set qr3, 3
.set qr4, 4
.set qr5, 5
.set qr6, 6
.set qr7, 7
# Small Data Area (read/write) Base
.set _SDA_BASE_, 0x802C76E0
# Small Data Area (read only) Base
.set _SDA2_BASE_, 0x802C90C0

