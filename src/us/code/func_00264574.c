#include "common.h"
#include "globals/engine.h"
#include "protos/ai.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the intelligence from CGameObjectInstance (N64 0x14), the AI's m_dwFlags and
 * m_ConditionTimer (N64 0x64); the platform body (asset data) keeps its byte offsets. */
void Platform_ForwardUpdate(void *arg0, void *arg1) {
    CGameObjectInstance *actor = (CGameObjectInstance *) arg0;
    CAI *ai = (CAI *) arg1;
    u8 *info = (u8 *) actor->header.m_pIntelligence + 0x14;
    s32 flags = *(s32 *) (info + 0x24);
    s32 ok = 1;

    if (flags & 0x10000) {
        ok = (u32) (ai->m_dwFlags & 0x20000) < 1;
    }
    if ((flags & 0x4000) && !(ai->m_dwFlags & 0x20000)) {
        ok = 0;
    }
    if (ok != 0) {
        ai->m_ConditionTimer += gFrameIncrement / *(f32 *) (info + 0x38);
    }
    if (ai->m_ConditionTimer >= 1.0f) {
        CAI__SetMode(arg0, arg1, 2);
    }
}
#else
void Platform_ForwardUpdate(void *arg0, void *arg1)
{
  s32 temp_a3;
  s32 var_a2;
  void *temp_t0;
  unsigned int new_var;
  temp_t0 = (*((s32 *) (((s8 *) arg0) + 0x14))) + 0x14;
  temp_a3 = *((s32 *) (((s8 *) temp_t0) + 0x24));
  var_a2 = 1;
  if (temp_a3 & 0x10000)
  {
    new_var = (*((s32 *) (((s8 *) arg1) + 0))) & 0x20000;
    var_a2 = new_var < 1;
  }
  if ((temp_a3 & 0x4000) && (!((*((s32 *) (((s8 *) arg1) + 0))) & 0x20000)))
  {
    var_a2 = 0;
  }
  if (var_a2 != 0)
  {
    *((f32 *) (((s8 *) arg1) + 0x64)) = (*((f32 *) (((s8 *) arg1) + 0x64))) + (gFrameIncrement / (*((f32 *) (((s8 *) temp_t0) + 0x38))));
  }
  if ((*((f32 *) (((s8 *) arg1) + 0x64))) >= 1.0f)
  {
    CAI__SetMode(arg0, arg1, 2);
  }
}
#endif
