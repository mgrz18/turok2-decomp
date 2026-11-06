
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
extern s32 D_8011AAD8;
s32 CPlayer__IsSpecialPickup(s32 arg0, s32 arg1)
{
  int new_var;
  if (!(D_8011AAD8 & 0x10))
  {
 if (D_8011AAD8) { new_var = 0xFA0; } else { new_var = 0xFA0; } do { do { } while (0); } while (0);
    return ((u32) (arg1 - new_var)) < 0x401U;
  }
  return 0;
}
