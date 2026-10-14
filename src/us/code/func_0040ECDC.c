
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
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
s32 func_00235510(s32);
s32 func_0040C850(s32);
s32 func_0040CB28(s32);
M2C_UNK func_0027AD88(M2C_UNK *, s32);
extern M2C_UNK D_00432410;
extern M2C_UNK D_801100F0;
void func_0040ECDC(void *arg0)
{
  M2C_UNK (*temp_v0)();
  s32 var_s0;
  s32 var_s1;
  temp_v0 = *((typeof(&D_00432410)) (((s8 *) (&D_00432410)) + ((*((s32 *) (((s8 *) arg0) + 0x4FC))) * 8)));
  if (temp_v0 != 0)
  {
    temp_v0();
  }
  var_s0 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0x4EC))) > 0)
  {
    var_s1 = 0x3C;
    do
    {
      func_00235510((s32) (arg0 + var_s1));
      var_s0 += 1;
      var_s1 += 0x64;
      var_s0++;
      var_s0--;
    }
    while (var_s0 < (*((s32 *) (((s8 *) arg0) + 0x4EC))));
  }
  func_0040C850((s32) (arg0 + 0x518));
  func_0040C850((s32) (arg0 + 0x530));
  func_0040C850((s32) (arg0 + 0x548));
  func_0027AD88(&D_801100F0, func_0040CB28((s32) arg0));
}
