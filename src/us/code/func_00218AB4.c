
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
u32 func_00218AB4(s32 arg0)
{
  s32 var_v0;
  u32 var_a1;
  u32 var_a2;
  u32 var_v1;
  var_a1 = 0;
  var_a2 = 0;
  var_v1 = 0;
  var_v0 = 1 << 0;
  do
  {
    var_v0 = 1 << var_v1;
    if (arg0 & var_v0)
    {
      var_a1 += 1;
      var_a2 = var_v1;
    }
    var_v1 += 1;
  }
  while (var_v1 < 0x20U);
  if (var_a1 >= 2U)
  {
    return var_a2 + 1;
  }
  return var_a2;
}
