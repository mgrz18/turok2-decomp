#include "common.h"
#include "globals/menu_text.h"
#include "globals/onscreen.h"

sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
void COnScreen__SetBoss(void *, void *, char *);
s32 TotemDefense_DrawTotem(sptr, s32, sptr);

void COnScreen__SetBoss2(void *, void *, char *);
#ifdef TARGET_PORT
#else
#endif

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

#define T2_TEXT(s) ((void *) (s))

/* The port: the same on TotemDefenseState / CGameObjectInstance (the N64 code reads both at raw
 * offsets: the definition 0x14, the AI's m_pfDraw 0x234, m_AnimType/m_AnimIndex/
 * m_AnimRepeatedCount 0x204/0x206/0x207, m_dwFlags 0x140; written after module 2's typed twin
 * TotemDefense_InitFight_2, TotemDefense_InitFight_2). state->def is the definition's body pointer-wide. */
void TotemDefense_InitFight(void *arg0, void *arg1) {
    TotemDefenseState *state = (TotemDefenseState *) arg0;
    CGameObjectInstance *obj = (CGameObjectInstance *) arg1;
    CGameObjectInstance *totem;
    CAI *ai;

    state->obj = obj;
    state->def = &obj->header.m_pIntelligence->m_Destructible;
    state->state = 0;
    state->kills = 0;
    state->timer = (f32) (obj->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 0xF);
    obj->m_AI.m_pfDraw = (void *) TotemDefense_DrawTotem;
    totem = state->obj;
    state->warned = 0;
    ai = &totem->m_AI;
    ai->m_AnimIndex = CGameObjectInstance__PickRandomAnim(totem, 0x5334, -1);
    ai->m_AnimType = 0x5334;
    totem->m_AI.m_dwFlags &= ~1;
    ai->m_AnimRepeatedCount = 0;
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, arg1, T2_TEXT(sTotemDefenseText_Enemies));
    COnScreen__SetBoss2((s32 *) &gEngineApp_m_OnScreen, arg1, &sTotemDefenseText_Time);
}
#else
void TotemDefense_InitFight(void *arg0, void *arg1)
{
  s32 new_var;
  void *temp_a3;
  void *temp_s0;
  *((void **) (((s8 *) arg0) + 0)) = arg1;
  *((s32 *) (((s8 *) arg0) + 4)) = (sptr) ((*((void **) (((s8 *) arg1) + 0x14))) + 0x14);
  new_var = *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x14)))) + 0x54));
  *((s32 *) (((s8 *) arg0) + 8)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x10)) = 0;
  *((f32 *) (((s8 *) arg0) + 0xC)) = (f32) (new_var * 0xF);
  *((s32 (**)(s32, s32, s32)) (((s8 *) arg1) + 0x234)) = TotemDefense_DrawTotem;
  temp_s0 = *((void **) (((s8 *) arg0) + 0));
  *((s32 *) (((s8 *) arg0) + 0x14)) = 0;
  temp_a3 = temp_s0 + 0x140;
  *((s8 *) (((s8 *) temp_a3) + 0xC6)) = CGameObjectInstance__PickRandomAnim(temp_s0, 0x5334, -1);
  *((s16 *) (((s8 *) temp_a3) + 0xC4)) = 0x5334;
  *((s32 *) (((s8 *) temp_s0) + 0x140)) = (s32) ((*((s32 *) (((s8 *) temp_s0) + 0x140))) & (~1));
  *((s8 *) (((s8 *) temp_a3) + 0xC7)) = 0;
  COnScreen__SetBoss(&gEngineApp_m_OnScreen, arg1, sTotemDefenseText_Enemies);
  COnScreen__SetBoss2(&gEngineApp_m_OnScreen, arg1, &sTotemDefenseText_Time);
}
#endif
