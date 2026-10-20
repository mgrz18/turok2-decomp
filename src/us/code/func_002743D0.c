
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
extern M2C_UNK D_800A8178;
extern f32 D_800B2A30;
f32 func_002743D0(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f1;
  f32 temp_f3;
  temp_f1 = (*((f32 *) (((s8 *) arg0) + 0))) - (*((f32 *) (((s8 *) arg1) + 0)));
  temp_f3 = (*((f32 *) (((s8 *) arg0) + 4))) - (*((f32 *) (((s8 *) arg1) + 4)));
  temp_f0_2 = (*((f32 *) (((s8 *) arg0) + 8))) - (*((f32 *) (((s8 *) arg1) + 8)));
  temp_f0 = ((temp_f1 * temp_f1) + (temp_f3 * temp_f3)) + (temp_f0_2 * temp_f0_2);
  if (!(D_800B2A30 <= temp_f0))
  {
    return (*((f32 *) (((s8 *) (&D_800A8178)) + 4))) - (temp_f0 / D_800B2A30);
  }
  if (arg1)
  {
    return 0.0f;
  }
  else
    if (arg1)
  {
    return 0.0f;
  }
  else
  {
    return 0.0f;
  }
}
