#include "common.h"
#include "globals/boss.h"
#include "globals/onscreen.h"
#include "protos/ai.h"

s32 COnScreen__SetBoss(sptr, sptr, sptr);

#include "structs/game_object.h"
#include "structs/boss.h"

/* Host body: the ROM writes script3 and script2 at N64 deltas from script3.cursor (-4 the steps,
 * -0xC script2On, -0x14/-0x10 script2's cursor/current, -0x18 script2's steps): with the host's
 * 8-byte step pointers those deltas land in the wrong members (script3.steps half-written: the
 * first Primagen_NextSoundStep_2 crashed). The same stores by member. */
void Primagen_UpdateMode28_2(sptr arg0, sptr arg1)
{
  COnScreen__SetBoss((sptr) (&gEngineApp_m_OnScreen), arg0, (sptr) "Primagen");
  gLevelScript.script3.cursor = -1;
  gLevelScript.script3.steps = sPrimagenSoundScript3_2;
  gLevelScript.script2On = 1;
  gLevelScript.script3.current = -1;
  gLevelScript.script2.cursor = -1;
  gLevelScript.script2.current = -1;
  gLevelScript.script2.steps = sPrimagenSpawnScript1_2;
  CAI__SetMode(arg0, arg1, 5);
}
