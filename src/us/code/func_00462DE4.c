
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
volatile int __osContRamWrite(s32, s32, s32, s32, s32);
void __osPfsSelectBank_2(void *arg0)
{
  u8 sp18[0x20];
  s32 var_a0;
  u8 *var_v1;
  var_a0 = 0;
  var_v1 = sp18;
  do
  {
    var_v1 = &sp18[var_a0];
    var_a0 += 1;
    *var_v1 = *((u8 *) (((s8 *) arg0) + 0x65));
  }
  while (var_a0 < 0x20);
  var_a0 = *((s32 *) (((s8 *) arg0) + 4));
  __osContRamWrite(*((s32 *) (((s8 *) arg0) + 4)), *((s32 *) (((s8 *) arg0) + 8)), 0x400, (s32) sp18, 0);
}
