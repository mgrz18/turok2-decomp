
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
extern f32 D_800B6D28;
s32 func_00211860(s32 arg0, s32 arg1, s32 arg2)
{
  f32 temp_f1;
  s32 var_a0;
  s32 var_a2;
  s32 var_v0;
  var_a2 = arg2;
  temp_f1 = (f32) arg1;
  var_a0 = arg0 + ((s32) (temp_f1 * D_800B6D28));
  if (temp_f1 < 0.0f)
  {
    var_a2 = -var_a2;
    var_v0 = var_a0 < var_a2;
  }
  else
  {
    var_v0 = var_a2 < var_a0;
  }
  if (var_v0 != 0)
  {
    var_a0 = var_a2;
  }
  if (var_a2)
  {
    return var_a0;
  }
  else
  {
    return var_a0;
  }
}
