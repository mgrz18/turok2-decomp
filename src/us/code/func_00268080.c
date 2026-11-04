
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
s32 func_00268080(void *arg0)
{
  s8 *new_var;
  void *temp_a0;
  temp_a0 = *((void **) (((s8 *) arg0) + 0x14));
  if ((*((s32 *) (((s8 *) temp_a0) + 0))) != 1)
  {
    if (temp_a0 || new_var)
    {
      return 0;
    }
    else
    {
      return 0;
    }
  }
  new_var = ((s8 *) temp_a0) + 0x48;
  return (*((s32 *) new_var)) & 0x20;
}
