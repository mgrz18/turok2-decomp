#include "common.h"
#include "structs/game_object.h"

/* The state this sets up for its object. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *obj;
    /* 0x04 */ s32 info;                /* &obj's intelligence + 0x14 */
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} ObjState;

s32 CGameObjectInstance__PickRandomAnim(s32, s32, s32);
s32 COnScreen__SetBoss(s32, s32, s32);
s32 TotemDefense_DrawTotem(s32, s32, s32);

void COnScreen__SetBoss2(void *, void *, void *);         /* extern */
extern u8 D_800C0560[];
extern u8 D_800C0568[];
extern u8 D_801100F0[];

void TotemDefense_InitFight_2(ObjState *arg0, CGameObjectInstance *arg1)
{
  CAI *temp_a3;
  CGameObjectInstance *temp_s0;
  int new_var;
  arg0->obj = arg1;
  arg0->info = (s32) &arg1->header.m_pIntelligence->m_Destructible;
  new_var = arg1->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 0xF;
  arg0->unk8 = 0;
  arg0->unk10 = 0;
  arg0->unkC = (f32) new_var;
  arg1->m_AI.m_pfDraw = TotemDefense_DrawTotem;
  temp_s0 = arg0->obj;
  arg0->unk14 = 0;
  temp_a3 = &temp_s0->m_AI;
  temp_a3->m_AnimIndex = CGameObjectInstance__PickRandomAnim((s32) temp_s0, 0x5334, -1);
  temp_a3->m_AnimType = 0x5334;
  temp_s0->m_AI.m_dwFlags = temp_s0->m_AI.m_dwFlags & (~1);
  temp_a3->m_AnimRepeatedCount = 0;
  COnScreen__SetBoss((s32) D_801100F0, (s32) arg1, (s32) D_800C0560);
  COnScreen__SetBoss2(D_801100F0, arg1, D_800C0568);
}
