#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 state;
} Arg1;

f32 CMtxF__PreRotateY(sptr, f32);

void WeaponNode_Rotate_AF084(sptr arg0, Arg1 *arg1) {
    if (arg1->state == 2) {
        CMtxF__PreRotateY(arg0, arg1->owner->m_AI.m_Weapon.m_BarrelAngle);
    }
}
