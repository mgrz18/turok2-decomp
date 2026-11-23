#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/enemy.h"
#include "protos/ai.h"
#include "globals/controller.h"

/* Three override slots, 6 bytes apart. */

s32 EnemyAnim_JoypadTestPlayAnim(CGameObjectInstance *actor, void *state) {
    u16 *slots = &gContPadData[1].button;
    s32 r;

    if (slots[0] != 0) {
        r = AI_PickAnimFromTable(actor, state, sJoypadTestAnimTable, slots[0]);
    } else if (slots[3] != 0) {
        r = AI_PickAnimFromTable(actor, state, sJoypadTestAnimTable, slots[3]) + 100;
    } else if (slots[6] == 0) {
        r = AI_PickAnimFromTable(actor, state, (u8 *) sEnemyAnimIdle_AF6F0, actor->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
    } else {
        r = AI_PickAnimFromTable(actor, state, sJoypadTestAnimTable, slots[6]) + 200;
    }
    return r;
}
