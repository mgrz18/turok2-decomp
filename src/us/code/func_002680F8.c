
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
s32 func_002680F8(void *arg0)
{
  void *temp_a0;
  int new_var2;
  s32 *new_var;
  temp_a0 = *((void **) (((s8 *) arg0) + 0x14));
  if ((*(new_var = (s32 *) (((s8 *) temp_a0) + 0))) != 1)
  {
    if (temp_a0)
    {
      return 0;
    }
    else
    {
      new_var2 = 0;
      return new_var2;
    }
  }
  return (*((s32 *) (((s8 *) temp_a0) + 0x48))) & 0x2000;
}
