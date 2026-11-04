#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

typedef struct {
    /* 0x00 */ CGameObjectInstance *player;
} PlayerState;

extern PlayerState D_800F1CE0;

void func_00245828(DamageEvent *event, CGameObjectInstance *source, s32 damage, f32 radius, s32 flags, void *shield);
void func_002465C8(CGameObjectInstance *actor, void *ai, DamageEvent *event);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_002613D8(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;
    PlayerState *ps;

    if (ai->m_Health != 0 && ai->m_Mode == 0xE) {
        ps = &D_800F1CE0;
        if (ps->player != NULL && (ps->player == ai->m_pSightTarget || ps->player == ai->m_pPathTarget)) {
            func_00245828(&event, self, 0x500, 25.599998f, 0x100000, NULL);
            func_002465C8(ps->player, ((u8 *)&ps->player->m_AI), &event);
            func_00243414(self, ai, 4);
        }
    }
}
