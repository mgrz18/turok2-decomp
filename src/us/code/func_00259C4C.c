#include "common.h"
#include "structs/player.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 rumble;
} Controller;

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 time;
} WeaponState;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *player;
} Owner;

extern s16 D_800AE9D2;

extern f32 func_002119FC(f32 lo, f32 hi); /* random in range */
extern s32 func_00243414(void *obj, WeaponState *state, s32 mode);

/* Weapon state: on release, charges by the time held (0.1 + t/45, at most 1)
 * and sets the shot count D_800AE9D2 (charge * 3, at least 1), then goes to
 * state 5. */
void func_00259C4C(Owner *obj, WeaponState *state) {
    f32 base = 0.1f;
    CPlayer *player = obj->player;
    f32 charge;
    f32 n;
    f32 t;
    s16 shots;

    ((Controller *)player->m_ContState.m_pContState)->rumble = func_002119FC(base, 0.3f);
    if (player->m_ContState.m_dwButtons & 0x2000) {
        return;
    }
    t = state->time * (1.0f / 45) + base;
    charge = MIN(t, 1.0f);
    player->weapon_particle_damage_scaler = charge;
    n = charge * 3.0f;
    shots = (n < 1.0f) ? 1 : (s32) n;
    D_800AE9D2 = shots;
    func_00243414(obj, state, 5);
    ((Controller *)player->m_ContState.m_pContState)->rumble = 0.0f;
}
