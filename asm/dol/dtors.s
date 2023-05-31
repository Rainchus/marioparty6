.include "include/macros.s"

.section .dtors, "wa"  # 0x80216040 - 0x80216060 ; 0x00000020


.global lbl_80216040
lbl_80216040:

	# ROM: 0x213040
	.4byte func_800ED910
	.4byte lbl_800EE85C
	.4byte func_800ED910
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
