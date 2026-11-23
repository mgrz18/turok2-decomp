#include "common.h"
#include "structs/player.h"

extern f32 Math_Approach(f32 value, f32 target, f32 rate);
extern void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 sound);
extern void CPlayer__StopWeaponBackgroundSound(CPlayer *player);

void WeaponActor_Sway_AF084(void *obj, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)obj)->m_AI.m_pParent);
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
            CPlayer__StartWeaponBackgroundSound(player, 0x467);
        }
    } else {
        rate = 0.008726647124f;
        target = -0.017453294f;
        if (state->m_Weapon.m_BarrelSpeed > 0.0f) {
            target = 0.017453294f;
        }
        if (player->m_pLoopingSound_WeaponBackground != 0) {
            CPlayer__StopWeaponBackgroundSound(player);
        }
    }
    tilt = Math_Approach(state->m_Weapon.m_BarrelSpeed, target, rate);
    state->m_Weapon.m_BarrelSpeed = tilt;
    state->m_Weapon.m_BarrelAngle -= tilt;
}
