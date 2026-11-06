#include "common.h"
#include "globals/boss.h"
#include "globals/onscreen.h"

s32 CAI__SetMode(s32, s32, s32);
s32 COnScreen__SetBoss(s32, s32, s32);

void Primagen_UpdateMode28(s32 arg0, int arg1)
{
  s32 *new_var3;
  BossStep2 **new_var2;
  int new_var;
  COnScreen__SetBoss((s32) (&gEngineApp_m_OnScreen), arg0, (s32) "Primagen");
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + 0)) = -1;
  *((BossStep3 **) (((s8 *) (&gLevelScript.script3.cursor)) + (-4))) = sPrimagenSoundScript3;
  *((s8 *) (((s8 *) (&gLevelScript.script3.cursor)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + 4)) = -1;
  new_var3 = (s32 *) ((-0x14) + ((s8 *) (&gLevelScript.script3.cursor)));
  *new_var3 = -1;
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + (new_var = -0x10))) = -1;
  *(new_var2 = (BossStep2 **) (((s8 *) (&gLevelScript.script3.cursor)) + (-0x18))) = sPrimagenSpawnScript1;
  CAI__SetMode(arg0, arg1, 5);
}
