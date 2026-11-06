#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "globals/engine.h"

/* Weapon spin: turns the barrel and makes the controller rumble with the spin speed. */



extern f32 Math_Approach(f32 value, f32 target, f32 rate);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);
extern f32 func_002982F0(f32 x); /* sin */

void WeaponBarrel_Spin_AF1D4(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    f32 t;

    state->m_Weapon.m_BarrelAngle += state->m_Weapon.m_BarrelSpeed * gFrameIncrement;
    if (state->m_Mode == 4) {
        Math_EaseToward(&state->m_Weapon.m_BarrelSpeed, 0.47123894f, 0.025f);
    } else {
        state->m_Weapon.m_BarrelSpeed = Math_Approach(state->m_Weapon.m_BarrelSpeed, 0.08726647496f, 0.008f);
    }
    state->m_Weapon.m_SpinTime += gFrameIncrement;
    t = state->m_Weapon.m_BarrelSpeed * 2.122065783f;
    player->m_ContState.m_pContState->rumbleBase = t * t * (func_002982F0(state->m_Weapon.m_BarrelAngle * 1.5f) * 0.4f + 0.4f) + t * 0.05f;
}
