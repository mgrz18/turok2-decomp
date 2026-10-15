
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
void func_0026D474(void *arg0, void *arg1)
{
  void **temp_v1;
  void *temp_a2;
  s8 *new_var;
  void *temp_v1_2;
  void *new_var2;
  temp_v1 = *((void ***) (((s8 *) arg1) + 4));
  temp_a2 = arg0 + 0xC;
  if (temp_v1 != 0)
  {
    *temp_v1 = *((void **) (((s8 *) arg1) + 0));
  }
  temp_v1_2 = *((void **) (((s8 *) arg1) + 0));
  if (temp_v1_2 != 0)
  {
    *((void ***) (((s8 *) temp_v1_2) + 4)) = (void **) (*((void ***) (((s8 *) arg1) + 4)));
  }
  if ((*((void ***) (((s8 *) arg0) + 0xC))) == arg1)
  {
    *((void ***) (((s8 *) arg0) + 0xC)) = (void **) (*((void ***) (((s8 *) arg1) + 4)));
  }
  if ((*((void **) (((s8 *) temp_a2) + 4))) == arg1)
  {
    *((void **) (((s8 *) temp_a2) + 4)) = (void *) (*((void **) (((s8 *) arg1) + 0)));
  }
  *((s32 *) (((s8 *) temp_a2) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_a2) + 8))) - 1);
  if ((*((s32 *) (((s8 *) arg0) + 8))) == 0)
  {
    *((void **) (((s8 *) arg0) + 0)) = arg1;
    *((void **) (((s8 *) arg0) + 4)) = arg1;
    *((void **) (((s8 *) arg1) + 0)) = 0;
    *((void ***) (((s8 *) arg1) + 4)) = 0;
  }
  else
  {
    new_var2 = *((void **) (((s8 *) arg0) + 4));
    *((void ***) (((s8 *) arg1) + 4)) = 0;
    *((void **) (((s8 *) arg1) + 0)) = (void *) new_var2;
    new_var = ((s8 *) arg0) + 4;
    *((void **) (((s8 *) (*((void **) new_var))) + 4)) = arg1;
    *((void **) new_var) = arg1;
  }
  *((s32 *) (((s8 *) arg0) + 8)) = (s32) ((*((s32 *) (((s8 *) arg0) + 8))) + 1);
}
