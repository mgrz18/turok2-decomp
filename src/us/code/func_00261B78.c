#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

typedef struct {
    /* 0x00 */ CGameObjectInstance *player;
    /* 0x04 */ u8 pad4[0x9C - 0x4];
    /* 0x9C */ s32 unk9C;
} PlayerState;

extern PlayerState D_800F1CE0;

void func_00245828(DamageEvent *event, CGameObjectInstance *source, s32 damage, f32 radius, s32 flags, void *shield);
void func_002465C8(CGameObjectInstance *actor, void *ai, DamageEvent *event);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_00261B78(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (D_800F1CE0.unk9C != 0) {
        ai->m_vLeashPos = ai->m_vInitialPos;
    }
    if (D_800F1CE0.player != NULL && ai->m_Mode == 0x10 && D_800F1CE0.player == ai->m_pPathTarget) {
        func_00245828(&event, self, 0x200, 25.599998f, 0x100000, NULL);
        func_002465C8(D_800F1CE0.player, ((u8 *)&D_800F1CE0.player->m_AI), &event);
        func_00243414(self, ai, 6);
    }
}
