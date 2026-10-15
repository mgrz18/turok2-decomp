
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
s32 func_002932BC();
s32 func_00293254(void *arg0, s32 *arg1)
{
  u32 temp_s1;
  unsigned char new_var;
  temp_s1 = *((u32 *) (((s8 *) arg0) + 8));
  new_var = 0x10;
  if (temp_s1 < ((u32) ((*((s32 *) (((s8 *) arg0) + 0))) + (*((s32 *) (((s8 *) arg0) + new_var))))))
  {
    if (arg0)
    {
      *arg1 = func_002932BC();
      *((u32 *) (((s8 *) arg0) + 8)) = temp_s1;
      return 1;
    }
    else
    {
      *arg1 = func_002932BC();
      *((u32 *) (((s8 *) arg0) + 8)) = temp_s1;
      return 1;
    }
  }
  if (arg0 || temp_s1)
  {
    return 0;
  }
  else
  {
    return 0;
  }
}
