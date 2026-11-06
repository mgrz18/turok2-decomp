#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/enemy.h"

/* Three override slots, 6 bytes apart. */
extern u16 D_800F5C66[];
s32 AI_PickAnimFromTable(CGameObjectInstance *actor, void *state, void *table, s32 anim);

s32 EnemyAnim_JoypadTestPlayAnim(CGameObjectInstance *actor, void *state) {
    u16 *slots = D_800F5C66;
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
