#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

extern u8 *D_800AF618[];

s32 func_00254564(CPlayer *arg0) {
    return *(s16 *)(D_800AF618[arg0->m_Data.m_CurrentWeapon] + 8) > 0;
}
