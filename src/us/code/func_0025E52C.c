#include "common.h"

/* Pushes an AI state's alert level up when a noise of the given kind is
 * heard within 3072 units, remembering where it came from. */

typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ f32 alert;
    /* 0x48 */ u8 pad48[0xE8];
    /* 0x130 */ s8 modes[3];
    /* 0x133 */ s8 index;
} AlertState;

typedef struct {
    /* 0x00 */ s32 x, y, z;
} NoisePos;

f32 func_0020F6BC(void *, void *);
extern NoisePos D_800B06AC;

void func_0025E52C(u8 *actor, AlertState *state, NoisePos *pos, s32 kind) {
    s32 mode = state->modes[state->index];
    f32 strength;
    f32 dist;
    f32 alert;

    if (mode >= 0) {
    if (mode < 3) {
        switch (kind) {
        case 1:
            strength = 45.0f;
            break;
        case 2:
            strength = 105.0f;
            break;
        case 3:
            strength = 150.0f;
            break;
        case 0:
        default:
            return;
        }
        dist = func_0020F6BC(actor + 4, pos);
        if (dist < 3072.0f) {
            strength = strength * (1.0f / 3072.0f) * (3072.0f - dist);
            alert = state->alert;
            if (alert < strength) {
                state->alert = alert + strength;
                if (strength < alert + strength) {
                    state->alert = strength;
                }
                D_800B06AC = *pos;
            }
        }
    }
    }
}
