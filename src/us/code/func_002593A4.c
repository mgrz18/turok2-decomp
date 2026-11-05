#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 state;
} Arg1;

f32 func_00210894(s32, f32);

void func_002593A4(s32 arg0, Arg1 *arg1) {
    if (arg1->state == 2) {
        func_00210894(arg0, arg1->owner->m_AI.m_Weapon.m_BarrelAngle);
    }
}
