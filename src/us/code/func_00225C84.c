
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
s32 func_00220260(s32, s32, s32);
void func_00225C84(void *arg0)
{
  s32 temp_s2;
  int new_var;
  s32 var_s0;
  s32 var_s1;
  void *temp_a1;
  temp_s2 = *((s32 *) (((s8 *) arg0) + 0xE8));
  var_s0 = 0;
  var_s1 = 0;
  if (temp_s2 > 0)
  {
    new_var = 3;
    var_s1 = 0;
    do
    {
      temp_a1 = (*((s32 *) (((s8 *) arg0) + 0xE0))) + var_s1;
      if ((*(*((s32 **) (((s8 *) temp_a1) + 0x14)))) == new_var)
      {
        func_00220260((s32) arg0, (s32) temp_a1, 1);
      }
      var_s0 += 1;
      var_s1 += 0x288;
    }
    while (var_s0 < temp_s2);
  }
}
