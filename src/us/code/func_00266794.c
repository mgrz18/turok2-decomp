#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

/* Typed on CGameObjectInstance/CAI (m2c's offsets 0x14/0xD4 and the AI's 0x10C-0x114 are the
 * N64 layout): the AI's first variable takes the word after the intelligence's type flags. */
void InteractiveAnim_Advance(CGameObjectInstance *arg0, CAI *arg1)
{
  int new_var;
  arg1->m_VarS32[0] = ((s32 *) &arg0->header.m_pIntelligence->m_dwTypeFlags)[1];
  CAI__SetMode(arg0, arg1, 0);
  arg1->m_VarF32[1] = 1.0f;
  arg1->m_VarF32[2] = 1.0f;
  if (CScene__GetInstanceSavedBit((&gScene), arg0) == 1)
  {
    new_var = ~0x2000;
    arg0->m_dwFlags = (arg0->m_dwFlags & new_var) & (~0x100);
  }
}
