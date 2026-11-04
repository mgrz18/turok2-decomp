#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

extern f32 D_800B6D28; /* frame delta */

void func_00245828(DamageEvent *event, void *target, s32 damage, f32 radius, s32 flags, void *shield);
void func_002465C8(CGameObjectInstance *self, CAI *ai, DamageEvent *event);
void func_00275544(s16 sound, CVector3 pos, s32 arg4, s32 arg5);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_0025FAD0(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (ai->m_ConditionTimer > 300.0f) {
        ai->m_ConditionTimer = 300.0f;
    }
    func_00245828(&event, NULL, D_800B6D28 * 34.133335f, 51.199997f, 0x100000, NULL);
    func_002465C8(self, ai, &event);
    func_00275544(0x227, self->header.m_vPos, 0, -1);
    ai->m_ConditionTimer -= D_800B6D28;
    if (ai->m_ConditionTimer <= 0.0f) {
        func_00243414(self, ai, 10);
    }
}
