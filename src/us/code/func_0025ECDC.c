#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

s32 Render_DrawModelShared(s32, s32, s32, s32, s32, s32, s32);
s32 CGameObjectInstance__DrawModelImmediate(s32, sptr, s32);


void EnemyAI_DrawNode(s32 arg0, s32 arg1, s32 arg2)
{
  int new_var;
  CGameObjectInstance__DrawModelImmediate(arg0, arg1, arg2);
  new_var = 0x110;
  Render_DrawModelShared(*((s32 *) (((s8 *) arg2) + 0xC)), *((s32 *) (((s8 *) arg0) + 0x98)), 1, arg0 + ((gEvenOdd * 0x18) + new_var), 0, (s32) (*((s8 *) (((s8 *) arg2) + 0x1B))), -1);
}
