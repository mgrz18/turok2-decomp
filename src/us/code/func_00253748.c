
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
extern s32 D_8011AAD8;
u32 func_00253748(s32 arg0, s32 arg1)
{
  s32 new_var;
  if (!(D_8011AAD8 & 0x10))
  {
    if (new_var)
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
    else
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
  }
  return 1U;
}
