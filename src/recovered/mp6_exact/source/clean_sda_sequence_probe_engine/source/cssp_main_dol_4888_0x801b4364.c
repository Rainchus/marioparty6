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

extern volatile u32 seq_sym0;
extern volatile u32 seq_sym1;
extern volatile u32 seq_sym2;
extern volatile u32 seq_sym3;

__attribute__((section(".text.cssp_main_dol_4888_0x801b4364"), used, noinline))
void cssp_main_dol_4888_0x801b4364(u32 value0, u32 value1, u32 value2, u32 value3)
{
    seq_sym0 = value0;
    seq_sym1 = value1;
    seq_sym2 = value2;
    seq_sym3 = value3;
}
