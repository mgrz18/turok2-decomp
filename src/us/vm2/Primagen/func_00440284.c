/* The second VM module's copy of src/us/vm1/Primagen/func_00407284.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

void Primagen_EnterMode1_2(CGameObjectInstance *obj) {
    if (gLevelScript_bossState.mode == 5) {
        gLevelScript_bossState.state = 5;
    } else if (gLevelScript_bossState.mode == 6) {
        gLevelScript_bossState.state = 6;
    } else if (gLevelScript_bossState.mode == 10) {
        gLevelScript_bossState.state = 10;
    } else {
        gLevelScript_bossState.state = 6;
    }
    obj->m_dwFlags |= 0x2000;
}
