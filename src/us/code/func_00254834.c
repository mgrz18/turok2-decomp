/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GFX/TEXT/8022BA90_3.CPP, func_8022C2B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/render.h"

struct ScreenRectangle;
#include "common.h"
#include "globals/game.h"
extern void func_00254834(void *arg0, struct ScreenRectangle *arg1);
#ifdef TARGET_PORT
/* The camera's viewport at its host offsets (round 12 a5, tools/port/n64viewcheck.py): the N64 view
 * read 0x288.. of the host CCamera, which has pointers before m_Viewport. */
#include "structs/camera.h"
#define T2_HOFF_ScreenRectangle(m) __builtin_offsetof(CCamera, m)
typedef struct ScreenRectangle {
    union {
        struct { u8 _hp_x[T2_HOFF_ScreenRectangle(m_Viewport.m_Width)]; f32 x; }; /* N64 0x288 */
        struct { u8 _hp_y[T2_HOFF_ScreenRectangle(m_Viewport.m_Height)]; f32 y; }; /* N64 0x28C */
        struct { u8 _hp_width[T2_HOFF_ScreenRectangle(m_Viewport.m_ScreenPosX)]; f32 width; }; /* N64 0x290 */
        struct { u8 _hp_height[T2_HOFF_ScreenRectangle(m_Viewport.m_ScreenPosY)]; f32 height; }; /* N64 0x294 */
    };
} ScreenRectangle;
#else
typedef struct ScreenRectangle { char pad0[0x288]; f32 x,y,width,height; } ScreenRectangle;
#endif
extern s32 Hud_DrawSprite(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6);
void Hud_SetupTextureMode(u8 arg0);
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
  Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
  x = arg1->x;
  ax = x * 0.5f;
  y = arg1->y;
  ay = y * 0.5f;
  sx = x / ((f32) gScreenWidth);
  bx = sx * 16.0f;
  sy = y / ((f32) gScreenHeight);
  by = sy * 16.0f;
  screenX = ((arg1->width) + ax) - bx;
  screenY = ((arg1->height) + ay) - by;
  Hud_DrawSprite(0x1FB, 0, (s16) ((s32) (((arg1->width) + ax) - bx)), (s16) ((s32) screenY), sx, sy, 1);
}
