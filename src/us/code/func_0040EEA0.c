#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 model;
} DrawParams;

s32 func_0020A33C(s32, s32, s32, s32, s32, s32, s32);
s32 func_0021D7BC(s32, s32, s32);
s32 func_00235898(void);
s32 func_00236324(void *);
extern s32 D_800AB8B0;
extern s32 D_800B6D1C;

void func_0040EEA0(CGameObjectInstance *arg0, CAI *arg1, DrawParams *arg2)
{
  ScriptAnimator *slots;
  slots = (ScriptAnimator *)&D_80130200;
  func_0021D7BC((s32) arg0, (s32) arg1, (s32) arg2);
  if (arg1->m_Health != 0)
  {
    /* &D_80130200.slots[i], through the block base as the ROM does */
    func_00236324((u8 *) slots + (arg1->m_VarBuffer[2] * sizeof(ScriptAnimator) + 0x3C));
    func_00235898();
    func_0020A33C(arg2->model, (s32) arg0->m_pmtDrawMtxs, 1, (s32) arg0->m_Lights[D_800B6D1C], 0, 1, arg0->m_AI.m_nTexture);
  }
  D_800AB8B0 = 0;
}
