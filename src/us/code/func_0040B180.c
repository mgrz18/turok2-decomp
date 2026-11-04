#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Clears the 0x250/0x254 fields of every actor of the module, resets the module (func_0040A5B4) and queues func_0040C6F4. */

extern void func_0040A5B4(void *obj, void *ai);
extern void func_0040C6F4(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

void func_0040B180(void *obj, void *ai) {
    MotherBossState *state = &D_8012FB70;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_VarBuffer[2] = 0;
        actor->m_AI.m_VarBuffer[1] = 0;
    }
    func_0040A5B4(obj, ai);
    func_00283FF8(0xD2, func_0040C6F4, NULL);
}
