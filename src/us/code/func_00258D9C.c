#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 mode;
} WeaponState;

extern void CMtxF__PreRotateY(void *arg0, f32 arg1);
extern void CMtxF__RotateY(void *arg0, f32 arg1);

void WeaponBarrel_NodeMatrix_AEE38(void *arg0, WeaponState *state) {
    f32 value = state->owner->m_AI.m_Weapon.m_BarrelAngle;
    f32 half = value * 0.5f;
    f32 more = value * 1.5f;

    switch (state->mode) {
    case 1:
        CMtxF__PreRotateY(arg0, half);
        break;
    case 7:
        CMtxF__RotateY(arg0, more);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        CMtxF__PreRotateY(arg0, -more);
        break;
    }
}
