/* The second VM module's copy of src/us/vm1/Primagen/func_00407374.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"

extern f32 Random_Blend(f32 lo, f32 hi);

void Primagen_UpdateMode5_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        gLevelScript_bossMode.mode = 5;
        gLevelScript_bossMode.timer = Random_Blend(0.5f, 4.0f) * 15.0f;
        CAI__SetMode(obj, ai, 1);
    }
    if (gLevelScript_bossMode.mode == 0) {
        CAI__SetMode(obj, ai, 6);
    }
}
