#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

extern f32 D_800B6D28; /* frame delta */

void DamageEvent_Set(DamageEvent *event, void *target, s32 damage, f32 radius, s32 flags, void *shield);
void CAI__CallHurt(CGameObjectInstance *self, CAI *ai, DamageEvent *event);
void Sound__Play(s16 sound, CVector3 pos, s32 arg4, s32 arg5);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);

void EnemyMode_ReactionShocked_Update(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (ai->m_ConditionTimer > 300.0f) {
        ai->m_ConditionTimer = 300.0f;
    }
    DamageEvent_Set(&event, NULL, D_800B6D28 * 34.133335f, 51.199997f, 0x100000, NULL);
    CAI__CallHurt(self, ai, &event);
    Sound__Play(0x227, self->header.m_vPos, 0, -1);
    ai->m_ConditionTimer -= D_800B6D28;
    if (ai->m_ConditionTimer <= 0.0f) {
        CAI__SetMode(self, ai, 10);
    }
}
