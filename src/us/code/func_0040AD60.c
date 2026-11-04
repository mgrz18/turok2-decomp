#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module init: collects up to 4 actors of type (0x44E, 9) into the module state, hooks their callbacks,
 * sets up the first phase (func_0040AE24) and queues func_0040C65C. */

extern u8 D_800F7078[];
extern s32 func_00225F6C(void *container, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
extern void func_0040BEE4(void);
extern void func_0040A25C(void);
extern void func_0040AE24(void *obj, void *ai);
extern void func_0040C65C(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

void func_0040AD60(void *obj, void *ai) {
    MotherBossState *state = &D_8012FB70;
    CGameObjectInstance *actor;
    s32 i;

    state->count = func_00225F6C(D_800F7078, 0x44E, 9, -1, state->actors, 4);
    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_pfDraw = func_0040BEE4;
        actor->m_AI.m_pfEffects = func_0040A25C;
    }
    func_0040AE24(obj, ai);
    func_00283FF8(0xC8, func_0040C65C, NULL);
}
