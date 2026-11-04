
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
s32 *func_0026D3BC(void *arg0)
{
  s32 *temp_a1;
  s32 *temp_a1_2;
  s32 *temp_v1;
  void *temp_a1_3;
  s32 *new_var;
  temp_v1 = *((s32 **) (((s8 *) arg0) + 0));
  if (temp_v1 != 0)
  {
    temp_a1 = *((s32 **) (((s8 *) temp_v1) + 4));
    if (temp_a1 != 0)
    {
      *temp_a1 = *((s32 **) (((s8 *) temp_v1) + 0));
    }
    temp_a1_2 = *((s32 **) (((s8 *) temp_v1) + 0));
    if (temp_a1_2 != 0)
    {
      *((s32 **) (((s8 *) temp_a1_2) + 4)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 4)));
    }
    if ((*((s32 **) (((s8 *) arg0) + 0))) == temp_v1)
    {
      *((s32 **) (((s8 *) arg0) + 0)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 4)));
    }
    if ((*((s32 **) (((s8 *) arg0) + 4))) == temp_v1)
    {
      *((s32 **) (((s8 *) arg0) + 4)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 0)));
    }
    temp_a1_3 = arg0 + 0xC;
    *((s32 *) (((s8 *) arg0) + 8)) = (s32) ((*((s32 *) (((s8 *) arg0) + 8))) - 1);
    if ((*((s32 *) (((s8 *) temp_a1_3) + 8))) == 0)
    {
      *((s32 **) (((s8 *) arg0) + 0xC)) = temp_v1;
      *((s32 **) (((s8 *) temp_a1_3) + 4)) = temp_v1;
      *((s32 **) (((s8 *) temp_v1) + 0)) = 0;
      *((s32 **) (((s8 *) temp_v1) + 4)) = 0;
    }
    else
    {
      new_var = *((s32 **) (((s8 *) temp_a1_3) + 4));
      *((s32 **) (((s8 *) temp_v1) + 4)) = 0;
      *((s32 **) (((s8 *) temp_v1) + 0)) = new_var;
      *((s32 **) (((s8 *) (*((s32 **) (((s8 *) temp_a1_3) + 4)))) + 4)) = temp_v1;
      *((s32 **) (((s8 *) temp_a1_3) + 4)) = temp_v1;
    }
    *((s32 *) (((s8 *) temp_a1_3) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_a1_3) + 8))) + 1);
  }
  return temp_v1;
}
