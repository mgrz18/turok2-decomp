
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
s32 func_00285628(s32);
s32 func_00285670(s32);
s32 func_00416558(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
  s32 var_s0;
  s32 var_s1;
  s32 var_s2;
  s32 var_s4;
  s32 var_s5;
  s32 var_v0;
  int new_var;
  void *var_s3;
  ;
  var_s0 = arg1;
  var_s4 = arg2;
  var_s1 = arg4;
  var_s5 = arg5;
  var_s2 = arg3;
  if (func_00285628(*((s32 *) (0x20 + ((s8 *) arg0)))) != 0)
  {
    var_s0 -= var_s4;
    if (var_s0 < var_s2)
    {
      var_s0 = var_s2;
      if (var_s5 != 0)
      {
        if (var_s2 || arg0)
        {
          var_s0 = var_s1;
        }
        else
        {
          var_s0 = var_s1;
        }
      }
    }
  }
  var_v0 = var_s0;
 new_var = 0x20; if (func_00285670(*((s32 *) (((s8 *) arg0) + new_var))) != 0) { var_s0 += var_s4;
    var_v0 = var_s0;
    if (var_s1 < var_s0)
    {
      var_s0 = var_s1;
      if (var_s5 != 0)
      {
        var_s0 = var_s2;
      }
      var_v0 = var_s0;
    }
  }
  return var_v0;
}
