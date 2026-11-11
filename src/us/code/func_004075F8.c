#include "common.h"
#include "globals/boss.h"


#ifdef TARGET_PORT
#include "structs/game_object.h"

/* Host body: arg0 + 0x1FC is m_AI.m_AnimSpeed and + 0xD4 m_dwFlags at N64 offsets (as
 * Primagen_EnterMode19 has them typed); the gLevelScript_bossMode words (mode, timer at +8,
 * swayTimer at +0xC, unk3CA at -0xA) are a run of scalars, the same deltas on the host. */
void Primagen_EnterMode18(void *arg0)
{
  CGameObjectInstance *obj = (CGameObjectInstance *) arg0;

  obj->m_AI.m_AnimSpeed = 1.0f;
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 3)
  {
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = 18.75f;
  }
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 4)
  {
    *((s8 *) (((s8 *) (&gLevelScript_bossMode)) + (-0xA))) = 1;
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = 18.75f;
  }
  obj->m_dwFlags |= 0x2000;
  *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 0xC)) = 15.0f;
}
#else
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
#endif
