#include "common.h"
#include "globals/boss.h"
#include "protos/ai.h"

s32 MotherBoss_TeleportTarget(sptr, s32);


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
