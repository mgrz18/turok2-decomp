#include "common.h"
#include "globals/engine.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/controller.h"
/* Host body: SpinState is the weapon actor's CAI (m_Mode, m_Weapon.m_BarrelAngle/Speed),
 * SpinObj the actor (owner = m_AI.m_pParent), SpinOwner the player (N64 0xA50 =
 * m_ContState.m_pContState) and SpinTarget its controller state (0x168 rumbleBase). */
f32 Math_DecayToZero(f32 value, f32 rate);
void Sound__StopSoundId(s32 sound);

void WeaponBarrel_Spin_AF324(CGameObjectInstance *obj, CAI *s) {
    CPlayer *owner = (CPlayer *) obj->m_AI.m_pParent;
    f32 spin;

    if (s->m_Mode != 4) {
        s->m_Weapon.m_BarrelSpeed = Math_DecayToZero(s->m_Weapon.m_BarrelSpeed, 0.0065449853f);
    }
    s->m_Weapon.m_BarrelAngle += s->m_Weapon.m_BarrelSpeed * gFrameIncrement * 2;
    spin = ABS(s->m_Weapon.m_BarrelSpeed);
    owner->m_ContState.m_pContState->rumbleBase = (spin * 1.7904929f > 1.0f) ? 1.0f : spin * 1.7904929f;
    if (s->m_Mode != 4 && (gFrameCountGameplay & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}
#else
/* A spinning part driven by an AI callback (minimal layouts). */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 mode;
    /* 0x035 */ u8 pad35[0x10C - 0x35];
    /* 0x10C */ f32 angle;
    /* 0x110 */ f32 spin;
} SpinState;

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 rate;
} SpinTarget;

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ SpinTarget *target;
} SpinOwner;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ SpinOwner *owner;
} SpinObj;

f32 Math_DecayToZero(f32 value, f32 rate);
void Sound__StopSoundId(s32 sound);


void WeaponBarrel_Spin_AF324(SpinObj *obj, SpinState *s) {
    SpinOwner *owner = obj->owner;
    SpinTarget *target;

    if (s->mode != 4) {
        s->spin = Math_DecayToZero(s->spin, 0.0065449853f);
    }
    s->angle += s->spin * gFrameIncrement * 2;
    target = owner->target;
    target->rate = (ABS(s->spin) * 1.7904929f > 1.0f) ? 1.0f : ABS(s->spin) * 1.7904929f;
    if (s->mode != 4 && (gFrameCountGameplay & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}
#endif
