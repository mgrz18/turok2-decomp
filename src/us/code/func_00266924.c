#include "common.h"
#include "structs/game_object.h"

s32 func_00220260(s32, s32, s32);

extern s32 D_800F7078;

void func_00266924(CGameObjectInstance *arg0)
{
  int new_var;
  new_var = ~0x2000;
  arg0->m_dwFlags = (s32) ((arg0->m_dwFlags & new_var) & (~0x100));
  func_00220260((s32) (&D_800F7078), (s32) arg0, 1);
}
