#include "common.h"
#include "globals/boss.h"

s32 CAI__SetMode(s32, s32, s32);
s32 MotherBoss_TeleportTarget(s32, s32);


void MotherBoss_Mode16_TeleportTarget(s32 arg0, void *arg1)
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
      CAI__SetMode(arg0, (s32) arg1, 0x11);
    }
  }
}
