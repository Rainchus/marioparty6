.include "include/macros.s"

.section .sdata, "wa"  # 0x802BF6E0 - 0x802BFF60 ; 0x00000880


.global lbl_802BF6E0
lbl_802BF6E0:

	# ROM: 0x2497A0
	.asciz "USR0"

.global lbl_802BF6E5
lbl_802BF6E5:

	# ROM: 0x2497A5
	.byte 0x55, 0x53, 0x52
	.4byte 0x31000000

.global lbl_802BF6EC
lbl_802BF6EC:

	# ROM: 0x2497AC
	.4byte 0x0000D9ED

.global lbl_802BF6F0
lbl_802BF6F0:

	# ROM: 0x2497B0
	.4byte 0xFFFFFFFF
	.4byte 0

.global lbl_802BF6F8
lbl_802BF6F8:

	# ROM: 0x2497B8
	.asciz "dvd.c"

.global lbl_802BF6FE
lbl_802BF6FE:

	# ROM: 0x2497BE
	.2byte 0x0A00

.global lbl_802BF700
lbl_802BF700:

	# ROM: 0x2497C0
	.asciz "data.c"

.global lbl_802BF707
lbl_802BF707:

	# ROM: 0x2497C7
	.byte 0x0A
	.4byte 0
	.4byte 0

.global lbl_802BF710
lbl_802BF710:

	# ROM: 0x2497D0
	.asciz "1.1.4"
	.balign 4

.global lbl_802BF718
lbl_802BF718:

	# ROM: 0x2497D8
	.4byte 0x00000001

.global lbl_802BF71C
lbl_802BF71C:

	# ROM: 0x2497DC
	.asciz "init.c"

.global lbl_802BF723
lbl_802BF723:

	# ROM: 0x2497E3
	.byte 0x6C
	.asciz "oop\n"
	.balign 4
	.4byte 0

.global lbl_802BF730
lbl_802BF730:

	# ROM: 0x2497F0
	.2byte 0x0A00

.global lbl_802BF732
lbl_802BF732:

	# ROM: 0x2497F2
	.byte 0x42, 0x4D
	.asciz "P%d:\n"
	.balign 4
	.4byte 0

.global lbl_802BF740
lbl_802BF740:

	# ROM: 0x249800
	.2byte 0x0001

.global lbl_802BF742
lbl_802BF742:

	# ROM: 0x249802
	.byte 0xFF, 0xFF
	.byte 0x00, 0x00

.global lbl_802BF746
lbl_802BF746:

	# ROM: 0x249806
	.balign 4
	.byte 0xFF, 0xFF

.global lbl_802BF74A
lbl_802BF74A:

	# ROM: 0x24980A
	.byte 0x45, 0x72
	.asciz "ror\n"
	.balign 4
	.4byte 0

.global lbl_802BF758
lbl_802BF758:

	# ROM: 0x249818
	.4byte 0x0A000000
	.4byte 0

.global lbl_802BF760
lbl_802BF760:

	# ROM: 0x249820
	.4byte 0x43505500

.global lbl_802BF764
lbl_802BF764:

	# ROM: 0x249824
	.asciz "DRAW"
	.balign 4
	.4byte 0

.global lbl_802BF770
lbl_802BF770:

	# ROM: 0x249830
	.4byte 0x01000000

.global lbl_802BF774
lbl_802BF774:

	# ROM: 0x249834
	.4byte 0xFFFFFFFF

.global lbl_802BF778
lbl_802BF778:

	# ROM: 0x249838
	.4byte lbl_8022B587

.global lbl_802BF77C
lbl_802BF77C:

	# ROM: 0x24983C
	.asciz "%s:%d"
	.balign 4
	.4byte 0

.global func_802BF788
func_802BF788:

	# ROM: 0x249848
	.4byte 0x0001FFFF
	.4byte 0

.global func_802BF790
func_802BF790:

	# ROM: 0x249850
	.4byte 0xFFFFFFFF

.global lbl_802BF794
lbl_802BF794:

	# ROM: 0x249854
	.4byte 0x213F2E00

.global lbl_802BF798
lbl_802BF798:

	# ROM: 0x249858
	.4byte 0x03E80000

.global lbl_802BF79C
lbl_802BF79C:

	# ROM: 0x24985C
	.4byte 0

.global lbl_802BF7A0
lbl_802BF7A0:

	# ROM: 0x249860
	.asciz "star"

.global lbl_802BF7A5
lbl_802BF7A5:

	# ROM: 0x249865
	.byte 0x63, 0x6F, 0x69
	.byte 0x6E, 0x00

.global lbl_802BF7AA
lbl_802BF7AA:

	# ROM: 0x24986A
	.byte 0x53, 0x74
	.byte 0x61, 0x72, 0x00

.global lbl_802BF7AF
lbl_802BF7AF:

	# ROM: 0x24986F
	.byte 0x43
	.4byte 0x6F696E00

.global lbl_802BF7B4
lbl_802BF7B4:

	# ROM: 0x249874
	.4byte 0x6F726200

.global lbl_802BF7B8
lbl_802BF7B8:

	# ROM: 0x249878
	.4byte 0x4F726200

.global lbl_802BF7BC
lbl_802BF7BC:

	# ROM: 0x24987C
	.asciz "point"

.global lbl_802BF7C2
lbl_802BF7C2:

	# ROM: 0x249882
	.byte 0x50, 0x6F
	.4byte 0x696E7400

.global lbl_802BF7C8
lbl_802BF7C8:

	# ROM: 0x249888
	.asciz "space"

.global lbl_802BF7CE
lbl_802BF7CE:

	# ROM: 0x24988E
	.byte 0x53, 0x70
	.4byte 0x61636500

.global lbl_802BF7D4
lbl_802BF7D4:

	# ROM: 0x249894
	.asciz "stars"

.global lbl_802BF7DA
lbl_802BF7DA:

	# ROM: 0x24989A
	.byte 0x63, 0x6F
	.4byte 0x696E7300

.global lbl_802BF7E0
lbl_802BF7E0:

	# ROM: 0x2498A0
	.asciz "Stars"

.global lbl_802BF7E6
lbl_802BF7E6:

	# ROM: 0x2498A6
	.byte 0x43, 0x6F
	.4byte 0x696E7300

.global lbl_802BF7EC
lbl_802BF7EC:

	# ROM: 0x2498AC
	.asciz "orbs"

.global lbl_802BF7F1
lbl_802BF7F1:

	# ROM: 0x2498B1
	.byte 0x4F, 0x72, 0x62
	.byte 0x73, 0x00

.global lbl_802BF7F6
lbl_802BF7F6:

	# ROM: 0x2498B6
	.byte 0x70, 0x6F
	.asciz "ints"

.global lbl_802BF7FD
lbl_802BF7FD:

	# ROM: 0x2498BD
	.byte 0x50, 0x6F, 0x69
	.4byte 0x6E747300

.global lbl_802BF804
lbl_802BF804:

	# ROM: 0x2498C4
	.asciz "spaces"

.global lbl_802BF80B
lbl_802BF80B:

	# ROM: 0x2498CB
	.byte 0x53
	.asciz "paces"
	.balign 4
	.4byte 0

.global lbl_802BF818
lbl_802BF818:

	# ROM: 0x2498D8
	.asciz "hair1"

.global lbl_802BF81E
lbl_802BF81E:

	# ROM: 0x2498DE
	.byte 0x00

.global lbl_802BF81F
lbl_802BF81F:

	# ROM: 0x2498DF
	.byte 0x03
	.4byte 0x37E7E7E7
	.4byte 0xE7E7E700

.global lbl_802BF828
lbl_802BF828:

	# ROM: 0x2498E8
	.4byte 0x00B100B1
	.4byte 0x00B100B1

.global lbl_802BF830
lbl_802BF830:

	# ROM: 0x2498F0
	.4byte 0x0411E7E7
	.4byte 0xE7E7E7E7

.global lbl_802BF838
lbl_802BF838:

	# ROM: 0x2498F8
	.4byte 0x00B500B5
	.4byte 0x00B500B5

.global lbl_802BF840
lbl_802BF840:

	# ROM: 0x249900
	.4byte 0x0117E7E7
	.4byte 0xE7E7E7E7

.global lbl_802BF848
lbl_802BF848:

	# ROM: 0x249908
	.4byte 0x00B100B1
	.4byte 0x00B100B1

.global lbl_802BF850
lbl_802BF850:

	# ROM: 0x249910
	.4byte 0x54485000
	.4byte 0

.global lbl_802BF858
lbl_802BF858:

	# ROM: 0x249918
	.byte 0x00

.global lbl_802BF859
lbl_802BF859:

	# ROM: 0x249919
	.byte 0x25, 0x64, 0x0A
	.4byte 0

.global func_802BF860
func_802BF860:

	# ROM: 0x249920
	.4byte 0xFFD815FF

.global func_802BF864
func_802BF864:

	# ROM: 0x249924
	.4byte 0xFFFFFFFF

.global func_802BF868
func_802BF868:

	# ROM: 0x249928
	.4byte 0x42FF7AFF
	.4byte 0

.global lbl_802BF870
lbl_802BF870:

	# ROM: 0x249930
	.4byte 0x00000001

.global lbl_802BF874
lbl_802BF874:

	# ROM: 0x249934
	.4byte 0xFFFFFFFF

.global lbl_802BF878
lbl_802BF878:

	# ROM: 0x249938
	.asciz "MARIPA6"

.global lbl_802BF880
lbl_802BF880:

	# ROM: 0x249940
	.asciz "ERASE"

.global lbl_802BF886
lbl_802BF886:

	# ROM: 0x249946
	.2byte 0xFFFF

.global lbl_802BF888
lbl_802BF888:

	# ROM: 0x249948
	.asciz "EMPT"

.global lbl_802BF88D
lbl_802BF88D:

	# ROM: 0x24994D
	.byte 0x53, 0x41, 0x56
	.byte 0x45, 0x00

.global lbl_802BF892
lbl_802BF892:

	# ROM: 0x249952
	.byte 0x25, 0x64
	.4byte 0

.global lbl_802BF898
lbl_802BF898:

	# ROM: 0x249958
	.4byte 0
	.4byte 0

.global lbl_802BF8A0
lbl_802BF8A0:

	# ROM: 0x249960
	.4byte 0xFF000000

.global lbl_802BF8A4
lbl_802BF8A4:

	# ROM: 0x249964
	.4byte 0
	.4byte 0
	.4byte 0

.global lbl_802BF8B0
lbl_802BF8B0:

	# ROM: 0x249970
	.4byte lbl_8006EE88
	.4byte lbl_8006EF20

.global lbl_802BF8B8
lbl_802BF8B8:

	# ROM: 0x249978
	.4byte lbl_8007039C
	.4byte 0

.global lbl_802BF8C0
lbl_802BF8C0:

	# ROM: 0x249980
	.4byte 0xCFD8B500

.global lbl_802BF8C4
lbl_802BF8C4:

	# ROM: 0x249984
	.asciz "MARIO"

.global lbl_802BF8CA
lbl_802BF8CA:

	# ROM: 0x24998A
	.byte 0xD9, 0xB2
	.4byte 0xB0BCDE00

.global lbl_802BF8D0
lbl_802BF8D0:

	# ROM: 0x249990
	.asciz "LUIGI"

.global lbl_802BF8D6
lbl_802BF8D6:

	# ROM: 0x249996
	.byte 0xCB, 0xDF
	.byte 0xB0, 0xC1, 0x00

.global lbl_802BF8DB
lbl_802BF8DB:

	# ROM: 0x24999B
	.byte 0x50
	.asciz "EACH"

.global lbl_802BF8E1
lbl_802BF8E1:

	# ROM: 0x2499A1
	.byte 0xD6, 0xAF, 0xBC
	.byte 0xB0, 0x00

.global lbl_802BF8E6
lbl_802BF8E6:

	# ROM: 0x2499A6
	.byte 0x59, 0x4F
	.4byte 0x53484900

.global lbl_802BF8EC
lbl_802BF8EC:

	# ROM: 0x2499AC
	.4byte 0xDCD8B500

.global lbl_802BF8F0
lbl_802BF8F0:

	# ROM: 0x2499B0
	.asciz "WARIO"

.global lbl_802BF8F6
lbl_802BF8F6:

	# ROM: 0x2499B6
	.byte 0xC3, 0xDE
	.4byte 0xB2BCDEB0
	.byte 0x00

.global lbl_802BF8FD
lbl_802BF8FD:

	# ROM: 0x2499BD
	.byte 0x44, 0x41, 0x49
	.byte 0x53, 0x59, 0x00

.global lbl_802BF903
lbl_802BF903:

	# ROM: 0x2499C3
	.byte 0xDC
	.4byte 0xD9B2B0BC
	.byte 0xDE, 0x00

.global lbl_802BF90A
lbl_802BF90A:

	# ROM: 0x2499CA
	.byte 0x57, 0x41
	.asciz "LUIGI"

.global lbl_802BF912
lbl_802BF912:

	# ROM: 0x2499D2
	.byte 0xB7, 0xC9
	.4byte 0xCBDFB500

.global lbl_802BF918
lbl_802BF918:

	# ROM: 0x2499D8
	.asciz "TOAD"

.global lbl_802BF91D
lbl_802BF91D:

	# ROM: 0x2499DD
	.byte 0xC3, 0xDA, 0xBB
	.byte 0x00

.global lbl_802BF921
lbl_802BF921:

	# ROM: 0x2499E1
	.byte 0x42, 0x4F, 0x4F
	.byte 0x00

.global lbl_802BF925
lbl_802BF925:

	# ROM: 0x2499E5
	.byte 0x42, 0x55, 0x55
	.asciz " HUU"

.global lbl_802BF92D
lbl_802BF92D:

	# ROM: 0x2499ED
	.byte 0xD0, 0xC6, 0xB8
	.4byte 0xAFCADF00

.global lbl_802BF934
lbl_802BF934:

	# ROM: 0x2499F4
	.4byte 0xB7C9CBDF
	.byte 0xBA, 0x00

.global lbl_802BF93A
lbl_802BF93A:

	# ROM: 0x2499FA
	.byte 0xD0, 0xC6
	.4byte 0xB8AFCADF
	.byte 0x52, 0x00

.global lbl_802BF942
lbl_802BF942:

	# ROM: 0x249A02
	.byte 0xD0, 0xC6
	.4byte 0xB8AFCADF
	.byte 0x47, 0x00

.global lbl_802BF94A
lbl_802BF94A:

	# ROM: 0x249A0A
	.byte 0xD0, 0xC6
	.4byte 0xB8AFCADF
	.byte 0x42, 0x00

.global lbl_802BF952
lbl_802BF952:

	# ROM: 0x249A12
	.byte 0xC4, 0xDE
	.4byte 0xDDB7B000

.global lbl_802BF958
lbl_802BF958:

	# ROM: 0x249A18
	.byte 0x44, 0x4B, 0x00

.global lbl_802BF95B
lbl_802BF95B:

	# ROM: 0x249A1B
	.byte 0xB8
	.4byte 0xAFCADF00

.global lbl_802BF960
lbl_802BF960:

	# ROM: 0x249A20
	.asciz "BOWSER"
	.balign 4

.global lbl_802BF968
lbl_802BF968:

	# ROM: 0x249A28
	.4byte 0x00000003

.global lbl_802BF96C
lbl_802BF96C:

	# ROM: 0x249A2C
	.2byte 0xFFFF

.global lbl_802BF96E
lbl_802BF96E:

	# ROM: 0x249A2E
	.2byte 0xFFFF

.global lbl_802BF970
lbl_802BF970:

	# ROM: 0x249A30
	.4byte 0x30000000

.global lbl_802BF974
lbl_802BF974:

	# ROM: 0x249A34
	.4byte 0xFFFF0000

.global lbl_802BF978
lbl_802BF978:

	# ROM: 0x249A38
	.4byte 0xFFFFFFFF

.global lbl_802BF97C
lbl_802BF97C:

	# ROM: 0x249A3C
	.4byte 0x00000FA0

.global lbl_802BF980
lbl_802BF980:

	# ROM: 0x249A40
	.4byte 0xFFFFFFFF

.global lbl_802BF984
lbl_802BF984:

	# ROM: 0x249A44
	.byte 0x55, 0x53, 0x00

.global lbl_802BF987
lbl_802BF987:

	# ROM: 0x249A47
	.byte 0x2E
	.4byte 0x63747800

.global lbl_802BF98C
lbl_802BF98C:

	# ROM: 0x249A4C
	.asciz ".gcd"

.global lbl_802BF991
lbl_802BF991:

	# ROM: 0x249A51
	.byte 0x2E, 0x77, 0x72
	.byte 0x64, 0x00

.global lbl_802BF996
lbl_802BF996:

	# ROM: 0x249A56
	.byte 0x2E, 0x62
	.byte 0x69, 0x6E, 0x00

.global lbl_802BF99B
lbl_802BF99B:

	# ROM: 0x249A5B
	.byte 0x4F
	.asciz "uch!\n"

.global lbl_802BF9A2
lbl_802BF9A2:

	# ROM: 0x249A62
	.byte 0x42, 0x41
	.asciz "D SNR"
	.balign 4
	.4byte 0

.global lbl_802BF9B0
lbl_802BF9B0:

	# ROM: 0x249A70
	.4byte lbl_8023ADA8

.global lbl_802BF9B4
lbl_802BF9B4:

	# ROM: 0x249A74
	.asciz "%08x\n"
	.balign 4

.global lbl_802BF9BC
lbl_802BF9BC:

	# ROM: 0x249A7C
	.4byte 0x25730A00

.global lbl_802BF9C0
lbl_802BF9C0:

	# ROM: 0x249A80
	.4byte 0xFFFFFFFF
	.4byte 0

.global lbl_802BF9C8
lbl_802BF9C8:

	# ROM: 0x249A88
	.4byte 0xFFFFFFFF
	.4byte 0

.global lbl_802BF9D0
lbl_802BF9D0:

	# ROM: 0x249A90
	.4byte 0x000000F8

.global lbl_802BF9D4
lbl_802BF9D4:

	# ROM: 0x249A94
	.4byte 0x0A000000

.global lbl_802BF9D8
lbl_802BF9D8:

	# ROM: 0x249A98
	.4byte 0x25640000
	.4byte 0

.global lbl_802BF9E0
lbl_802BF9E0:

	# ROM: 0x249AA0
	.4byte 0xFFFF0000
	.4byte 0

.global lbl_802BF9E8
lbl_802BF9E8:

	# ROM: 0x249AA8
	.4byte lbl_8009EAD8

.global lbl_802BF9EC
lbl_802BF9EC:

	# ROM: 0x249AAC
	.4byte 0

.global lbl_802BF9F0
lbl_802BF9F0:

	# ROM: 0x249AB0
	.4byte 0
	.4byte 0x3F800000

.global lbl_802BF9F8
lbl_802BF9F8:

	# ROM: 0x249AB8
	.4byte 0x00000001
	.4byte 0

.global lbl_802BFA00
lbl_802BFA00:

	# ROM: 0x249AC0
	.asciz "dvdfs.c"

.global lbl_802BFA08
lbl_802BFA08:

	# ROM: 0x249AC8
	.4byte lbl_8023D158

.global lbl_802BFA0C
lbl_802BFA0C:

	# ROM: 0x249ACC
	.4byte 0x00000001

.global lbl_802BFA10
lbl_802BFA10:

	# ROM: 0x249AD0
	.4byte lbl_800A3BDC

.global lbl_802BFA14
lbl_802BFA14:

	# ROM: 0x249AD4
	.asciz "dvd.c"
	.balign 4

.global lbl_802BFA1C
lbl_802BFA1C:

	# ROM: 0x249ADC
	.4byte 0xFFFFFFFF

.global lbl_802BFA20
lbl_802BFA20:

	# ROM: 0x249AE0
	.4byte 0x0A000000

.global lbl_802BFA24
lbl_802BFA24:

	# ROM: 0x249AE4
	.4byte 0x4F464600

.global lbl_802BFA28
lbl_802BFA28:

	# ROM: 0x249AE8
	.4byte 0x4F4E0000
	.4byte 0

.global lbl_802BFA30
lbl_802BFA30:

	# ROM: 0x249AF0
	.4byte lbl_8023D398

.global lbl_802BFA34
lbl_802BFA34:

	# ROM: 0x249AF4
	.asciz "vi.c"
	.balign 4
	.4byte 0

.global lbl_802BFA40
lbl_802BFA40:

	# ROM: 0x249B00
	.asciz "%s: %8d"

.global lbl_802BFA48
lbl_802BFA48:

	# ROM: 0x249B08
	.4byte lbl_8023E4D8

.global lbl_802BFA4C
lbl_802BFA4C:

	# ROM: 0x249B0C
	.4byte 0x00000020

.global lbl_802BFA50
lbl_802BFA50:

	# ROM: 0x249B10
	.4byte 0xF0000000

.global lbl_802BFA54
lbl_802BFA54:

	# ROM: 0x249B14
	.4byte 0x00000300

.global lbl_802BFA58
lbl_802BFA58:

	# ROM: 0x249B18
	.4byte 0x00000005

.global lbl_802BFA5C
lbl_802BFA5C:

	# ROM: 0x249B1C
	.4byte lbl_800AB560

.global lbl_802BFA60
lbl_802BFA60:

	# ROM: 0x249B20
	.4byte 0x41000000

.global lbl_802BFA64
lbl_802BFA64:

	# ROM: 0x249B24
	.4byte 0x42000000

.global lbl_802BFA68
lbl_802BFA68:

	# ROM: 0x249B28
	.4byte lbl_8023E530
	.4byte 0

.global lbl_802BFA70
lbl_802BFA70:

	# ROM: 0x249B30
	.4byte lbl_8023E578
	.4byte 0

.global lbl_802BFA78
lbl_802BFA78:

	# ROM: 0x249B38
	.4byte lbl_8023E5C0
	.4byte 0

.global lbl_802BFA80
lbl_802BFA80:

	# ROM: 0x249B40
	.4byte lbl_8023E608
	.4byte 0

.global lbl_802BFA88
lbl_802BFA88:

	# ROM: 0x249B48
	.4byte lbl_8023E7E0
	.4byte 0

.global lbl_802BFA90
lbl_802BFA90:

	# ROM: 0x249B50
	.4byte 0x00040102

.global lbl_802BFA94
lbl_802BFA94:

	# ROM: 0x249B54
	.4byte 0x00080102

.global lbl_802BFA98
lbl_802BFA98:

	# ROM: 0x249B58
	.4byte 0x000C0102
	.4byte 0

.global lbl_802BFAA0
lbl_802BFAA0:

	# ROM: 0x249B60
	.4byte 0x80818283
	.4byte 0xA0A1A2A3

.global lbl_802BFAA8
lbl_802BFAA8:

	# ROM: 0x249B68
	.4byte 0x84858687
	.4byte 0xA4A5A6A7

.global lbl_802BFAB0
lbl_802BFAB0:

	# ROM: 0x249B70
	.4byte 0x88898A8B
	.4byte 0xA8A9AAAB

.global lbl_802BFAB8
lbl_802BFAB8:

	# ROM: 0x249B78
	.4byte 0x8C8D8E8F
	.4byte 0xACADAEAF

.global lbl_802BFAC0
lbl_802BFAC0:

	# ROM: 0x249B80
	.4byte 0x90919293
	.4byte 0xB0B1B2B3

.global lbl_802BFAC8
lbl_802BFAC8:

	# ROM: 0x249B88
	.4byte 0x94959697
	.4byte 0xB4B5B6B7

.global lbl_802BFAD0
lbl_802BFAD0:

	# ROM: 0x249B90
	.4byte 0x98999A9B
	.4byte 0xB8B9BABB

.global lbl_802BFAD8
lbl_802BFAD8:

	# ROM: 0x249B98
	.4byte 0x00040105
	.4byte 0x02060000

.global lbl_802BFAE0
lbl_802BFAE0:

	# ROM: 0x249BA0
	.4byte lbl_8023F268
	.4byte 0

.global lbl_802BFAE8
lbl_802BFAE8:

	# ROM: 0x249BA8
	.4byte 0x00000001
	.4byte 0

.global lbl_802BFAF0
lbl_802BFAF0:

	# ROM: 0x249BB0
	.2byte 0xFFFF

.global lbl_802BFAF2
lbl_802BFAF2:

	# ROM: 0x249BB2
	.byte 0x1C, 0x00
	.4byte 0

.global lbl_802BFAF8
lbl_802BFAF8:

	# ROM: 0x249BB8
	.4byte lbl_8023F460
	.4byte 0

.global lbl_802BFB00
lbl_802BFB00:

	# ROM: 0x249BC0
	.4byte lbl_8023F570

.global lbl_802BFB04
lbl_802BFB04:

	# ROM: 0x249BC4
	.asciz "Busy"
	.balign 4

.global lbl_802BFB0C
lbl_802BFB0C:

	# ROM: 0x249BCC
	.asciz "Unknown"
	.4byte 0

.global lbl_802BFB18
lbl_802BFB18:

	# ROM: 0x249BD8
	.4byte 0x00000001
	.4byte 0

.global lbl_802BFB20
lbl_802BFB20:

	# ROM: 0x249BE0
	.4byte 0x00000001
	.4byte 0x00020000

.global lbl_802BFB28
lbl_802BFB28:

	# ROM: 0x249BE8
	.4byte 0x00000001
	.4byte 0x00020000

.global lbl_802BFB30
lbl_802BFB30:

	# ROM: 0x249BF0
	.4byte 0x1EC00000
	.4byte 0

.global lbl_802BFB38
lbl_802BFB38:

	# ROM: 0x249BF8
	.4byte 0x01000000

.global lbl_802BFB3C
lbl_802BFB3C:

	# ROM: 0x249BFC
	.4byte lbl_802166AC
	.4byte 0

.global lbl_802BFB44
lbl_802BFB44:

	# ROM: 0x249C04
	.4byte lbl_8021669C
	.4byte lbl_80245350
	.4byte 0

.global lbl_802BFB50
lbl_802BFB50:

	# ROM: 0x249C10
	.4byte lbl_800EE0C4

.global lbl_802BFB54
lbl_802BFB54:

	# ROM: 0x249C14
	.4byte lbl_800EE09C

.global lbl_802BFB58
lbl_802BFB58:

	# ROM: 0x249C18
	.4byte 0xFFFFFFFE
	.4byte 0

.global lbl_802BFB60
lbl_802BFB60:

	# ROM: 0x249C20
	.4byte lbl_80216768
	.4byte 0

.global lbl_802BFB68
lbl_802BFB68:

	# ROM: 0x249C28
	.4byte lbl_80216754
	.4byte lbl_8024544C

.global lbl_802BFB70
lbl_802BFB70:

	# ROM: 0x249C30
	.4byte 0
	.4byte 0

.global lbl_802BFB78
lbl_802BFB78:

	# ROM: 0x249C38
	.4byte 0x7FFFFFFF

.global lbl_802BFB7C
lbl_802BFB7C:

	# ROM: 0x249C3C
	.4byte 0x7F800000

.global lbl_802BFB80
lbl_802BFB80:

	# ROM: 0x249C40
	.4byte 0x80000000
	.4byte 0

.global lbl_802BFB88
lbl_802BFB88:

	# ROM: 0x249C48
	.4byte lbl_8010373C
	.4byte lbl_80245C90

.global lbl_802BFB90
lbl_802BFB90:

	# ROM: 0x249C50
	.2byte 0x0004

.global lbl_802BFB92
lbl_802BFB92:

	# ROM: 0x249C52
	.byte 0x01, 0xF4
	.4byte 0

.global lbl_802BFB98
lbl_802BFB98:

	# ROM: 0x249C58
	.asciz "1.1.4"
	.balign 4

.global lbl_802BFBA0
lbl_802BFBA0:

	# ROM: 0x249C60
	.4byte 0x00000009

.global lbl_802BFBA4
lbl_802BFBA4:

	# ROM: 0x249C64
	.4byte 0x00000005

.global lbl_802BFBA8
lbl_802BFBA8:

	# ROM: 0x249C68
	.4byte 0xFFFFFFFF

.global lbl_802BFBAC
lbl_802BFBAC:

	# ROM: 0x249C6C
	.4byte 0x00000014

.global lbl_802BFBB0
lbl_802BFBB0:

	# ROM: 0x249C70
	.4byte 0x00000014

.global lbl_802BFBB4
lbl_802BFBB4:

	# ROM: 0x249C74
	.4byte 0xFFFFFFFF

.global lbl_802BFBB8
lbl_802BFBB8:

	# ROM: 0x249C78
	.byte 0x80

.global lbl_802BFBB9
lbl_802BFBB9:

	# ROM: 0x249C79
	.byte 0xBE

.global lbl_802BFBBA
lbl_802BFBBA:

	# ROM: 0x249C7A
	.byte 0x8C

.global lbl_802BFBBB
lbl_802BFBBB:

	# ROM: 0x249C7B
	.byte 0xFF
	.4byte 0x643282FF

.global lbl_802BFBC0
lbl_802BFBC0:

	# ROM: 0x249C80
	.asciz "eye1"

.global lbl_802BFBC5
lbl_802BFBC5:

	# ROM: 0x249C85
	.byte 0x65, 0x79, 0x65
	.byte 0x32, 0x00

.global lbl_802BFBCA
lbl_802BFBCA:

	# ROM: 0x249C8A
	.byte 0x6D, 0x61
	.4byte 0x74313400

.global lbl_802BFBD0
lbl_802BFBD0:

	# ROM: 0x249C90
	.asciz "mat16"

.global lbl_802BFBD6
lbl_802BFBD6:

	# ROM: 0x249C96
	.byte 0x6D, 0x61
	.4byte 0x74363500

.global lbl_802BFBDC
lbl_802BFBDC:

	# ROM: 0x249C9C
	.asciz "mat66"

.global lbl_802BFBE2
lbl_802BFBE2:

	# ROM: 0x249CA2
	.byte 0x4D, 0x61
	.4byte 0x72696F00

.global lbl_802BFBE8
lbl_802BFBE8:

	# ROM: 0x249CA8
	.asciz "Luigi"

.global lbl_802BFBEE
lbl_802BFBEE:

	# ROM: 0x249CAE
	.byte 0x50, 0x65
	.4byte 0x61636800

.global lbl_802BFBF4
lbl_802BFBF4:

	# ROM: 0x249CB4
	.asciz "Yoshi"

.global lbl_802BFBFA
lbl_802BFBFA:

	# ROM: 0x249CBA
	.byte 0x57, 0x61
	.4byte 0x72696F00

.global lbl_802BFC00
lbl_802BFC00:

	# ROM: 0x249CC0
	.asciz "Daisy"

.global lbl_802BFC06
lbl_802BFC06:

	# ROM: 0x249CC6
	.byte 0x57, 0x61
	.asciz "luigi"

.global lbl_802BFC0E
lbl_802BFC0E:

	# ROM: 0x249CCE
	.byte 0x4B, 0x69
	.asciz "nopio"

.global lbl_802BFC16
lbl_802BFC16:

	# ROM: 0x249CD6
	.byte 0x54, 0x65
	.asciz "resa"
	.balign 4

.global lbl_802BFC20
lbl_802BFC20:

	# ROM: 0x249CE0
	.4byte 0x00000006
	.4byte 0x0000000D

.global lbl_802BFC28
lbl_802BFC28:

	# ROM: 0x249CE8
	.4byte 0x00000007
	.4byte 0x00000008

.global lbl_802BFC30
lbl_802BFC30:

	# ROM: 0x249CF0
	.4byte 0x0000001E
	.4byte 0x0000001C

.global lbl_802BFC38
lbl_802BFC38:

	# ROM: 0x249CF8
	.4byte 0x0000023D
	.4byte 0x00000249

.global lbl_802BFC40
lbl_802BFC40:

	# ROM: 0x249D00
	.4byte 0x00000006
	.4byte 0x0000000D

.global lbl_802BFC48
lbl_802BFC48:

	# ROM: 0x249D08
	.4byte 0x0000001E
	.4byte 0x0000001C

.global lbl_802BFC50
lbl_802BFC50:

	# ROM: 0x249D10
	.4byte 0x00000007
	.4byte 0x00000008

.global lbl_802BFC58
lbl_802BFC58:

	# ROM: 0x249D18
	.4byte 0x0000023D
	.4byte 0x00000249

.global lbl_802BFC60
lbl_802BFC60:

	# ROM: 0x249D20
	.4byte 0x3FA66666
	.4byte 0x3FE66666

.global lbl_802BFC68
lbl_802BFC68:

	# ROM: 0x249D28
	.4byte 0x3FA66666
	.4byte 0x3FE66666

.global lbl_802BFC70
lbl_802BFC70:

	# ROM: 0x249D30
	.4byte 0x00000001
	.4byte 0x00000002

.global lbl_802BFC78
lbl_802BFC78:

	# ROM: 0x249D38
	.4byte 0x00000019
	.4byte 0x00000019

.global lbl_802BFC80
lbl_802BFC80:

	# ROM: 0x249D40
	.4byte lbl_80247770
	.4byte lbl_802477C8

.global lbl_802BFC88
lbl_802BFC88:

	# ROM: 0x249D48
	.4byte lbl_80247820
	.4byte lbl_802478A4

.global lbl_802BFC90
lbl_802BFC90:

	# ROM: 0x249D50
	.4byte 0x00050005
	.4byte 0x000E0020

.global lbl_802BFC98
lbl_802BFC98:

	# ROM: 0x249D58
	.4byte 0xFFFFFFFF
	.4byte 0

.global lbl_802BFCA0
lbl_802BFCA0:

	# ROM: 0x249D60
	.4byte lbl_802B8E74

.global lbl_802BFCA4
lbl_802BFCA4:

	# ROM: 0x249D64
	.byte 0xFF

.global lbl_802BFCA5
lbl_802BFCA5:

	# ROM: 0x249D65
	.byte 0xFF

.global lbl_802BFCA6
lbl_802BFCA6:

	# ROM: 0x249D66
	.byte 0xFF

.global lbl_802BFCA7
lbl_802BFCA7:

	# ROM: 0x249D67
	.byte 0xFF

.global lbl_802BFCA8
lbl_802BFCA8:

	# ROM: 0x249D68
	.4byte 0x00040000
	.4byte 0x00040003

.global lbl_802BFCB0
lbl_802BFCB0:

	# ROM: 0x249D70
	.4byte 0x00040001
	.4byte 0x00040003

.global lbl_802BFCB8
lbl_802BFCB8:

	# ROM: 0x249D78
	.asciz "coin"

.global lbl_802BFCBD
lbl_802BFCBD:

	# ROM: 0x249D7D
	.byte 0x70, 0x6C, 0x75
	.4byte 0x73000000
	.4byte 0

.global lbl_802BFCC8
lbl_802BFCC8:

	# ROM: 0x249D88
	.4byte 0x00000001

.global lbl_802BFCCC
lbl_802BFCCC:

	# ROM: 0x249D8C
	.4byte 0x030C0B17
	.byte 0xFF

.global lbl_802BFCD1
lbl_802BFCD1:

	# ROM: 0x249D91
	.byte 0x03, 0x0C, 0x0B
	.4byte 0x17FF0000

.global lbl_802BFCD8
lbl_802BFCD8:

	# ROM: 0x249D98
	.4byte 0xFFFFFFFF

.global lbl_802BFCDC
lbl_802BFCDC:

	# ROM: 0x249D9C
	.4byte 0xFFFFFFFF

.global lbl_802BFCE0
lbl_802BFCE0:

	# ROM: 0x249DA0
	.4byte 0

.global lbl_802BFCE4
lbl_802BFCE4:

	# ROM: 0x249DA4
	.4byte 0x25640000

.global lbl_802BFCE8
lbl_802BFCE8:

	# ROM: 0x249DA8
	.4byte 0x00010202
	.4byte 0x03030303

.global lbl_802BFCF0
lbl_802BFCF0:

	# ROM: 0x249DB0
	.4byte 0xFFFF0000
	.4byte 0

.global lbl_802BFCF8
lbl_802BFCF8:

	# ROM: 0x249DB8
	.4byte 0xFFFFFFFF

.global lbl_802BFCFC
lbl_802BFCFC:

	# ROM: 0x249DBC
	.4byte 0x030C0B17
	.byte 0xFF

.global lbl_802BFD01
lbl_802BFD01:

	# ROM: 0x249DC1
	.byte 0x03, 0x0C, 0x0B
	.byte 0x17, 0xFF

.global lbl_802BFD06
lbl_802BFD06:

	# ROM: 0x249DC6
	.2byte 0x2000

.global lbl_802BFD08
lbl_802BFD08:

	# ROM: 0x249DC8
	.2byte 0xFF00

.global lbl_802BFD0A
lbl_802BFD0A:

	# ROM: 0x249DCA
	.2byte 0xFFFF

.global lbl_802BFD0C
lbl_802BFD0C:

	# ROM: 0x249DCC
	.4byte 0xFFFFFFFF

.global lbl_802BFD10
lbl_802BFD10:

	# ROM: 0x249DD0
	.4byte 0x3F6B851F
	.4byte 0

.global lbl_802BFD18
lbl_802BFD18:

	# ROM: 0x249DD8
	.4byte 0x00010203
	.byte 0xFF

.global lbl_802BFD1D
lbl_802BFD1D:

	# ROM: 0x249DDD
	.byte 0x25, 0x64, 0x00

.global lbl_802BFD20
lbl_802BFD20:

	# ROM: 0x249DE0
	.byte 0xFF

.global lbl_802BFD21
lbl_802BFD21:

	# ROM: 0x249DE1
	.byte 0x7F

.global lbl_802BFD22
lbl_802BFD22:

	# ROM: 0x249DE2
	.byte 0x7F

.global lbl_802BFD23
lbl_802BFD23:

	# ROM: 0x249DE3
	.byte 0xFF
	.4byte 0x7F7FFFFF

.global lbl_802BFD28
lbl_802BFD28:

	# ROM: 0x249DE8
	.4byte 0xFFFFFFFF

.global lbl_802BFD2C
lbl_802BFD2C:

	# ROM: 0x249DEC
	.4byte 0xFFFFFFFF

.global lbl_802BFD30
lbl_802BFD30:

	# ROM: 0x249DF0
	.4byte 0xFFBEFFFF

.global lbl_802BFD34
lbl_802BFD34:

	# ROM: 0x249DF4
	.byte 0x25, 0x64, 0x00

.global lbl_802BFD37
lbl_802BFD37:

	# ROM: 0x249DF7
	.byte 0x68
	.4byte 0x65616400

.global lbl_802BFD3C
lbl_802BFD3C:

	# ROM: 0x249DFC
	.asciz "target"
	.balign 4

.global lbl_802BFD44
lbl_802BFD44:

	# ROM: 0x249E04
	.4byte 0x0011003A

.global lbl_802BFD48
lbl_802BFD48:

	# ROM: 0x249E08
	.4byte 0x0005008C
	.4byte 0x0005008C

.global lbl_802BFD50
lbl_802BFD50:

	# ROM: 0x249E10
	.4byte 0x00010203
	.4byte 0x04FF0000

.global lbl_802BFD58
lbl_802BFD58:

	# ROM: 0x249E18
	.4byte 0x0005008D
	.4byte 0x0005008D

.global lbl_802BFD60
lbl_802BFD60:

	# ROM: 0x249E20
	.4byte 0x00010203
	.4byte 0x04FF0000

.global lbl_802BFD68
lbl_802BFD68:

	# ROM: 0x249E28
	.4byte 0xFFFFFFFF

.global lbl_802BFD6C
lbl_802BFD6C:

	# ROM: 0x249E2C
	.4byte 0xFFFFFFFF

.global lbl_802BFD70
lbl_802BFD70:

	# ROM: 0x249E30
	.4byte 0xFFFFFFFF

.global lbl_802BFD74
lbl_802BFD74:

	# ROM: 0x249E34
	.4byte 0xFFFFFFFF

.global lbl_802BFD78
lbl_802BFD78:

	# ROM: 0x249E38
	.4byte 0xFFFFFFFF

.global lbl_802BFD7C
lbl_802BFD7C:

	# ROM: 0x249E3C
	.asciz "KINOKO"

.global lbl_802BFD83
lbl_802BFD83:

	# ROM: 0x249E43
	.byte 0x4B
	.asciz "ILLER"

.global lbl_802BFD8A
lbl_802BFD8A:

	# ROM: 0x249E4A
	.byte 0x44, 0x4F
	.4byte 0x4B414E00

.global lbl_802BFD90
lbl_802BFD90:

	# ROM: 0x249E50
	.asciz "NULL"

.global lbl_802BFD95
lbl_802BFD95:

	# ROM: 0x249E55
	.byte 0x4E, 0x41, 0x53
	.byte 0x49, 0x00

.global lbl_802BFD9A
lbl_802BFD9A:

	# ROM: 0x249E5A
	.byte 0x54, 0x4F
	.asciz "GEZO"

.global lbl_802BFDA1
lbl_802BFDA1:

	# ROM: 0x249E61
	.byte 0x4B, 0x55, 0x52
	.4byte 0x49424F00

.global lbl_802BFDA8
lbl_802BFDA8:

	# ROM: 0x249E68
	.asciz "PAKKUN"

.global lbl_802BFDAF
lbl_802BFDAF:

	# ROM: 0x249E6F
	.byte 0x4A
	.asciz "ANGO"

.global lbl_802BFDB5
lbl_802BFDB5:

	# ROM: 0x249E75
	.byte 0x48, 0x41, 0x4E
	.4byte 0x554B4500

.global lbl_802BFDBC
lbl_802BFDBC:

	# ROM: 0x249E7C
	.asciz "KAMEKKU"

.global lbl_802BFDC4
lbl_802BFDC4:

	# ROM: 0x249E84
	.asciz "SUKA"

.global lbl_802BFDC9
lbl_802BFDC9:

	# ROM: 0x249E89
	.byte 0x4B, 0x41, 0x52
	.byte 0x41, 0x00

.global lbl_802BFDCE
lbl_802BFDCE:

	# ROM: 0x249E8E
	.byte 0x42, 0x4F
	.4byte 0x424C4500

.global lbl_802BFDD4
lbl_802BFDD4:

	# ROM: 0x249E94
	.asciz "BIRIQ"

.global lbl_802BFDDA
lbl_802BFDDA:

	# ROM: 0x249E9A
	.byte 0x44, 0x4F
	.asciz "SSUN"

.global lbl_802BFDE1
lbl_802BFDE1:

	# ROM: 0x249EA1
	.byte 0x42, 0x4F, 0x4D
	.asciz "UHEI"

.global lbl_802BFDE9
lbl_802BFDE9:

	# ROM: 0x249EA9
	.byte 0x4E, 0x45, 0x54
	.byte 0x41, 0x00

.global lbl_802BFDEE
lbl_802BFDEE:

	# ROM: 0x249EAE
	.byte 0x47, 0x41
	.byte 0x20, 0x20, 0x00

.global lbl_802BFDF3
lbl_802BFDF3:

	# ROM: 0x249EB3
	.byte 0x54
	.4byte 0x554B4900

.global lbl_802BFDF8
lbl_802BFDF8:

	# ROM: 0x249EB8
	.asciz "TA  "

.global lbl_802BFDFD
lbl_802BFDFD:

	# ROM: 0x249EBD
	.byte 0x48, 0x4F, 0x4E
	.byte 0x45, 0x00

.global lbl_802BFE02
lbl_802BFE02:

	# ROM: 0x249EC2
	.byte 0x4C, 0x49
	.4byte 0x47485400

.global lbl_802BFE08
lbl_802BFE08:

	# ROM: 0x249EC8
	.asciz "TARU"

.global lbl_802BFE0D
lbl_802BFE0D:

	# ROM: 0x249ECD
	.byte 0x30, 0x30, 0x30
	.byte 0x30, 0x00

.global lbl_802BFE12
lbl_802BFE12:

	# ROM: 0x249ED2
	.byte 0x4B, 0x45
	.asciz "TTOU"

.global lbl_802BFE19
lbl_802BFE19:

	# ROM: 0x249ED9
	.byte 0x4D, 0x49, 0x52
	.asciz "ACLE"

.global lbl_802BFE21
lbl_802BFE21:

	# ROM: 0x249EE1
	.byte 0x4B, 0x4F, 0x4F
	.byte 0x50, 0x41, 0x00

.global lbl_802BFE27
lbl_802BFE27:

	# ROM: 0x249EE7
	.byte 0x44
	.asciz "ONKEY"

.global lbl_802BFE2E
lbl_802BFE2E:

	# ROM: 0x249EEE
	.byte 0x56, 0x53
	.byte 0x00

.global lbl_802BFE31
lbl_802BFE31:

	# ROM: 0x249EF1
	.byte 0x44, 0x49, 0x43
	.byte 0x45, 0x00

.global lbl_802BFE36
lbl_802BFE36:

	# ROM: 0x249EF6
	.byte 0x59, 0x41
	.asciz "MERU"

.global lbl_802BFE3D
lbl_802BFE3D:

	# ROM: 0x249EFD
	.byte 0x00, 0x00, 0x90
	.byte 0xC0

.global lbl_802BFE41
lbl_802BFE41:

	# ROM: 0x249F01
	.byte 0x25, 0x73, 0x00

.global lbl_802BFE44
lbl_802BFE44:

	# ROM: 0x249F04
	.asciz "%02d"

.global lbl_802BFE49
lbl_802BFE49:

	# ROM: 0x249F09
	.byte 0x7D, 0x3B, 0x0A
	.byte 0x00

.global lbl_802BFE4D
lbl_802BFE4D:

	# ROM: 0x249F0D
	.byte 0x00, 0x00, 0x90
	.byte 0xC0

.global lbl_802BFE51
lbl_802BFE51:

	# ROM: 0x249F11
	.byte 0x63, 0x65, 0x6E
	.4byte 0x74657200

.global lbl_802BFE58
lbl_802BFE58:

	# ROM: 0x249F18
	.byte 0xFF

.global lbl_802BFE59
lbl_802BFE59:

	# ROM: 0x249F19
	.byte 0xFF

.global lbl_802BFE5A
lbl_802BFE5A:

	# ROM: 0x249F1A
	.byte 0x80

.global lbl_802BFE5B
lbl_802BFE5B:

	# ROM: 0x249F1B
	.byte 0x40

.global lbl_802BFE5C
lbl_802BFE5C:

	# ROM: 0x249F1C
	.byte 0xFF

.global lbl_802BFE5D
lbl_802BFE5D:

	# ROM: 0x249F1D
	.byte 0xFF

.global lbl_802BFE5E
lbl_802BFE5E:

	# ROM: 0x249F1E
	.byte 0xFF

.global lbl_802BFE5F
lbl_802BFE5F:

	# ROM: 0x249F1F
	.byte 0xFF

.global lbl_802BFE60
lbl_802BFE60:

	# ROM: 0x249F20
	.byte 0xFF

.global lbl_802BFE61
lbl_802BFE61:

	# ROM: 0x249F21
	.byte 0xFF

.global lbl_802BFE62
lbl_802BFE62:

	# ROM: 0x249F22
	.byte 0xFF

.global lbl_802BFE63
lbl_802BFE63:

	# ROM: 0x249F23
	.byte 0xFF
	.4byte 0

.global lbl_802BFE68
lbl_802BFE68:

	# ROM: 0x249F28
	.4byte 0x0000000A

.global lbl_802BFE6C
lbl_802BFE6C:

	# ROM: 0x249F2C
	.4byte 0x00000005

.global lbl_802BFE70
lbl_802BFE70:

	# ROM: 0x249F30
	.4byte 0xFFFFFFFF

.global lbl_802BFE74
lbl_802BFE74:

	# ROM: 0x249F34
	.4byte 0xFFFFFFFF

.global lbl_802BFE78
lbl_802BFE78:

	# ROM: 0x249F38
	.4byte 0xFFFFFFFF

.global lbl_802BFE7C
lbl_802BFE7C:

	# ROM: 0x249F3C
	.4byte 0xFFFFFFFF

.global lbl_802BFE80
lbl_802BFE80:

	# ROM: 0x249F40
	.4byte 0xFFFFFFFF

.global lbl_802BFE84
lbl_802BFE84:

	# ROM: 0x249F44
	.4byte 0xFFFFFFFF

.global lbl_802BFE88
lbl_802BFE88:

	# ROM: 0x249F48
	.4byte 0xFFFFFFFF

.global lbl_802BFE8C
lbl_802BFE8C:

	# ROM: 0x249F4C
	.4byte 0xFFFFFFFF

.global lbl_802BFE90
lbl_802BFE90:

	# ROM: 0x249F50
	.byte 0x25, 0x64, 0x00

.global lbl_802BFE93
lbl_802BFE93:

	# ROM: 0x249F53
	.byte 0xFF

.global lbl_802BFE94
lbl_802BFE94:

	# ROM: 0x249F54
	.byte 0xFF

.global lbl_802BFE95
lbl_802BFE95:

	# ROM: 0x249F55
	.byte 0x7F

.global lbl_802BFE96
lbl_802BFE96:

	# ROM: 0x249F56
	.2byte 0xFF00

.global lbl_802BFE98
lbl_802BFE98:

	# ROM: 0x249F58
	.4byte 0xFFFFFFFF

.global lbl_802BFE9C
lbl_802BFE9C:

	# ROM: 0x249F5C
	.4byte 0x25640000

.global lbl_802BFEA0
lbl_802BFEA0:

	# ROM: 0x249F60
	.4byte 0x00110000
	.4byte 0x0011001B

.global lbl_802BFEA8
lbl_802BFEA8:

	# ROM: 0x249F68
	.4byte lbl_8024B7D8
	.4byte lbl_8024B838

.global lbl_802BFEB0
lbl_802BFEB0:

	# ROM: 0x249F70
	.4byte 0x01070411
	.4byte 0xFF000000

.global lbl_802BFEB8
lbl_802BFEB8:

	# ROM: 0x249F78
	.4byte 0x00000006
	.4byte 0x00000007

.global lbl_802BFEC0
lbl_802BFEC0:

	# ROM: 0x249F80
	.4byte 0xFFFFFFFF

.global lbl_802BFEC4
lbl_802BFEC4:

	# ROM: 0x249F84
	.4byte 0xC3C3C3C3
	.4byte 0xC3C3C300

.global lbl_802BFECC
lbl_802BFECC:

	# ROM: 0x249F8C
	.4byte 0x5DC35E00

.global lbl_802BFED0
lbl_802BFED0:

	# ROM: 0x249F90
	.4byte 0x25640000
	.4byte 0

.global lbl_802BFED8
lbl_802BFED8:

	# ROM: 0x249F98
	.4byte 0xFFFFFFFF

.global lbl_802BFEDC
lbl_802BFEDC:

	# ROM: 0x249F9C
	.asciz "grid2"
	.balign 4

.global lbl_802BFEE4
lbl_802BFEE4:

	# ROM: 0x249FA4
	.4byte 0
	.4byte 0
	.4byte 0

.global lbl_802BFEF0
lbl_802BFEF0:

	# ROM: 0x249FB0
	.4byte 0x0C15070B
	.byte 0x08, 0x06, 0xFF

.global lbl_802BFEF7
lbl_802BFEF7:

	# ROM: 0x249FB7
	.byte 0x74
	.asciz "arget"
	.balign 4

.global lbl_802BFF00
lbl_802BFF00:

	# ROM: 0x249FC0
	.4byte 0x057803E8
	.byte 0x05, 0x78

.global lbl_802BFF06
lbl_802BFF06:

	# ROM: 0x249FC6
	.balign 4
	.4byte 0x01000000

.global lbl_802BFF0C
lbl_802BFF0C:

	# ROM: 0x249FCC
	.4byte 0x0005006D
	.4byte 0x0005006E

.global lbl_802BFF14
lbl_802BFF14:

	# ROM: 0x249FD4
	.4byte 0x0005006F
	.4byte 0x00050070

.global lbl_802BFF1C
lbl_802BFF1C:

	# ROM: 0x249FDC
	.4byte 0x0005006D
	.4byte 0x0005006E

.global lbl_802BFF24
lbl_802BFF24:

	# ROM: 0x249FE4
	.4byte 0x0005006F
	.4byte 0x00050070
	.4byte 0

.global lbl_802BFF30
lbl_802BFF30:

	# ROM: 0x249FF0
	.byte 0xFF

.global lbl_802BFF31
lbl_802BFF31:

	# ROM: 0x249FF1
	.byte 0xFF

.global lbl_802BFF32
lbl_802BFF32:

	# ROM: 0x249FF2
	.byte 0xFF

.global lbl_802BFF33
lbl_802BFF33:

	# ROM: 0x249FF3
	.byte 0xFF

.global lbl_802BFF34
lbl_802BFF34:

	# ROM: 0x249FF4
	.byte 0xFF

.global lbl_802BFF35
lbl_802BFF35:

	# ROM: 0x249FF5
	.byte 0xFF

.global lbl_802BFF36
lbl_802BFF36:

	# ROM: 0x249FF6
	.byte 0xFF

.global lbl_802BFF37
lbl_802BFF37:

	# ROM: 0x249FF7
	.byte 0xFF

.global lbl_802BFF38
lbl_802BFF38:

	# ROM: 0x249FF8
	.byte 0xC0

.global lbl_802BFF39
lbl_802BFF39:

	# ROM: 0x249FF9
	.byte 0xC0

.global lbl_802BFF3A
lbl_802BFF3A:

	# ROM: 0x249FFA
	.byte 0xC0

.global lbl_802BFF3B
lbl_802BFF3B:

	# ROM: 0x249FFB
	.byte 0xFF
	.4byte 0

.global lbl_802BFF40
lbl_802BFF40:

	# ROM: 0x24A000
	.4byte 0xFFFFFFFF

.global lbl_802BFF44
lbl_802BFF44:

	# ROM: 0x24A004
	.4byte 0x00000002
	.4byte 0x00000007

.global lbl_802BFF4C
lbl_802BFF4C:

	# ROM: 0x24A00C
	.4byte 0x0C06FF00
	.4byte 0
	.4byte 0
	.4byte 0
	.4byte 0
