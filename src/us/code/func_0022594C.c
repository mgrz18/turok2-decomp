
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
void *func_0022594C(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f1;
  f32 temp_f2;
  f32 var_f3;
  s32 temp_a3;
  s32 var_v1;
  void **var_a0;
  void *temp_v0;
  void *var_a2;
 do { var_v1 = 0; var_a2 = 0; } while (0);
  temp_a3 = *((s32 *) (((s8 *) arg0) + 0x104C));
  var_f3 = 3.4028235e38f;
  var_a0 = arg0 + 0xFCC;
  if (temp_a3 > 0)
  {
    do
    {
      temp_v0 = *var_a0;
      temp_f0 = (*((f32 *) (((s8 *) arg1) + 4))) - (*((f32 *) (((s8 *) temp_v0) + 4)));
      temp_f2 = (*((f32 *) (((s8 *) arg1) + 8))) - (*((f32 *) (((s8 *) temp_v0) + 8)));
      temp_f1 = (*((f32 *) (((s8 *) arg1) + 0xC))) - (*((f32 *) (((s8 *) temp_v0) + 0xC)));
      temp_f0_2 = ((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f1 * temp_f1);
      var_v1 += 1;
      if (temp_f0_2 < var_f3)
      {
        var_f3 = temp_f0_2;
        var_a2 = temp_v0;
      }
      var_a0 = (typeof(var_a0)) ((s8 *) ((typeof(var_a0)) (((s8 *) var_a0) + 4)));
    }
    while (var_v1 < temp_a3);
  }
  return var_a2;
}
