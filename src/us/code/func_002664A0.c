#include "common.h"
#include "m2c_macros.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 9 x1): by member (N64 0x180 = m_AI.m_ModeTime, 0xD4 = m_dwFlags). Through the
 * N64 offsets the rescued child's vanish read a float and cleared bits inside other fields. */
void Action_ChildVanishUpdate(void *arg0_) {
    CGameObjectInstance *arg0 = (CGameObjectInstance *) arg0_;

    if (arg0->m_AI.m_ModeTime > 30.0f) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000 & ~0x100);
    }
}
#else
void Action_ChildVanishUpdate(void *arg0)
{
  int new_var;
  new_var = ~0x2000;
  if ((*((f32 *) (((s8 *) arg0) + 0x180))) > 30.0f)
  {
    *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0xD4))) & new_var) & (~0x100));
  }
}
#endif
