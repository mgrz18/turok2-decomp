#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"

/* Weapon spin: turns the barrel and makes the controller rumble with the spin speed. */


extern f32 D_800B6D28; /* frame time scale */

extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern void func_002117DC(f32 *value, f32 target, f32 rate);
extern f32 func_002982F0(f32 x); /* sin */

void func_00259944(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    f32 t;

    state->m_Weapon.m_BarrelAngle += state->m_Weapon.m_BarrelSpeed * D_800B6D28;
    if (state->m_Mode == 4) {
        func_002117DC(&state->m_Weapon.m_BarrelSpeed, 0.47123894f, 0.025f);
    } else {
        state->m_Weapon.m_BarrelSpeed = func_00211774(state->m_Weapon.m_BarrelSpeed, 0.08726647496f, 0.008f);
    }
    state->m_Weapon.m_SpinTime += D_800B6D28;
    t = state->m_Weapon.m_BarrelSpeed * 2.122065783f;
    player->m_ContState.m_pContState->rumbleBase = t * t * (func_002982F0(state->m_Weapon.m_BarrelAngle * 1.5f) * 0.4f + 0.4f) + t * 0.05f;
}
