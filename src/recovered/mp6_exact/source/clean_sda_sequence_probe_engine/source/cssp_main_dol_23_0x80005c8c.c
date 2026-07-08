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

extern volatile s32 seq_sym0;

__attribute__((section(".text.cssp_main_dol_23_0x80005c8c"), used, noinline))
s32 cssp_main_dol_23_0x80005c8c(void)
{
    return (s16)seq_sym0;
}
