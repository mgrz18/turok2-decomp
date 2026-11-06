#include "common.h"
#include "structs/player.h"

/* Byte-offset field access keeps the original load order. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void PlayerState12_Enter(CPlayer *arg0, CGameObjectInstance *arg1) {
    FIELD_AT(arg0, f32, CPlayer, m_Speed) = (FIELD_AT(arg0, f32, CPlayer, m_Speed) * 0.5f);
    FIELD_AT(arg0, f32, CPlayer, m_SideSpeed) = (FIELD_AT(arg0, f32, CPlayer, m_SideSpeed) * 0.5f);
    FIELD_AT(arg1, f32, CGameObjectInstance, m_vVelocity.y) = (FIELD_AT(arg1, f32, CGameObjectInstance, m_vVelocity.y) * 0.5f);
}
