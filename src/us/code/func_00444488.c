#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

s32 CAI__SetMode(s32, s32, s32);
s32 MotherBoss_TeleportTarget_2(s32, s32);

void MotherBoss_Mode16_TeleportTarget_2(s32 arg0, CAI *ai)
{
  MotherBossState *new_var;
  s32 temp_v0;
  if (ai->m_AnimRepeatedCount != 0)
  {
    new_var = &D_8012FB70;
    if (D_8012FB70.counter == ((D_8012FB70.counter / 3) * 3))
    {
      MotherBoss_TeleportTarget_2(arg0, new_var->counter == 0);
    }
    temp_v0 = D_8012FB70.counter + 1;
    D_8012FB70.counter = temp_v0;
    if (temp_v0 >= 0x1F)
    {
      CAI__SetMode(arg0, (s32) ai, 0x11);
    }
  }
}
