#include "common.h"
#include "globals/boss.h"
#include "globals/onscreen.h"
#include "protos/ai.h"

s32 COnScreen__SetBoss(sptr, sptr, sptr);

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

/* Host body: the ROM writes script3 and script2 at N64 deltas from script3.cursor (-4 the steps,
 * -0xC script2On, -0x14/-0x10 script2's cursor/current, -0x18 script2's steps): with the host's
 * 8-byte step pointers those deltas land in the wrong members (script3.steps half-written: the
 * first Primagen_NextSoundStep crashed). The same stores by member. */
void Primagen_UpdateMode28(sptr arg0, sptr arg1)
{
  COnScreen__SetBoss((sptr) (&gEngineApp_m_OnScreen), arg0, (sptr) "Primagen");
  gLevelScript.script3.cursor = -1;
  gLevelScript.script3.steps = sPrimagenSoundScript3;
  gLevelScript.script2On = 1;
  gLevelScript.script3.current = -1;
  gLevelScript.script2.cursor = -1;
  gLevelScript.script2.current = -1;
  gLevelScript.script2.steps = sPrimagenSpawnScript1;
  CAI__SetMode(arg0, arg1, 5);
}
#else
void Primagen_UpdateMode28(sptr arg0, sptr arg1)
{
  s32 *new_var3;
  BossStep2 **new_var2;
  int new_var;
  COnScreen__SetBoss((sptr) (&gEngineApp_m_OnScreen), arg0, (sptr) "Primagen");
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
#endif
