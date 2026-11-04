#include "common.h"
#include "structs/collision.h"

extern CCollisionResults D_800F1CE0;  /* the last collision test (m_pInstance: what it hit) */
void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, s32 arg5);
void func_002465C8(void *actor, void *behavior, DamageEvent *event);
s32 func_00243414(void *actor, void *state, s32 mode);

void func_00264CEC(u8 *actor, u8 *state) {
    DamageEvent event;
    u8 **player;

    if (*(s8 *) (state + 0x34) == 2) {
        player = (u8 **)&D_800F1CE0.m_pInstance;
        if (*player != NULL && D_800F1CE0.m_pInstance->header.m_Type == 1) {
            func_00245828(&event, actor, 0x500, 25.599998f, 0x100000, 0);
            func_002465C8(*player, *player + 0x140, &event);
            func_00243414(actor, state, 0);
        }
    }
}
