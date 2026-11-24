#include "common.h"
#include "globals/engine.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "protos/weapon_mode.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

f32 Math_DecayToZero(f32 value, f32 rate);

/* A weapon's spinning barrel (an AI callback): the weapon actor's CAI keeps the mode and
 * the barrel's angle and speed (m_Weapon), the actor's parent is the player, whose
 * controller state takes the rumble. */
void WeaponBarrel_Spin_AF324(CGameObjectInstance *obj, CAI *s) {
    CPlayer *owner = (CPlayer *) obj->m_AI.m_pParent;
    struct ControllerProfile *target;

    if (s->m_Mode != 4) {
        s->m_Weapon.m_BarrelSpeed = Math_DecayToZero(s->m_Weapon.m_BarrelSpeed, 0.0065449853f);
    }
    s->m_Weapon.m_BarrelAngle += s->m_Weapon.m_BarrelSpeed * gFrameIncrement * 2;
    target = owner->m_ContState.m_pContState;
    target->rumbleBase = (ABS(s->m_Weapon.m_BarrelSpeed) * 1.7904929f > 1.0f) ? 1.0f : ABS(s->m_Weapon.m_BarrelSpeed) * 1.7904929f;
    if (s->m_Mode != 4 && (gFrameCountGameplay & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}
