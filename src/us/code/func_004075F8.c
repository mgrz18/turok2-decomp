#include "common.h"

extern s32 gLevelScript_bossMode;

void Primagen_EnterMode18(void *arg0)
{
  f32 new_var;
  *((f32 *) (((s8 *) arg0) + 0x1FC)) = 1.0f;
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 3)
  {
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = 18.75f;
  }
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 4)
  {
    *((s8 *) (((s8 *) (&gLevelScript_bossMode)) + (-0xA))) = 1;
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = 18.75f;
  }
  new_var = 15.0f;
  *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) arg0) + 0xD4))) | 0x2000);
  *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 0xC)) = new_var;
}
