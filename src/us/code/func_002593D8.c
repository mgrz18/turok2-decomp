#include "common.h"
#include "structs/player.h"

extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern void func_00253DE0(CPlayer *player, s32 sound);
extern void func_00253E68(CPlayer *player);

void func_002593D8(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    f32 target;
    f32 rate;
    f32 dir;
    f32 tilt;

    if (player->m_ContState.m_FinalMoveY > 0.0f && state->m_Mode != 1) {
        target = -0.41887906f;
        if (player->m_Speed > 0.0f) {
            target = 0.41887906f;
        }
        dir = 1.0f;
        rate = player->m_TotalSpeed * 0.0525f;
        if (player->m_Speed < 0.0f) {
            dir = -1.0f;
        }
        if (state->m_Weapon.m_BarrelSpeed < 0.0f ? dir != -1.0f : dir != 1.0f) {
            rate *= 4.0f;
        }
        if (player->m_pLoopingSound_WeaponBackground == 0) {
            func_00253DE0(player, 0x467);
        }
    } else {
        rate = 0.008726647124f;
        target = -0.017453294f;
        if (state->m_Weapon.m_BarrelSpeed > 0.0f) {
            target = 0.017453294f;
        }
        if (player->m_pLoopingSound_WeaponBackground != 0) {
            func_00253E68(player);
        }
    }
    tilt = func_00211774(state->m_Weapon.m_BarrelSpeed, target, rate);
    state->m_Weapon.m_BarrelSpeed = tilt;
    state->m_Weapon.m_BarrelAngle -= tilt;
}
