#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

s32 AI_UpdateTargets(sptr, sptr, sptr, s32, s32);

void BlindOne_BossMode3Update_2(sptr arg0, CAI *arg1)
{
#ifdef TARGET_PORT
  u8 sp18[sizeof(CAITargets)]; /* soak S7 */
#else
  u8 sp18[0x88];
#endif
  f32 new_var;
  AI_UpdateTargets(arg0, (sptr) arg1, (sptr) (&sp18), 0, 1);
  new_var = arg1->m_ModeTime;
  if (arg1->m_ConditionTimer < new_var)
  {
    CAI__SetMode(arg0, arg1, 5);
  }
}
