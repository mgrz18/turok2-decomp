#include "common.h"
#include "globals/boss.h"
#include "protos/ai.h"

s32 MotherBoss_TeleportTarget(sptr, s32);


#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

/* Host body (round 9 x2): the ROM reads CAI::m_AnimRepeatedCount at N64 0xC7 and gMotherBoss.counter
 * at N64 0x67C; on the host both members are elsewhere (CAI and MotherBossState hold pointers), so the
 * test read other bytes, the counter never reached 31 and the Mother stayed in mode 16 (her warp
 * animation) for good after phase 1's first teleport attack: no more attacks, no more hits. Every
 * third repeat of the animation teleports the target (MotherBoss_TeleportTarget), after 31 mode 17. */
void MotherBoss_Mode16_TeleportTarget(sptr arg0, void *arg1) {
    CAI *ai = arg1;
    MotherBossState *s = &gMotherBoss;
    s32 count;

    if (ai->m_AnimRepeatedCount != 0) {
        if (s->counter == (s->counter / 3) * 3) {
            MotherBoss_TeleportTarget(arg0, s->counter == 0);
        }
        count = s->counter + 1;
        s->counter = count;
        if (count >= 0x1F) {
            CAI__SetMode((void *) arg0, ai, 0x11);
        }
    }
}
#else
void MotherBoss_Mode16_TeleportTarget(sptr arg0, void *arg1)
{
  s32 temp_v0;
  s32 *new_var;
  if ((*((s8 *) (((s8 *) arg1) + 0xC7))) != 0)
  {
    new_var = &gMotherBoss;
    if ((*((s32 *) (((s8 *) (&gMotherBoss)) + 0x67C))) == (((*((s32 *) (((s8 *) new_var) + 0x67C))) / 3) * 3))
    {
      MotherBoss_TeleportTarget(arg0, (*((s32 *) (((s8 *) (&gMotherBoss)) + 0x67C))) == 0);
    }
    temp_v0 = (*((s32 *) (((s8 *) (&gMotherBoss)) + 0x67C))) + 1;
    *((s32 *) (((s8 *) new_var) + 0x67C)) = temp_v0;
    if (temp_v0 >= 0x1F)
    {
      CAI__SetMode(arg0, arg1, 0x11);
    }
  }
}
#endif
