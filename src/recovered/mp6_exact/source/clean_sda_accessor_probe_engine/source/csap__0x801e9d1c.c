/* Generated clean-C SDA accessor probe. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;

extern volatile u32 csap__0x801e9d1c_data;

__attribute__((section(".text.csap__0x801e9d1c"), used, noinline))
void csap__0x801e9d1c(u32 value)
{
    csap__0x801e9d1c_data = value;
}
