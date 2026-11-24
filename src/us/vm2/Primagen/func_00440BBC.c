#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* LevelScriptState (structs/boss.h) from scriptCOn on, gLevelScript_scriptCOn (+0x3C8):
 * scriptCOn and script3. */


void Primagen_EnterMode16_2(void) {
    gLevelScript.scriptCOn = 1;
    gLevelScript.script3.cursor = -1;
    gLevelScript.script3.current = -1;
    gLevelScript.script3.steps = sPrimagenSoundScript4_2;
}
