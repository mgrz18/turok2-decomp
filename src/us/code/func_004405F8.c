#include "common.h"

extern f32 D_800C051C;
extern f32 D_800C0520;
extern f32 D_800C0524;
extern f32 D_800C0528;
extern s32 gLevelScript_bossMode;

void Primagen_EnterMode18_2(void *arg0)
{
  *((f32 *) (((s8 *) arg0) + 0x1FC)) = (f32) D_800C051C;
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 3)
  {
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = (f32) D_800C0520;
  }
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 4)
  {
    *((s8 *) (((s8 *) (&gLevelScript_bossMode)) + (-0xA))) = 1;
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = (f32) D_800C0524;
  }
  if ((D_800C0528 && D_800C0528) && D_800C0528)
  {
  }
  *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) arg0) + 0xD4))) | 0x2000);
  *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 0xC)) = (f32) D_800C0528;
}
