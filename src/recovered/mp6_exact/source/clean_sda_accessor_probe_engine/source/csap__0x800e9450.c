/* Generated clean-C SDA accessor probe. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;

extern volatile u8 csap__0x800e9450_data;

__attribute__((section(".text.csap__0x800e9450"), used, noinline))
void csap__0x800e9450(u8 value)
{
    csap__0x800e9450_data = value;
}
