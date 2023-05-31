.include "include/macros.s"

.section .ctors, "wa"  # 0x80216020 - 0x80216040 ; 0x00000020


.global lbl_80216020
lbl_80216020:

	# ROM: 0x213020
	.4byte lbl_800EE890
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
