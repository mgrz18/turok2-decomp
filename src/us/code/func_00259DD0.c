#include "common.h"
#include "structs/game_object.h"


s32 func_0024E700(void *player, s32 weapon);
void func_00243414(CGameObjectInstance *actor, void *state, s32 mode);

void func_00259DD0(CGameObjectInstance *actor, void *state) {
    if (func_0024E700(actor->m_AI.m_pParent, 0x1F) == 1) {
        func_00243414(actor, state, 7);
    } else {
        func_00243414(actor, state, 4);
    }
}
