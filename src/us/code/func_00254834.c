/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GFX/TEXT/8022BA90_3.CPP, func_8022C2B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ScreenRectangle;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
extern float D_800A7120;
extern float D_800A7124;
extern void func_00254834(void *arg0, struct ScreenRectangle *arg1);
typedef struct ScreenRectangle { char pad0[0x288]; f32 x,y,width,height; } ScreenRectangle;
extern s32 func_0027B320(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6);
extern u8 D_8011AAEA;
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_00279028(u8 arg0);
void func_00254834(void *arg0, struct ScreenRectangle *arg1)
{
  f32 x;
  f32 y;
  f32 ax;
  f32 ay;
  f32 sx;
  f32 sy;
  f32 bx;
  f32 by;
  f32 screenX;
  f32 screenY;
  func_00279028(D_8011AAEA);
  x = arg1->x;
  ax = x * D_800A7120;
  y = arg1->y;
  ay = y * D_800A7120;
  sx = x / ((f32) D_800C2030);
  bx = sx * D_800A7124;
  sy = y / ((f32) D_800C2034);
  by = sy * D_800A7124;
  screenX = ((arg1->width) + ax) - bx;
  screenY = ((arg1->height) + ay) - by;
  func_0027B320(0x1FB, 0, (s16) ((s32) (((arg1->width) + ax) - bx)), (s16) ((s32) screenY), sx, sy, 1);
}
