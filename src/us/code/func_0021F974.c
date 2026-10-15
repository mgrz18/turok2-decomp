
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
void func_0021F974(void *arg0, f32 arg1, void *arg2, void *arg3)
{
  f32 temp_f1;
  f32 temp_f1_2;
  f32 temp_f1_3;
  f32 new_var;
  new_var = *((f32 *) (((s8 *) arg3) + 0));
  temp_f1 = *((f32 *) (((s8 *) arg2) + 0));
  *((f32 *) (((s8 *) arg0) + 0)) = (f32) (temp_f1 + (arg1 * (new_var - temp_f1)));
  temp_f1_2 = *((f32 *) (((s8 *) arg2) + 4));
  new_var = *((f32 *) (((s8 *) arg3) + 4));
  *((f32 *) (((s8 *) arg0) + 4)) = (f32) (temp_f1_2 + (arg1 * (new_var - temp_f1_2)));
  ;
  *((f32 *) (((s8 *) arg0) + 8)) = (f32) ((*((f32 *) (((s8 *) arg2) + 8))) + (arg1 * ((*((f32 *) (((s8 *) arg3) + 8))) - (*((f32 *) (((s8 *) arg2) + 8))))));
}
