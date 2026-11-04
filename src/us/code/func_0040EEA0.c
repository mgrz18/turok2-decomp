#include "common.h"
#include "structs/game_object.h"

/* A 0x64-byte record of D_80130200, with a swoosh trail at 0x3C. */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ u8 trail[0x64 - 0x3C];
} TrailSlot;

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
extern TrailSlot D_80130200[];

void func_0040EEA0(CGameObjectInstance *arg0, CAI *arg1, DrawParams *arg2)
{
  TrailSlot *slots;
  slots = D_80130200;
  func_0021D7BC((s32) arg0, (s32) arg1, (s32) arg2);
  if (arg1->m_Health != 0)
  {
    /* &slots[i].trail */
    func_00236324((u8 *) slots + (arg1->m_VarBuffer[2] * sizeof(TrailSlot) + 0x3C));
    func_00235898();
    func_0020A33C(arg2->model, (s32) arg0->m_pmtDrawMtxs, 1, (s32) arg0->m_Lights[D_800B6D1C], 0, 1, arg0->m_AI.m_nTexture);
  }
  D_800AB8B0 = 0;
}
