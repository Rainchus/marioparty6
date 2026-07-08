/* Generated clean-C SDA sequence probe. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
struct seq_pointed_halfword { unsigned char pad[4]; u16 field; };
struct seq_stride_412 { unsigned char bytes[412]; };
struct seq_flag_byte_4c { unsigned char pad[0x4c]; unsigned char enabled : 1; unsigned char rest : 7; };

extern volatile struct seq_pointed_halfword * seq_sym0;

__attribute__((section(".text.cssp_main_dol_2052_0x800b81f8"), used, noinline))
void cssp_main_dol_2052_0x800b81f8(void)
{
    seq_sym0->field = (u16)4;
}
