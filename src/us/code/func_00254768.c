#include "globals/render.h"

#include "common.h"
#include "globals/game.h"
s32 Hud_SetupTextureMode(s32);
s32 Hud_DrawSprite(s32, s32, s16, s16, f32, f32, s32);
#ifdef TARGET_PORT
/* The host body on the members (round 12 a5, tools/port/n64viewcheck.py): the caller
 * (CPlayer__Update, tag mode) passes the player and its camera; 0x288-0x294 are N64 offsets of
 * the camera's viewport and 0xB60 of CPlayer.m_SightIconFrame. */
#include "structs/player.h"
#include "structs/camera.h"
void func_00254768(void *arg0, void *arg1)
{
  CPlayer *player = arg0;
  CCamera *view = arg1;
  f32 w;
  f32 h;
  f32 sx;
  f32 sy;
  Hud_SetupTextureMode((s32) gEngineApp_m_Settings.hudAlpha);
  w = view->m_Viewport.m_Width;
  sx = w / ((f32) gScreenWidth);
  h = view->m_Viewport.m_Height;
  sy = h / ((f32) gScreenHeight);
  Hud_DrawSprite(0x1FC, (s32) player->m_SightIconFrame,
                 (s16) ((s32) ((view->m_Viewport.m_ScreenPosX + w) + (sx * -5.0e1f))),
                 (s16) ((s32) ((view->m_Viewport.m_ScreenPosY + h) + (sy * -5.0e1f))), sx, sy, 1);
}
#else
void func_00254768(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f2;
  float new_var2;
  f32 temp_f3;
  f32 temp_f5;
  float new_var;
  Hud_SetupTextureMode((s32) gEngineApp_m_Settings.hudAlpha);
  temp_f0 = *((f32 *) (((s8 *) arg1) + 0x288));
  temp_f3 = temp_f0 / ((f32) gScreenWidth);
  temp_f5 = *((f32 *) (((s8 *) arg1) + 0x28C));
  temp_f2 = temp_f5 / ((f32) gScreenHeight);
  new_var = (*((f32 *) (((s8 *) arg1) + 0x290))) + temp_f0;
  new_var2 = (*((f32 *) (((s8 *) arg1) + 0x294))) + temp_f5;
  Hud_DrawSprite(0x1FC, (s32) (*((f32 *) (((s8 *) arg0) + 0xB60))), (s16) ((s32) (new_var + (temp_f3 * -5.0e1f))), (s16) ((s32) (new_var2 + (temp_f2 * -5.0e1f))), temp_f3, temp_f2, 1);
}
#endif
