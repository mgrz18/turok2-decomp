#include "common.h"
#include "globals/boss.h"


void Primagen_EnterMode18_2(void *arg0)
{
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
  *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) arg0) + 0xD4))) | 0x2000);
  *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 0xC)) = 15.0f;
}
