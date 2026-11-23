#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];

void Primagen_UpdateMode6_2(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(gLevelScript.found[1], &gLevelScript.found[1]->m_AI, 1);
        CAI__SetMode(gLevelScript.found[0], &gLevelScript.found[0]->m_AI, 1);
        gLevelScript.bossMode = 8;
        D_800F1C40[0] |= 0x300;
        D_800F1F50[0] |= 0x300;
        CAI__SetMode(arg0, arg1, 0x12);
    }
}
