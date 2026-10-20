
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
extern f32 D_800A8090;
f32 func_0026F400(void *arg0, void *arg1, void *arg2)
{
  f32 temp_f1;
  f32 new_var;
  f32 temp_f1_2;
  f32 temp_f3;
  f32 temp_f4;
  f32 temp_f5;
  temp_f3 = *((f32 *) (((s8 *) arg0) + 0));
  temp_f4 = *((f32 *) (((s8 *) arg0) + 4));
  temp_f1_2 = *((f32 *) (((s8 *) arg0) + 8));
  temp_f5 = ((temp_f3 * (*((f32 *) (((s8 *) arg1) + 0)))) + (temp_f4 * (*((f32 *) (((s8 *) arg1) + 4))))) + (temp_f1_2 * (*((f32 *) (((s8 *) arg1) + 8))));
  new_var = *((f32 *) (((s8 *) arg2) + 4));
  temp_f1 = ((temp_f3 * (*((f32 *) (((s8 *) arg2) + 0)))) + (temp_f4 * new_var)) + (temp_f1_2 * (*((f32 *) (((s8 *) arg2) + 8))));
  if (temp_f1 != temp_f5)
  {
    if (arg1)
    {
      return ((*((f32 *) (((s8 *) arg0) + 0xC))) - temp_f5) / (temp_f1 - temp_f5);
    }
    else
    {
      return ((*((f32 *) (((s8 *) arg0) + 0xC))) - temp_f5) / (temp_f1 - temp_f5);
    }
  }
  return D_800A8090;
}
