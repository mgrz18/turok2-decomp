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

typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} Vec3;

extern s32 D_800A8178;          /* f32 constants; [1] used here */
extern f32 D_800B2A30;          /* falloff distance squared */

/* Distance falloff: 1 - d^2 / D_800B2A30 inside, 0 outside. */
f32 func_002743D0(Vec3 *arg0, Vec3 *arg1)
{
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f1;
  f32 temp_f3;
  temp_f1 = arg0->x - arg1->x;
  temp_f3 = arg0->y - arg1->y;
  temp_f0_2 = arg0->z - arg1->z;
  temp_f0 = ((temp_f1 * temp_f1) + (temp_f3 * temp_f3)) + (temp_f0_2 * temp_f0_2);
  if (!(D_800B2A30 <= temp_f0))
  {
    return ((f32 *) &D_800A8178)[1] - (temp_f0 / D_800B2A30);
  }
  if (arg1)
  {
    return 0.0f;
  }
  else
    if (arg1)
  {
    return 0.0f;
  }
  else
  {
    return 0.0f;
  }
}
