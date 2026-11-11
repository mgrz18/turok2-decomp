#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/model.h"

s32 Render_DrawModelShared(sptr, sptr, s32, sptr, sptr, s32, s32);
s32 CGameObjectInstance__DrawModelImmediate(sptr, sptr, sptr);

/* Host body (C23): the N64 code reads the draw args' model handle (0x0C), the actor's draw
 * matrices (0x98) and lights (0x110 + gEvenOdd * 0x18) and the low byte of m_nModel (0x1B, big
 * endian) at N64 offsets, with the actor and the args as s32. */
void EnemyAI_DrawNode(sptr arg0, sptr arg1, sptr arg2) {
    CGameObjectInstance *actor = (CGameObjectInstance *) arg0;
    CDrawArgs *args = (CDrawArgs *) arg2;

    CGameObjectInstance__DrawModelImmediate(arg0, arg1, arg2);
    Render_DrawModelShared(args->m_hModel, (sptr) actor->m_pmtDrawMtxs, 1, (sptr) &actor->m_Lights[gEvenOdd], 0,
                           (s8) args->m_nModel, -1);
}
#else

s32 Render_DrawModelShared(sptr, sptr, s32, sptr, sptr, s32, s32);
s32 CGameObjectInstance__DrawModelImmediate(sptr, sptr, sptr);


void EnemyAI_DrawNode(s32 arg0, s32 arg1, s32 arg2)
{
  int new_var;
  CGameObjectInstance__DrawModelImmediate(arg0, arg1, arg2);
  new_var = 0x110;
  Render_DrawModelShared(*((s32 *) (((s8 *) arg2) + 0xC)), *((s32 *) (((s8 *) arg0) + 0x98)), 1, arg0 + ((gEvenOdd * 0x18) + new_var), 0, (s32) (*((s8 *) (((s8 *) arg2) + 0x1B))), -1);
}
#endif
