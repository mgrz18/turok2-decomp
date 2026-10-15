
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
extern M2C_UNK D_800B5AF4;
extern M2C_UNK D_800B5B10;
extern s32 D_8011AAD8;
extern s32 D_8011AADC;
s32 func_0041B9DC(void *arg0)
{
  s32 var_v0;
  int new_var;
  if (D_8011AADC & 0x8000)
  {
 new_var = 0x01000000; do { var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) | new_var; } while (0);
  }
  else
  {
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) & 0xFEFFFFFF;
  }
  *((s32 *) (((s8 *) arg0) + 8)) = var_v0;
  if (D_8011AAD8 & 0x8000)
  {
    *((M2C_UNK **) (((s8 *) arg0) + 0x14)) = &D_800B5AF4;
  }
  else
  {
    *((M2C_UNK **) (((s8 *) arg0) + 0x14)) = &D_800B5B10;
  }
  return 0;
}
