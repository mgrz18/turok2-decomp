
#include "protos/ai.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif
s32 AI_UpdateTargets(sptr, sptr, sptr, s32, s32);
void BlindOne_BossMode3Update(sptr arg0, sptr arg1)
{
#ifdef TARGET_PORT
  u8 sp18[sizeof(CAITargets)]; /* soak S7 */
  AI_UpdateTargets(arg0, arg1, (sptr) (&sp18), 0, 1);
  if (((CAI *) arg1)->m_ModeTime > ((CAI *) arg1)->m_ConditionTimer) /* N64 0x40, 0x64 */
#else
  u8 sp18[0x88];
  AI_UpdateTargets(arg0, arg1, (sptr) (&sp18), 0, 1);
  if ((*((f32 *) (((s8 *) arg1) + 0x40))) > (*((f32 *) (((s8 *) arg1) + 0x64))))
#endif
  {
    CAI__SetMode(arg0, arg1, 5);
  }
}
