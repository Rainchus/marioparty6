/* Generated clean-C SDA accessor probe. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;

extern volatile u8 csap__0x80003140_data;

__attribute__((section(".text.csap__0x80003140"), used, noinline))
void csap__0x80003140(void)
{
    csap__0x80003140_data = (u8)1;
}
