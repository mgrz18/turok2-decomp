#include "common.h"
#include "globals/onscreen.h"

sptr CGameObjectInstance__PickRandomAnim(sptr, sptr, sptr);
s32 COnScreen__SetBoss(sptr, sptr, sptr);
s32 TotemDefense_DrawTotem(sptr, s32, sptr);

s32 COnScreen__SetBoss2(s32 *, void *, s32 *); /* extern */
extern s32 D_800C0560;
extern s32 D_800C0568;

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

/* The port: the same on TotemDefenseState / CGameObjectInstance (the N64 code reads both at raw
 * offsets: the definition 0x14, the AI's m_pfDraw 0x234, m_AnimType/m_AnimIndex/
 * m_AnimRepeatedCount 0x204/0x206/0x207, m_dwFlags 0x140; written after module 2's typed twin
 * TotemDefense_InitFight_2, func_00441B20). state->def is the definition's body pointer-wide. */
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
    ai->m_AnimIndex = CGameObjectInstance__PickRandomAnim((sptr) totem, 0x5334, -1);
    ai->m_AnimType = 0x5334;
    totem->m_AI.m_dwFlags &= ~1;
    ai->m_AnimRepeatedCount = 0;
    COnScreen__SetBoss((sptr) (&gEngineApp_m_OnScreen), (sptr) arg1, (sptr) (&D_800C0560));
    COnScreen__SetBoss2((s32 *) &gEngineApp_m_OnScreen, arg1, &D_800C0568);
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
  *((s8 *) (((s8 *) temp_a3) + 0xC6)) = CGameObjectInstance__PickRandomAnim((sptr) temp_s0, 0x5334, -1);
  *((s16 *) (((s8 *) temp_a3) + 0xC4)) = 0x5334;
  *((s32 *) (((s8 *) temp_s0) + 0x140)) = (s32) ((*((s32 *) (((s8 *) temp_s0) + 0x140))) & (~1));
  *((s8 *) (((s8 *) temp_a3) + 0xC7)) = 0;
  COnScreen__SetBoss((sptr) (&gEngineApp_m_OnScreen), (sptr) arg1, (sptr) (&D_800C0560));
  COnScreen__SetBoss2(&gEngineApp_m_OnScreen, arg1, &D_800C0568);
}
#endif
