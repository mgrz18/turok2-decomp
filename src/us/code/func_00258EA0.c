#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

extern f32 Math_DecayToZero(f32 value, f32 rate);  /* moves value toward zero */
extern void Sound__StopSoundId(s32 sound);
extern s32 D_800B6D20;      /* frame counter */
extern f32 D_800B6D28;      /* frame time scale */

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* Spins down unless held (state 4), turns the angle and drives the rumble
 * (the controller's rumbleBase) and the spin sound from the spin speed. */
void WeaponBarrel_Spin_AEE38(CGameObjectInstance *actor, CAI *sp) {
    CPlayer *owner;
    ControllerProfile *pad;

    owner = ((CPlayer *)actor->m_AI.m_pParent);
    if (sp->m_Mode != 4) {
        sp->m_Weapon.m_BarrelSpeed = Math_DecayToZero(sp->m_Weapon.m_BarrelSpeed, 0.0065449853f);
    }
    sp->m_Weapon.m_BarrelAngle += sp->m_Weapon.m_BarrelSpeed * D_800B6D28 * 2.0f;
    pad = owner->m_ContState.m_pContState;
    pad->rumbleBase = (ABS(sp->m_Weapon.m_BarrelSpeed) * 1.7904929f > 1.0f) ? 1.0f : ABS(sp->m_Weapon.m_BarrelSpeed) * 1.7904929f;
    if (sp->m_Mode != 4 && (D_800B6D20 & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}
