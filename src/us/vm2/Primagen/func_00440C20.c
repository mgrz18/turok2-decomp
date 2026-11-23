
#include "common.h"
#include "globals/boss.h"
#include "globals/onscreen.h"
#include "protos/ai.h"
s32 COnScreen__SetBoss(s32 *, s32, char *);
void Primagen_UpdateMode28_2(s32 arg0, s32 arg1)
{
  COnScreen__SetBoss(&gEngineApp_m_OnScreen, arg0, "Primagen");
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + 0)) = -1;
  *((s32 **) (((s8 *) (&gLevelScript.script3.cursor)) + (-4))) = (s32 *) sPrimagenSoundScript3_2;
  *((s8 *) (((s8 *) (&gLevelScript.script3.cursor)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + 4)) = -1;
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + (-0x14))) = -1;
  *((s32 *) (((s8 *) (&gLevelScript.script3.cursor)) + (-0x10))) = -1;
  *((s32 **) (((s8 *) (&gLevelScript.script3.cursor)) + (-0x18))) = (s32 *) sPrimagenSpawnScript1_2;
  CAI__SetMode(arg0, arg1, 5);
}
