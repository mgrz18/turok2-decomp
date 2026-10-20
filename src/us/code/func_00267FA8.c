
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
s32 func_00267FA8(void *arg0)
{
  void *temp_a0;
  unsigned long long new_var;
  temp_a0 = *((void **) (((s8 *) arg0) + 0x14));
  if ((*((s32 *) (((s8 *) temp_a0) + (new_var = 0)))) != 1)
  {
    if (1)
    {
      return new_var;
    }
 do { } while (new_var);
  }
  return (*((s32 *) (((s8 *) temp_a0) + 0x48))) & 8;
}
