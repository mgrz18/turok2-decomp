
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
s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern u8 D_8011AAEE[];
s32 func_00419C54(s32 arg0, s32 arg1)
{
  s32 var_v1;
  s32 new_var;
  var_v1 = D_8011AAEE[0];
  new_var = (s32) var_v1;
  var_v1 = func_0041648C(arg1, new_var, 8, 0, 0xFF, 0);
  if (var_v1 == 0xF7)
  {
    var_v1 = 0xF8;
  }
  D_8011AAEE[0] = (u8) var_v1;
  return 0;
}
