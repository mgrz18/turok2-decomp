
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
void *func_002017D4(s32, M2C_UNK);
s32 func_00226D54(void *arg0)
{
  s32 var_s0;
  s8 *new_var;
  void *var_s1;
  s32 new_var2;
  int new_var3;
  var_s0 = 0;
  new_var3 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0x14A8))) > new_var3)
  {
    var_s1 = arg0;
    loop_2:
    var_s0 += 1;

    if ((new_var2 = *((s32 *) (((s8 *) func_002017D4(*(*((s32 **) (((s8 *) var_s1) + 0x14AC))), 2)) + 4))) != 0)
    {
      return 1;
    }
    var_s1 += 8;
    new_var = ((s8 *) arg0) + 0x14A8;
    if (var_s0 >= (*((s32 *) new_var)))
    {
      goto block_5;
    }
    goto loop_2;
  }
  block_5:
  return 0;

}
