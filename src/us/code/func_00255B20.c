#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 damping;
} Unk800A71A8;
extern s32 D_800A71A8;   /* an Unk800A71A8 */

/* Byte-offset field access keeps the original load order. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void PlayerState12_Enter(CPlayer *arg0, CGameObjectInstance *arg1) {
    FIELD_AT(arg0, f32, CPlayer, m_Speed) = (FIELD_AT(arg0, f32, CPlayer, m_Speed) * FIELD_AT(&D_800A71A8, f32, Unk800A71A8, damping));
    FIELD_AT(arg0, f32, CPlayer, m_SideSpeed) = (FIELD_AT(arg0, f32, CPlayer, m_SideSpeed) * FIELD_AT(&D_800A71A8, f32, Unk800A71A8, damping));
    FIELD_AT(arg1, f32, CGameObjectInstance, m_vVelocity.y) = (FIELD_AT(arg1, f32, CGameObjectInstance, m_vVelocity.y) * FIELD_AT(&D_800A71A8, f32, Unk800A71A8, damping));
}
