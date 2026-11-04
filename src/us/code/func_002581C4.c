#include "common.h"
#include "structs/player.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *player;
} Owner;

extern f32 D_800AE350;

extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern s32 func_00243414(void *obj, CAI *state, s32 mode);

/* Weapon state: sets the fire-rate scale D_800AE350 from the time held for
 * weapons 2-3 (and their 0x1294 ramp) and 12, then goes to state 7 or 5. */
void func_002581C4(Owner *obj, CAI *state) {
    CPlayer *player = obj->player;
    f32 t;
    f32 scale;

    if (player->m_ContState.m_dwButtons & 0x2000) {
        return;
    }
    switch (player->m_Data.m_CurrentWeapon) {
    case 2:
    case 3:
        if (player->m_SniperMode) {
            D_800AE350 = 1.5f;
        } else {
            scale = MAX(MIN(state->m_ModeTime * (2.0f / 21), 1.5f), 0.5f);
            D_800AE350 = scale;
            t = state->m_ModeTime * (2.0f / 15) + 0.5f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            player->weapon_particle_damage_scaler = t;
        }
        break;
    case 12:
        D_800AE350 = MAX(MIN(state->m_ModeTime * (2.0f / 21), 2.0f), 0.5f);
        break;
    }
    if (func_0024E700(player, player->m_Data.m_CurrentWeapon) == 1) {
        func_00243414(obj, state, 7);
    } else {
        func_00243414(obj, state, 5);
    }
}
